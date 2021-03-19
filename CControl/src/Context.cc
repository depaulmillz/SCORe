//
// Created by depaulsmiller on 3/18/21.
//


#include <Context.hh>

namespace score {

    Context::Context(uint64_t rank_, uint64_t nodes_) : rank(rank_), nodes(nodes_), nextID(0), commitID(0),
                                                        maxSeen(0) {
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

    std::tuple<data_t, version_t, bool> Context::doRead(version_t sid, data_t &key) {
        SPDLOG_TRACE("{}({}, {})", __FUNCTION__, sid, key);

        nextID = std::max(nextID, sid);

        SPDLOG_TRACE("{} while({} < {});", __FUNCTION__, commitID, sid);
        while (commitID < sid && exclusiveUnlocked(key));
        accessor_t a;
        if (m.find(a, key)) {
            std::shared_ptr<VersionList> v = a->second;
            shared_lock sl(v->mtx);
            bool isFirst = true;
            for (auto &elm : v->l) {
                if (elm.second <= sid) {
                    return {elm.first, commitID, isFirst};
                }
                isFirst = false;
            }
        }
        return {data_t(), commitID, START_VERSION};
    }

    void Context::updateNodeTimestamps(version_t lastCommitted, const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        nextID = std::max(nextID, lastCommitted);
        maxSeen = std::max(maxSeen, lastCommitted);
    }

    bool Context::getLocksWithTimeout(const std::map<data_t, bool> &toLock) {
        for (auto &e : toLock) {
            accessor_t a;
            while (!m.find(a, e.first)) {
                m.insert(a, e.first);
                a->second = std::make_shared<VersionList>();
            }
            if (e.second) {
                a->second->mtx.lock_shared();
            } else {
                a->second->mtx.lock();
            }
        }
        return true;
    }

    bool Context::releaseLocks(const std::map<data_t, bool> &toLock) {
        for (auto &e : toLock) {
            accessor_t a;
            m.find(a, e.first);
            if (e.second) {
                a->second->mtx.unlock_shared();
            } else {
                a->second->mtx.unlock();
            }
        }
        return true;
    }

    // need to be holding state mutex to call
    void Context::uponCondition(const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        if (maxSeen > commitID && pendQ.empty() && stableQ.empty()) {
            commitID = std::max(maxSeen, commitID);
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
        assert(stateLock.owns_lock());
        return nextID;
    }

    version_t &Context::getCommitID(const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        return commitID;
    }

    version_t &Context::getMaxSeen(const std::unique_lock<std::mutex> &stateLock) {
        assert(stateLock.owns_lock());
        return maxSeen;
    }

}
