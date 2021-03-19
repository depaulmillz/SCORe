//
// Created by depaulsmiller on 2/10/21.
//

#include <thread>
#include <vector>
#include <iostream>
#include <CControlAPI.hh>
#include <CControlRPC.hh>
#include "gtest/gtest.h"
#include "protocol.pb.h"
#include <TxClientAPI.hh>
#include <Server.hh>

using namespace score;

TEST(CControlTest, AllocateAndDelete) {
    spdlog::set_level(spdlog::level::trace);

    CControl c(std::make_shared<Context>(0, 1));
}

TEST(CControlTest, TestDoReadRequest) {
    spdlog::set_level(spdlog::level::trace);

    CControl c(std::make_shared<Context>(0, 1));
    score::ReadRequest req;
    ReadReturn res;
    req.set_txid(1);
    req.set_key("hi");
    req.set_firstread(true);
    req.set_readsid(0);

    c.DoReadRequest(req, &res);

    ASSERT_TRUE(res.value().empty()) << "String should be empty";
}

TEST(CControlTest, TestTwoPC) {
    spdlog::set_level(spdlog::level::trace);

    CControl c(std::make_shared<Context>(0, 1));
    Prepare req;
    Vote res;
    req.set_txid(1);
    req.set_nodeid(0);
    req.set_sid(1);
    auto r = req.add_rs();
    r->set_key("hi");
    r->set_ver(0);
    auto w = req.add_ws();
    w->set_key("hi");
    w->set_value("bye");

    c.DoPrepare(req, &res);

    ASSERT_TRUE(res.outcome()) << "Outcome should be good ";

    Decide d;
    Committed cm;

    d.set_txid(1);
    d.set_nodeid(0);
    d.set_outcome(true);
    d.set_fsn(res.sn());

    c.DoDecide(d, &cm);

    ASSERT_TRUE(cm.success()) << "Should commit";
}

TEST(CControlTest, TxAPIStartTest) {
    spdlog::set_level(spdlog::level::trace);

    std::string addr = "127.0.0.1:8080";

    CControlContextPair pair(0, 1);

    auto server = RunServer(addr, pair.cc);

    auto f = std::async([&]() {
        server->Wait();
    });

    std::vector<std::shared_ptr<InternalClient>> servers;
    servers.push_back(std::make_shared<InternalClient>(addr));

    TXAPI txapi(pair.ctx, servers);
    Empty e;
    TxIDMsg res;
    txapi.StartTx(e, &res);
    auto a = res.txid();
    txapi.StartTx(e, &res);
    ASSERT_TRUE(res.txid() == a + 1) << "Should increment txid";

    std::cerr << "Done\n";
    server->Shutdown();
    f.get();
}

TEST(CControlTest, TxAPIReadWhatYouWriteTest) {
    spdlog::set_level(spdlog::level::trace);

    std::string addr = "127.0.0.1:8080";

    CControlContextPair pair(0, 1);

    auto server = RunServer(addr, pair.cc);

    auto f = std::async([&]() {
        server->Wait();
    });
    std::vector<std::shared_ptr<InternalClient>> servers;
    servers.push_back(std::make_shared<InternalClient>(addr));
    TXAPI txapi(pair.ctx, servers);
    Empty e;
    TxIDMsg res;
    txapi.StartTx(e, &res);
    auto txid = res.txid();
    auto node = res.nodeid();

    WriteOperation w;
    w.set_key("hi");
    w.set_value("test");
    w.set_txid(txid);
    w.set_nodeid(node);
    WriteOperationResponse res2;
    txapi.Write(w, &res2);
    ASSERT_TRUE(res2.txid() == txid);
    ReadOperation r;
    r.set_key("hi");
    r.set_txid(txid);
    ReadOperationResponse res3;
    txapi.Read(r, &res3);
    ASSERT_TRUE(res3.value() == "test") << "Should read own writes";
    std::cerr << "Done\n";
    server->Shutdown();
    f.get();
}

TEST(CControlTest, TxAPIShouldCommit) {
    spdlog::set_level(spdlog::level::trace);

    std::string addr = "127.0.0.1:8080";

    CControlContextPair pair(0, 1);

    auto server = RunServer(addr, pair.cc);

    auto f = std::async([&]() {
        server->Wait();
    });
    std::vector<std::shared_ptr<InternalClient>> servers;
    servers.push_back(std::make_shared<InternalClient>(addr));
    TXAPI txapi(pair.ctx, servers);
    Empty e;
    TxIDMsg res;
    txapi.StartTx(e, &res);
    auto txid = res.txid();
    auto node = res.nodeid();

    WriteOperation w;
    w.set_key("hi");
    w.set_value("test");
    w.set_txid(txid);
    w.set_nodeid(node);
    WriteOperationResponse res2;
    txapi.Write(w, &res2);
    ASSERT_TRUE(res2.txid() == txid);
    ReadOperation r;
    r.set_key("hi");
    r.set_txid(txid);
    r.set_nodeid(node);
    ReadOperationResponse res3;
    txapi.Read(r, &res3);
    ASSERT_TRUE(res3.value() == "test") << "Should read own writes";
    Committed committed;
    txapi.Commit(res, &committed);
    ASSERT_TRUE(committed.success()) << "Should succeed\n";
    std::cerr << "Done\n";
    server->Shutdown();
    f.get();
}

