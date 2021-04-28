//
// Created by depaulsmiller on 3/18/21.
//


#include <Context.hh>
#include <spdlog/spdlog.h>
#include <thread>

namespace score {

    Context::Context(uint64_t rank_, uint64_t nodes_) : rank(rank_), nodes(nodes_), ts() {
    }

    Context::~Context() {

    }

    std::list<uint64_t> Context::replicas(const data_t &key) {
        std::list<uint64_t> l;
        l.push_front(std::hash<data_t>{}(key) % nodes);
        return l;
    }

    bool Context::thisNodeIsAReplica(const data_t &key) {
        return std::hash<data_t>{}(key) % nodes == rank;
    }


    bool Context::exclusiveUnlocked(const data_t &key) {
        // TODO implement
        return true;
    }

    bool Context::exclusiveLocked(const data_t &key) {

        accessor_t a;
        if (m.find(a, key)) {
            return a->second->mtx.isLocked_exclusive();
        }

        return false;
    }


    std::tuple<data_t, version_t, bool> Context::doRead(version_t sid, data_t &key, std::unique_lock<std::mutex> &stateLock) {
        SPDLOG_TRACE("{}(sid = {}, key = {})", __FUNCTION__, sid, key);

        ts.getNextID(stateLock) = std::max(ts.getNextID(stateLock), sid);

        SPDLOG_TRACE("{} while(commitID({}) < sid({}));", __FUNCTION__, ts.getCommitID(stateLock), sid);
        while (ts.getCommitID(stateLock) < sid && exclusiveLocked(key)){
            stateLock.unlock();
            std::this_thread::yield();
            stateLock.lock();
        }
        accessor_t a;
        if (m.find(a, key)) {
            std::shared_ptr<VersionList> v = a->second;
            shared_lock sl(v->mtx);
            bool isFirst = true;
            for (auto &elm : v->l) {
                if (elm.second <= sid) {
                    return {elm.first, ts.getCommitID(stateLock), isFirst};
                }
                isFirst = false;
            }
        }
        return {data_t(), ts.getCommitID(stateLock), START_VERSION};
    }

    void Context::updateNodeTimestamps(version_t lastCommitted, const std::unique_lock<std::mutex> &stateLock) {
        //SPDLOG_DEBUG("Updating node time stamps with lastCommited {}", lastCommitted);
        ts.getNextID(stateLock) = std::max(ts.getNextID(stateLock), lastCommitted);
        ts.getMaxSeen(stateLock) = std::max(ts.getMaxSeen(stateLock), lastCommitted);
        uponCondition(stateLock);
    }

    bool Context::getLocksWithTimeout(const std::map<data_t, bool> &toLock) {
        //TODO: implement better timeout
        for (auto &e : toLock) {
            accessor_t a;
            while (!m.find(a, e.first)) {
                m.insert(a, e.first);
                a->second = std::make_shared<VersionList>();
            }
            if (e.second) {
                if(!a->second->mtx.try_lock_shared())
                    return false;
            } else {
                if(!a->second->mtx.try_lock()){
                    return false;
                }
            }
        }
        return true;
    }

    bool Context::releaseLocks(const std::map<data_t, bool> &toLock) {
        for (auto &e : toLock) {
            accessor_t a;
            m.find(a, e.first);
            assert(!a.empty());
            if (e.second) {
                assert(a->second->mtx.isLocked_shared());
                a->second->mtx.unlock_shared();
            } else {
                assert(a->second->mtx.isLocked_exclusive());
                a->second->mtx.unlock();
            }
        }
        return true;
    }

    // need to be holding state mutex to call
    void Context::uponCondition(const std::unique_lock<std::mutex> &stateLock) {
        if (ts.getMaxSeen(stateLock) > ts.getCommitID(stateLock) && pendQ.empty() && stableQ.empty()) {
            ts.getCommitID(stateLock) = std::max(ts.getMaxSeen(stateLock), ts.getCommitID(stateLock));
        }
    }

    Context::q_t &Context::getStableQ(const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        return stableQ;
    }

    Context::p_t &Context::getPendQ(const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        return pendQ;
    }

    version_t &Context::getNextID(const std::unique_lock<std::mutex> &stateLock) {
        return ts.getNextID(stateLock);
    }

    version_t &Context::getCommitID(const std::unique_lock<std::mutex> &stateLock) {
        return ts.getCommitID(stateLock);
    }

    version_t &Context::getMaxSeen(const std::unique_lock<std::mutex> &stateLock) {
        return ts.getMaxSeen(stateLock);
    }

}
