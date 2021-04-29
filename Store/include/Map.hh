//
// Created by depaulsmiller on 2/9/21.
//

#include <string>
#include <tbb/concurrent_hash_map.h>
#include <list>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <cds/container/michael_kvlist_hp.h>
#include <cds/container/michael_map.h>

#ifndef SCORE_MAP_HH
#define SCORE_MAP_HH

namespace score {

    using data_t = std::string;

    static_assert(std::atomic<long>::is_always_lock_free, "ssize_t needs to be lock free");
    class mutex {
    public:
        mutex() : l(0) {
            
        }

        ~mutex(){

        }

        inline void lock(){
            while(!try_lock());
        }

        inline bool try_lock(){
            ssize_t expected = 0;
            if(l.load() != 0)
                return false;
            return l.compare_exchange_strong(expected, -1);
        }

        inline void lock_shared(){
            while(!try_lock_shared());
        }

        inline bool try_lock_shared(){
            ssize_t expected = l.load();
            if(expected >= 0)
                return l.compare_exchange_strong(expected, expected + 1);
            else
                return false;
        }

        inline void unlock(){
            l.store(0);
        }

        inline void unlock_shared(){
            l.fetch_sub(1);
        }

        inline bool isLocked_exclusive(){
            return l.load() == -1;
        }

        inline bool isLocked_shared(){
            return l.load() > 0;
        }

    private:
        std::atomic<long> l;
    };

    using unique_lock = std::unique_lock<mutex>;
    using shared_lock = std::shared_lock<mutex>;

    using version_t = uint64_t;
    const version_t START_VERSION = 0;

    struct VersionList {
        VersionList() {
            l.push_front({data_t(), START_VERSION});
        }

        VersionList(const VersionList &other) = delete;

        ~VersionList() {}

        mutex mtx;
        std::list<std::pair<data_t, version_t>> l;
    };


    using kvlist_t = cds::container::MichaelKVList<cds::gc::HP, data_t, std::shared_ptr<VersionList>>;

    using map_t = cds::container::MichaelHashMap<cds::gc::HP, kvlist_t>;

    //using map_t = tbb::concurrent_hash_map<data_t, std::shared_ptr<VersionList>>;
    //using accessor_t = map_t::accessor;


}

#endif //SCORE_MAP_HH
