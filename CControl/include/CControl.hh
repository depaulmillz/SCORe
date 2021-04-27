//
// Created by depaulsmiller on 3/18/21.
//

#include <protocol.pb.h>
#include <spdlog/spdlog.h>
#include <Context.hh>

#ifndef SCORE_CCONTROL_HH
#define SCORE_CCONTROL_HH

namespace score {
    class CControl {
    public:
        explicit CControl(std::shared_ptr<Context> ctx);

        ~CControl();

        void DoReadRequest(const ReadRequest &request,
                           ReadReturn *response);

        void DoPrepare(const Prepare &request, Vote *response);

        void DoDecide(const Decide &request, Committed *response);

        void commitCondition(const std::unique_lock<std::mutex> &stateLock);

    private:
        std::shared_ptr<Context> ctx_;
    };
}

#endif //SCORE_CCONTROL_HH
