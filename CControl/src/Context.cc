//
// Created by depaulsmiller on 3/18/21.
//


#include <Context.hh>
#include <spdlog/spdlog.h>
#include <thread>

namespace score {

    Context::Context(uint64_t rank_, uint64_t nodes_, const std::string &logFileName) : m(1000000, 1), rank(rank_),
                                                                                        nodes(nodes_),
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
        bool isLocked = false;
        m.find(key, [&isLocked](map_t::value_type &v) {
            isLocked = v.second->mtx.isLocked_exclusive();
        });

        return isLocked;
    }


    std::tuple<data_t, version_t, bool>
    Context::doRead(version_t sid, data_t &key, std::unique_lock<std::mutex> &stateLock) {
        if (!cds::threading::Manager::isThreadAttached())
            cds::threading::Manager::attachThread();

        SPDLOG_TRACE("{}(sid = {}, key = {})", __FUNCTION__, sid, key);

        ts.getNextID(stateLock) = std::max(ts.getNextID(stateLock), sid);

        SPDLOG_TRACE("{} while(commitID({}) < sid({}));", __FUNCTION__, ts.getCommitID(stateLock), sid);
        while (ts.getCommitID(stateLock) < sid && exclusiveLocked(key)) {
            stateLock.unlock();
            std::this_thread::yield();
            SPDLOG_TRACE("Yielding");
            stateLock.lock();
        }

        std::tuple<data_t, version_t, bool> retVal = {data_t(), ts.getCommitID(stateLock), START_VERSION};

        m.find(key, [&](map_t::value_type &v) {
            std::shared_ptr<VersionList> l = v.second;
            SPDLOG_TRACE("Shared locking {}", key);
            shared_lock sl(l->mtx);
            SPDLOG_TRACE("Shared locked {}", key);

            bool isFirst = true;
            for (auto &elm : l->l) {
                if (elm.second <= sid) {
                    SPDLOG_TRACE("Read {} -> {} at time {}", key, elm.first, elm.second);
                    retVal = {elm.first, ts.getCommitID(stateLock), isFirst};
                    return;
                }
                isFirst = false;
            }
        });

        return retVal;
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

            data_t cpyOfKey = e.first;

            bool failed = false;

            m.update(std::move(cpyOfKey), [&](bool bNew, map_t::value_type &item) {
                SPDLOG_TRACE("Accessed {} in map", e.first);

                if (bNew)
                    item.second = std::make_shared<VersionList>();

                if (e.second) {
                    if (!item.second->mtx.try_lock_shared())
                        failed = true;
                    else
                        SPDLOG_TRACE("shared lock {}", e.first);
                } else {
                    if (!item.second->mtx.try_lock())
                        failed = true;
                    else
                        SPDLOG_TRACE("lock {}", e.first);
                }
            });

            if (failed) {
                int count2 = 0;
                for (auto &e2 : toLock) {
                    if (count2 == count)
                        break;
                    m.find(e2.first, [&](map_t::value_type &item) {
                        if (e2.second) {
                            item.second->mtx.unlock_shared();
                            SPDLOG_DEBUG("Shared unlocking {}", e2.first);
                        } else {
                            item.second->mtx.unlock();
                            SPDLOG_DEBUG("Unlocking {}", e2.first);
                        }
                    });
                    count2++;
                }
                return false;
            }

            count++;
        }
        return true;
    }

    bool Context::releaseLocks(const std::map<data_t, bool> &toLock) {
        SPDLOG_DEBUG("Releasing locks");

        for (auto &e : toLock) {
            m.find(e.first, [&](map_t::value_type &item) {
                if (e.second) {
                    item.second->mtx.unlock_shared();
                    SPDLOG_DEBUG("Shared unlocking {}", e.first);
                } else {
                    item.second->mtx.unlock();
                    SPDLOG_DEBUG("Unlocking {}", e.first);
                }
            });
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
