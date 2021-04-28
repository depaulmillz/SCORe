//
// Created by depaulsmiller on 3/18/21.
//

#include <thread>
#include <vector>
#include <iostream>
#include <CControlAPI.hh>
#include <CControlRPC.hh>
#include "gtest/gtest.h"
#include "protocol.pb.h"

#ifndef SCORE_SERVER_HH
#define SCORE_SERVER_HH

namespace score {
    class Server {
    public:
        Server(int rank, std::vector<std::string> addresses, std::string clientAddr, std::string log);

        void stop();

    private:
        std::future<void> serverF;
        std::future<void> clientF;
        std::unique_ptr<grpc::Server> server;
        std::unique_ptr<grpc::Server> client;
    };
}

#endif //SCORE_SERVER_HH
