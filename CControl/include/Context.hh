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
#include <spdlog/spdlog.h>

#ifndef SCORE_CONTEXT_HH
#define SCORE_CONTEXT_HH

namespace score {

    struct OpTriple {
        txid_t txid;
        uint64_t node;
        version_t sn;
    };

    struct Context {

        explicit Context(uint64_t rank_, uint64_t nodes_);

        ~Context();


        using q_t = std::queue<OpTriple>;
        using p_t = std::deque<OpTriple>;

        map_t m;

        std::mutex stateMtx;

        std::atomic_uint64_t txid_counter{0};
        uint64_t rank;
        uint64_t nodes;
        tbb::concurrent_hash_map<std::pair<txid_t, uint64_t>, Transaction> txMap;
        using txMapAccessor = tbb::concurrent_hash_map<std::pair<txid_t, uint64_t>, Transaction>::accessor;

        std::list<uint64_t> replicas(const data_t &key);

        bool thisNodeIsAReplica(const data_t &key);


        bool exclusiveUnlocked(const data_t &key);

        std::tuple<data_t, version_t, bool> doRead(version_t sid, data_t &key);

        void updateNodeTimestamps(version_t lastCommitted, const std::unique_lock<std::mutex> &stateLock);

        bool getLocksWithTimeout(const std::map<data_t, bool> &toLock);

        bool releaseLocks(const std::map<data_t, bool> &toLock);

        // need to be holding state mutex to call
        void uponCondition(const std::unique_lock<std::mutex> &stateLock);

        q_t &getStableQ(const std::unique_lock<std::mutex> &stateLock);

        p_t &getPendQ(const std::unique_lock<std::mutex> &stateLock);

        version_t &getNextID(const std::unique_lock<std::mutex> &stateLock);

        version_t &getCommitID(const std::unique_lock<std::mutex> &stateLock);

        version_t &getMaxSeen(const std::unique_lock<std::mutex> &stateLock);

    private:
        version_t nextID;
        version_t commitID;
        version_t maxSeen;

        p_t pendQ;
        q_t stableQ;
    };
}

#endif //SCORE_CONTEXT_HH
