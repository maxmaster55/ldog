#include <gtest/gtest.h>
#include "LogManager.h"
#include "LogMessage.h"
#include "sinks/ConsoleSinkImpl.h"
#include <memory>
#include <sstream>
#include <iostream>


// Mock sink for testing
class MockSink : public ILogSink {
public:
    std::vector<LogMessage> received_messages;
    
    void write(const LogMessage& msg) override {
        received_messages.push_back(msg);
    }
};


class LogManagerTests : public ::testing::Test {
protected:
    LogManager manager = LogManager(10);
    
    void SetUp() override {
        // Each test gets a fresh LogManager
    }
    
    void TearDown() override {
        // Cleanup after each test
    }
};


TEST_F(LogManagerTests, AddSingleMessage_ShouldStoreMessage) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test message");
    manager.add_msg(msg);
    
    // Message should be stored internally
    // We verify this by writing to a sink
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 1);
    EXPECT_EQ(sink_ptr->received_messages[0].app_name, "app");
    EXPECT_EQ(sink_ptr->received_messages[0].text, "test message");
}


TEST_F(LogManagerTests, AddMultipleMessages_ShouldStoreAll) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    LogMessage msg3("app3", "ctx3", "2024-01-01 12:02:00", SeverityLvl::CRITICAL, "msg3");
    
    manager.add_msg(msg1);
    manager.add_msg(msg2);
    manager.add_msg(msg3);
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 3);
    EXPECT_EQ(sink_ptr->received_messages[0].app_name, "app1");
    EXPECT_EQ(sink_ptr->received_messages[1].app_name, "app2");
    EXPECT_EQ(sink_ptr->received_messages[2].app_name, "app3");
}


TEST_F(LogManagerTests, OperatorLeftShift_ShouldAddMessage) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    manager << msg;
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 1);
    EXPECT_EQ(sink_ptr->received_messages[0].app_name, "app");
}


TEST_F(LogManagerTests, OperatorLeftShift_ShouldReturnLogManagerReference) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    
    // Should support chaining
    manager << msg1 << msg2;
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 2);
}


TEST_F(LogManagerTests, AddMultipleSinks_ShouldRouteToAll) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    manager.add_msg(msg);
    
    auto sink1 = std::make_unique<MockSink>();
    auto sink2 = std::make_unique<MockSink>();
    auto sink3 = std::make_unique<MockSink>();
    
    auto sink1_ptr = static_cast<MockSink*>(sink1.get());
    auto sink2_ptr = static_cast<MockSink*>(sink2.get());
    auto sink3_ptr = static_cast<MockSink*>(sink3.get());
    
    manager.add_sink(std::move(sink1));
    manager.add_sink(std::move(sink2));
    manager.add_sink(std::move(sink3));
    manager.write();
    
    EXPECT_EQ(sink1_ptr->received_messages.size(), 1);
    EXPECT_EQ(sink2_ptr->received_messages.size(), 1);
    EXPECT_EQ(sink3_ptr->received_messages.size(), 1);
}


TEST_F(LogManagerTests, Write_ShouldClearBuffer) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    
    manager.add_msg(msg1);
    manager.add_msg(msg2);
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    
    manager.write();
    EXPECT_EQ(sink_ptr->received_messages.size(), 2);
    
    // Clear the mock sink's messages to verify behavior
    sink_ptr->received_messages.clear();
    
    // Write again without adding new messages - should be empty
    manager.write();
    EXPECT_EQ(sink_ptr->received_messages.size(), 0);
}


TEST_F(LogManagerTests, EmptyManager_ShouldWriteNothing) {
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 0);
}


TEST_F(LogManagerTests, MultipleWriteCycles) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    manager.add_msg(msg1);
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    
    // First write
    manager.write();
    EXPECT_EQ(sink_ptr->received_messages.size(), 1);
    
    // Add more messages
    sink_ptr->received_messages.clear();
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    manager.add_msg(msg2);
    
    // Second write
    manager.write();
    EXPECT_EQ(sink_ptr->received_messages.size(), 1);
    EXPECT_EQ(sink_ptr->received_messages[0].app_name, "app2");
}


// LogManagerBuilder Tests
class LogManagerBuilderTests : public ::testing::Test {
protected:
    LogManagerBuilder builder = LogManagerBuilder(10);
};


TEST_F(LogManagerBuilderTests, BuilderPattern_ShouldBuild) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    
    LogManager manager = builder
        .add_msg(msg)
        .build();
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 1);
}


TEST_F(LogManagerBuilderTests, BuilderWithMultipleSinks_ShouldBuild) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    
    LogManager manager = builder
        .add_msg(msg)
        .add_sink(std::make_unique<MockSink>())
        .add_sink(std::make_unique<MockSink>())
        .build();
    
    // Just verify it builds without crashing
    EXPECT_NO_THROW(manager.write());
}


TEST_F(LogManagerBuilderTests, BuilderChaining_ShouldReturnReference) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    
    LogManager manager = builder
        .add_msg(msg1)
        .add_msg(msg2)
        .add_sink(std::make_unique<MockSink>())
        .build();
    
    auto mock_sink = std::make_unique<MockSink>();
    auto sink_ptr = static_cast<MockSink*>(mock_sink.get());
    manager.add_sink(std::move(mock_sink));
    manager.write();
    
    EXPECT_EQ(sink_ptr->received_messages.size(), 2);
}
