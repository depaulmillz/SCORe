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

using namespace score;

TEST(CControlTest, AllocateAndDelete) {
    CControl c(std::make_shared<Context>(0, 1));
}

TEST(CControlTest, TestDoReadRequest) {
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
    CControl c(std::make_shared<Context>(0, 1));
    Prepare req;
    Vote res;
    req.set_txid(1);
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
    d.set_outcome(true);
    d.set_fsn(res.sn());

    c.DoDecide(d, &cm);

    ASSERT_TRUE(cm.success()) << "Should commit";
}

TEST(CControlTest, TxAPIStartTest) {
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
    WriteOperation w;
    w.set_key("hi");
    w.set_value("test");
    w.set_txid(txid);
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
    WriteOperation w;
    w.set_key("hi");
    w.set_value("test");
    w.set_txid(txid);
    WriteOperationResponse res2;
    txapi.Write(w, &res2);
    ASSERT_TRUE(res2.txid() == txid);
    ReadOperation r;
    r.set_key("hi");
    r.set_txid(txid);
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
    ReadOperation r;
    r.set_key("hi");
    r.set_txid(txid);
    ReadOperationResponse res2;
    txapi.Read(r, &res2);
    ASSERT_TRUE(res2.txid() == txid);
    Committed committed;
    res.set_txid(txid);
    txapi.Commit(res, &committed);
    ASSERT_TRUE(committed.success()) << "Should succeed\n";
    std::cerr << "Done\n";
    server->Shutdown();
    f.get();
}