#include <gtest/gtest.h>
#include "sinks/ConsoleSinkImpl.h"
#include "sinks/FileSinkImpl.h"
#include "LogMessage.h"
#include <sstream>
#include <fstream>
#include <filesystem>


class ConsoleSinkTests : public ::testing::Test {
protected:
    ConsoleSinkImpl sink;
    
    // Redirect cout for testing
    std::stringstream captured_output;
    std::streambuf* old_cout;
    
    void SetUp() override {
        old_cout = std::cout.rdbuf(captured_output.rdbuf());
    }
    
    void TearDown() override {
        std::cout.rdbuf(old_cout);
    }
};


TEST_F(ConsoleSinkTests, Write_ShouldOutputMessage) {
    LogMessage msg("TestApp", "TestContext", "2024-01-01 12:00:00", SeverityLvl::INFO, "Test message");
    sink.write(msg);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("TestApp"), std::string::npos);
    EXPECT_NE(output.find("TestContext"), std::string::npos);
    EXPECT_NE(output.find("Test message"), std::string::npos);
}


TEST_F(ConsoleSinkTests, Write_ShouldIncludeTimestamp) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::WARNING, "message");
    sink.write(msg);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("2024-01-01 12:00:00"), std::string::npos);
}


TEST_F(ConsoleSinkTests, Write_ShouldIncludeSeverityLevel) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::CRITICAL, "message");
    sink.write(msg);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("CRITICAL"), std::string::npos);
}


TEST_F(ConsoleSinkTests, Write_INFO_Level) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    sink.write(msg);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("INFO"), std::string::npos);
}


TEST_F(ConsoleSinkTests, Write_WARNING_Level) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::WARNING, "test");
    sink.write(msg);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("WARNING"), std::string::npos);
}


TEST_F(ConsoleSinkTests, Write_MultipleMessages) {
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    
    sink.write(msg1);
    sink.write(msg2);
    
    std::string output = captured_output.str();
    EXPECT_NE(output.find("msg1"), std::string::npos);
    EXPECT_NE(output.find("msg2"), std::string::npos);
}


class FileSinkTests : public ::testing::Test {
protected:
    std::string test_file = "/tmp/test_log_file.txt";
    
    void TearDown() override {
        // Clean up test file
        std::filesystem::remove(test_file);
    }
};


TEST_F(FileSinkTests, Write_ShouldCreateFile) {
    FileSinkImpl sink(test_file);
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "test");
    sink.write(msg);
    
    EXPECT_TRUE(std::filesystem::exists(test_file));
}


TEST_F(FileSinkTests, Write_ShouldWriteMessageToFile) {
    FileSinkImpl sink(test_file);
    LogMessage msg("TestApp", "TestContext", "2024-01-01 12:00:00", SeverityLvl::INFO, "Test message");
    sink.write(msg);
    
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_NE(content.find("TestApp"), std::string::npos);
    EXPECT_NE(content.find("TestContext"), std::string::npos);
    EXPECT_NE(content.find("Test message"), std::string::npos);
}


TEST_F(FileSinkTests, Write_ShouldIncludeTimestamp) {
    FileSinkImpl sink(test_file);
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::WARNING, "message");
    sink.write(msg);
    
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_NE(content.find("2024-01-01 12:00:00"), std::string::npos);
}


TEST_F(FileSinkTests, Write_MultipleMessages_ShouldAppend) {
    FileSinkImpl sink(test_file);
    LogMessage msg1("app1", "ctx1", "2024-01-01 12:00:00", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "2024-01-01 12:01:00", SeverityLvl::WARNING, "msg2");
    
    sink.write(msg1);
    sink.write(msg2);
    
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_NE(content.find("msg1"), std::string::npos);
    EXPECT_NE(content.find("msg2"), std::string::npos);
}


TEST_F(FileSinkTests, Write_CRITICAL_Level) {
    FileSinkImpl sink(test_file);
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::CRITICAL, "critical error");
    sink.write(msg);
    
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_NE(content.find("CRITICAL"), std::string::npos);
    EXPECT_NE(content.find("critical error"), std::string::npos);
}


TEST_F(FileSinkTests, Write_EmptyMessage) {
    FileSinkImpl sink(test_file);
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "");
    sink.write(msg);
    
    EXPECT_TRUE(std::filesystem::exists(test_file));
}


TEST_F(FileSinkTests, Write_LongMessage) {
    FileSinkImpl sink(test_file);
    std::string long_text(1000, 'a');
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, long_text);
    sink.write(msg);
    
    std::ifstream file(test_file);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_NE(content.find(long_text), std::string::npos); // Message should be found in file
    EXPECT_NE(content.length(), 0);
}
