//
// Created by depaulsmiller on 3/18/21.
//

#include <Map.hh>
#include <Transaction.hh>
#include <queue>
#include <map>
#include <protocol.pb.h>
#include <future>
#include <atomic>
#include <utility>
#include <rpc/client.h>

#ifndef SCORE_CONTEXT_HH
#define SCORE_CONTEXT_HH

namespace score {
    struct Context {

        explicit Context(uint64_t rank_, uint64_t nodes_) : rank(rank_), nodes(nodes_) {

        }

        ~Context() {

        }


        using q_t = std::queue <std::pair<txid_t, version_t>>;
        using p_t = std::deque <std::pair<txid_t, version_t>>;

        map_t m;

        std::mutex stateMtx;

        std::atomic_uint64_t txid_counter{0};
        uint64_t rank;
        uint64_t nodes;
        tbb::concurrent_hash_map <txid_t, Transaction> txMap;
        using txMapAccessor = tbb::concurrent_hash_map<txid_t, Transaction>::accessor;

        std::list <uint64_t> replicas(const data_t &key) {
            std::list <uint64_t> l;
            l.push_front(std::hash < data_t > {}(key) % nodes);
            return l;
        }

        bool thisNodeIsAReplica(const data_t &key) {
            return std::hash < data_t > {}(key) % nodes == rank;
        }


        bool exclusiveUnlocked(const data_t &key) {
            // TODO implement
            return true;
        }

        std::tuple<data_t, version_t, bool> doRead(version_t sid, data_t &key) {
            nextID = std::max(nextID, sid);
            while (commitID < sid && exclusiveUnlocked(key));
            accessor_t a;
            if (m.find(a, key)) {
                std::shared_ptr <VersionList> v = a->second;
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

        void updateNodeTimestamps(version_t lastCommitted, const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            nextID = std::max(nextID, lastCommitted);
            maxSeen = std::max(maxSeen, lastCommitted);
        }

        bool getLocksWithTimeout(const std::map<data_t, bool> &toLock) {
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

        bool releaseLocks(const std::map<data_t, bool> &toLock) {
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
        void uponCondition(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            if (maxSeen > commitID && pendQ.empty() && stableQ.empty()) {
                commitID = std::max(maxSeen, commitID);
            }
        }

        q_t &getStableQ(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            return stableQ;
        }

        p_t &getPendQ(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            return pendQ;
        }

        version_t &getNextID(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            return nextID;
        }

        version_t &getCommitID(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            return commitID;
        }

        version_t &getMaxSeen(const std::unique_lock <std::mutex> &stateLock) {
            assert(stateLock.owns_lock());
            return maxSeen;
        }

    private:
        version_t nextID;
        version_t commitID;
        version_t maxSeen;

        p_t pendQ;
        q_t stableQ;


    };
}

#endif //SCORE_CONTEXT_HH
