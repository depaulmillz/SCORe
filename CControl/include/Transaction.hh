//
// Created by depaulsmiller on 2/9/21.
//

#include <unordered_map>
#include <Map.hh>

#ifndef SCORE_TRANSACTION_HH
#define SCORE_TRANSACTION_HH

namespace score {

    using txid_t = uint64_t;

    struct Transaction {
        bool firstRead = true;
        bool isUpdate = false;
        bool aborted = false;
        bool committed = false;
        version_t sid;
        std::unordered_map<data_t, data_t> ws;
        std::unordered_map<data_t, version_t> rs;
        txid_t txid;
    };
}

#endif //SCORE_TRANSACTION_HH
