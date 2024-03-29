//
// Created by depaulsmiller on 3/18/21.
//

#include <Server.hh>
#include <spdlog/spdlog.h>
#include <cds/init.h>
#include <cds/gc/hp.h>

namespace score {

    Server::Server(int rank, std::vector<std::string> addresses, std::string clientAddr, std::string log) {
        cds::Initialize();
        cds::gc::hp::GarbageCollector::Construct(map_t::c_nHazardPtrCount + 1, 16, 16 );
        cds::threading::Manager::attachThread();

        SPDLOG_INFO("Starting server with rank {} accessible at {}", rank, clientAddr);

        CControlContextPair pair(rank, addresses.size(), log);

        server = RunServer(addresses[rank], pair.cc);
        serverF = std::async([this]() {
            server->Wait();
        });

        std::vector<std::shared_ptr<InternalClient>> servers;
        for (auto addr : addresses) {
            servers.push_back(std::make_shared<InternalClient>(addr));
        }
        client = RunClientSideServer(clientAddr, pair.ctx, servers);
        clientF = std::async([this]() {
            client->Wait();
        });
    }

    void Server::stop() {
        client->Shutdown();
        server->Shutdown();
        serverF.get();
        clientF.get();
    }

}