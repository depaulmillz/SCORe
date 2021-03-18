//
// Created by depaulsmiller on 2/9/21.
//

#include <string>
#include <tbb/concurrent_hash_map.h>
#include <list>
#include <shared_mutex>

#ifndef SCORE_MAP_HH
#define SCORE_MAP_HH

namespace score {

    using data_t = std::string;

    using mutex = std::shared_mutex;
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

    using map_t = tbb::concurrent_hash_map<data_t, std::shared_ptr<VersionList>>;
    using accessor_t = map_t::accessor;


}

#endif //SCORE_MAP_HH
