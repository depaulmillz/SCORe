//
// Created by depaulsmiller on 3/18/21.
//

#include <CControlContextPair.hh>

namespace score {

    CControlContextPair::CControlContextPair(uint64_t rank, uint64_t nodes,
                                             const std::string &log) : ctx(new Context(rank, nodes, log)),
                                                                       cc(new CControl(
                                                                               ctx)) {

    }

    CControlContextPair::~CControlContextPair() {

    }
}

