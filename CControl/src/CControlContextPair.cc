//
// Created by depaulsmiller on 3/18/21.
//

#include <CControlContextPair.hh>

namespace score {

    CControlContextPair::CControlContextPair(uint64_t rank, uint64_t nodes) : ctx(new Context(rank, nodes)),
                                                                              cc(new CControl(ctx)) {

    }

    CControlContextPair::~CControlContextPair() {

    }
}