TEST(CControlTest, TxAPIShouldCommitAllReads) {
    spdlog::set_level(spdlog::level::trace);

    std::string addr = "127.0.0.1:8080";

    CControlContextPair pair(0, 1);

    auto server = RunServer(addr, pair.cc);

    auto f = std::async([&]() {
        server->Wait();
    });
    std::vector<std::shared_ptr<InternalClient>> servers;
    servers.push_back(std::make_shared<InternalClient>(addr));
    TXAPI txapi(pair.ctx, servers);
    Empty e;
    TxIDMsg res;
    txapi.StartTx(e, &res);
    auto txid = res.txid();
    auto node = res.nodeid();

    ReadOperation r;
    r.set_key("hi");
    r.set_txid(txid);
    r.set_nodeid(node);

    ReadOperationResponse res2;
    txapi.Read(r, &res2);
    ASSERT_TRUE(res2.txid() == txid);
    Committed committed;
    res.set_txid(txid);
    res.set_nodeid(node);

    txapi.Commit(res, &committed);
    ASSERT_TRUE(committed.success()) << "Should succeed\n";
    std::cerr << "Done\n";
    server->Shutdown();
    f.get();
}

TEST(CControlTest, TxClientAPIShouldCommitAllReads) {
    spdlog::set_level(spdlog::level::trace);

    std::string internalAddr = "127.0.0.1:8080";
    std::string clientAddr = "127.0.0.1:8081";

    CControlContextPair pair(0, 1);

    auto server = RunServer(internalAddr, pair.cc);

    auto f = std::async([&]() {
        server->Wait();
    });

    std::vector<std::shared_ptr<InternalClient>> servers;
    servers.push_back(std::make_shared<InternalClient>(internalAddr));

    auto server2 = RunClientSideServer(clientAddr, pair.ctx, servers);

    auto f2 = std::async([&]() {
        server2->Wait();
    });

    TxClient client(clientAddr);

    auto tx = client.StartTx();

    ASSERT_TRUE(tx.Read("Hi").first);

    ASSERT_TRUE(tx.Read("Hi2").first);

    ASSERT_TRUE(tx.TryCommit()) << "Should succeed\n";
    std::cerr << "Done\n";

    server->Shutdown();
    server2->Shutdown();
    f.get();
    f2.get();
}

TEST(CControlTest, TxClientAPIShouldCommitAllReadsServer) {
    spdlog::set_level(spdlog::level::trace);

    std::vector<std::string> internalAddr = {"127.0.0.1:8080"};
    std::string clientAddr = "127.0.0.1:8081";

    Server s(0, internalAddr, clientAddr);

    TxClient client(clientAddr);

    auto tx = client.StartTx();

    ASSERT_TRUE(tx.Read("Hi").first);

    ASSERT_TRUE(tx.Read("Hi2").first);

    ASSERT_TRUE(tx.TryCommit()) << "Should succeed\n";
    std::cerr << "Done\n";

    s.stop();
}

TEST(CControlTest, TxClientAPIShouldCommitAllReads2Server) {
    spdlog::set_level(spdlog::level::trace);

    std::vector<std::string> internalAddr = {"127.0.0.1:8080", "127.0.0.1:8081"};

    Server s1(0, internalAddr, "127.0.0.1:8082");
    Server s2(1, internalAddr, "127.0.0.1:8083");

    TxClient client("127.0.0.1:8082");

    auto tx = client.StartTx();

    ASSERT_TRUE(tx.Read("Hi").first);

    ASSERT_TRUE(tx.Read("Hi2").first);

    ASSERT_TRUE(tx.TryCommit()) << "Should succeed\n";
    std::cerr << "Done\n";

    s1.stop();
    s2.stop();
}

TEST(CControlTest, TxClientAPIReadLastTx2Servers) {
    spdlog::set_level(spdlog::level::trace);

    std::vector<std::string> internalAddr = {"127.0.0.1:8080", "127.0.0.1:8081"};

    Server s1(0, internalAddr, "127.0.0.1:8082");
    Server s2(1, internalAddr, "127.0.0.1:8083");

    TxClient client("127.0.0.1:8082");

    auto tx = client.StartTx();

    ASSERT_TRUE(tx.Write("Hi", "bye"));

    ASSERT_TRUE(tx.TryCommit()) << "Should succeed\n";
    std::cerr << "Done\n";

    auto tx2 = client.StartTx();
    auto res = tx.Read("Hi");
    ASSERT_TRUE(res.first);

    ASSERT_TRUE(res.second == "bye");

    ASSERT_TRUE(tx2.TryCommit()) << "Should succeed\n";

    s1.stop();
    s2.stop();
}
