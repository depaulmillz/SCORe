//
// Created by depaulsmiller on 3/18/21.
//

#ifndef SCORE_CCONTROLCONTEXTPAIR_HH
#define SCORE_CCONTROLCONTEXTPAIR_HH

#include <CControl.hh>

namespace score {
    struct CControlContextPair {
        CControlContextPair(uint64_t rank, uint64_t nodes);

        ~CControlContextPair();

        std::shared_ptr<Context> ctx;
        std::shared_ptr<CControl> cc;
    };

}

#endif //SCORE_CCONTROLCONTEXTPAIR_HH
