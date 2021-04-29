//
// Created by depaulsmiller on 3/18/21.
//


#include <Context.hh>
#include <spdlog/spdlog.h>
#include <thread>

namespace score {

    Context::Context(uint64_t rank_, uint64_t nodes_, const std::string &logFileName) : rank(rank_), nodes(nodes_),
                                                                                        ts() {
        logFile.open(logFileName);
    }

    Context::~Context() {
        logFile.close();
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


    std::tuple<data_t, version_t, bool>
    Context::doRead(version_t sid, data_t &key, std::unique_lock<std::mutex> &stateLock) {
        SPDLOG_TRACE("{}(sid = {}, key = {})", __FUNCTION__, sid, key);

        ts.getNextID(stateLock) = std::max(ts.getNextID(stateLock), sid);

        SPDLOG_TRACE("{} while(commitID({}) < sid({}));", __FUNCTION__, ts.getCommitID(stateLock), sid);
        while (ts.getCommitID(stateLock) < sid && exclusiveLocked(key)) {
            stateLock.unlock();
            std::this_thread::yield();
            SPDLOG_TRACE("Yielding");
            stateLock.lock();
        }
        accessor_t a;
        if (m.find(a, key)) {
            std::shared_ptr<VersionList> v = a->second;
            SPDLOG_TRACE("Shared locking {}", key);
            shared_lock sl(v->mtx);
            SPDLOG_TRACE("Shared locked {}", key);

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
        SPDLOG_DEBUG("Getting locks");
        //TODO: implement better timeout
        int count = 0;
        for (auto &e : toLock) {
            SPDLOG_TRACE("Accessing {} in map", e.first);
            accessor_t a;
            while (!m.find(a, e.first)) {
                m.insert(a, e.first);
                a->second = std::make_shared<VersionList>();
            }
            SPDLOG_TRACE("Accessed {} in map", e.first);
            if (e.second) {
                if (!a->second->mtx.try_lock_shared()) {
                    a.release();

                    // unlock everything
                    int count2 = 0;
                    for (auto &e2 : toLock) {
                        if (count2 == count)
                            break;
                        m.find(a, e.first);
                        if (e2.second) {
                            a->second->mtx.unlock_shared();
                            SPDLOG_DEBUG("Shared unlocking {}", e.first);

                        } else {
                            a->second->mtx.unlock();
                            SPDLOG_DEBUG("Unlocking {}", e.first);
                        }
                        a.release();
                        count2++;
                    }

                    return false;
                }
                SPDLOG_DEBUG("Shared locked {}", e.first);

            } else {
                if (!a->second->mtx.try_lock()) {
                    a.release();

                    // unlock everything
                    int count2 = 0;
                    for (auto &e2 : toLock) {
                        if (count2 == count)
                            break;

                        m.find(a, e.first);
                        if (e2.second) {
                            a->second->mtx.unlock_shared();
                            SPDLOG_DEBUG("Shared unlocking {}", e.first);

                        } else {
                            a->second->mtx.unlock();
                            SPDLOG_DEBUG("Unlocking {}", e.first);
                        }
                        a.release();
                        count2++;
                    }

                    return false;
                }
                SPDLOG_DEBUG("Locked {}", e.first);

            }
            count++;
        }
        return true;
    }

    bool Context::releaseLocks(const std::map<data_t, bool> &toLock) {
        SPDLOG_DEBUG("Releasing locks");

        for (auto &e : toLock) {
            accessor_t a;
            m.find(a, e.first);
            assert(!a.empty());
            if (e.second) {
                assert(a->second->mtx.isLocked_shared());
                a->second->mtx.unlock_shared();
                SPDLOG_DEBUG("Shared unlocking {}", e.first);
            } else {
                assert(a->second->mtx.isLocked_exclusive());
                a->second->mtx.unlock();
                SPDLOG_DEBUG("Unlocking {}", e.first);
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

    std::ofstream &Context::getLogFile(const std::unique_lock<std::mutex> &stateLock) {
        return logFile;
    }

}
