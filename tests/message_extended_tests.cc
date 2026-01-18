#include <gtest/gtest.h>
#include "LogMessage.h"
#include <sstream>


class LogMessageTests : public ::testing::Test {
protected:
    // Base class for LogMessage tests
};


TEST(LogMessage_Construction, DefaultConstructor_WithAllParameters) {
    LogMessage msg("MyApp", "MyContext", "2024-01-15 10:30:45", SeverityLvl::WARNING, "This is a warning");
    
    EXPECT_EQ(msg.app_name, "MyApp");
    EXPECT_EQ(msg.context, "MyContext");
    EXPECT_EQ(msg.time, "2024-01-15 10:30:45");
    EXPECT_EQ(msg.level, SeverityLvl::WARNING);
    EXPECT_EQ(msg.text, "This is a warning");
}


TEST(LogMessage_Construction, EmptyStrings) {
    LogMessage msg("", "", "", SeverityLvl::INFO, "");
    
    EXPECT_EQ(msg.app_name, "");
    EXPECT_EQ(msg.context, "");
    EXPECT_EQ(msg.time, "");
    EXPECT_EQ(msg.text, "");
}


TEST(LogMessage_Construction, SeverityLevels_INFO) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "message");
    EXPECT_EQ(msg.level, SeverityLvl::INFO);
}


TEST(LogMessage_Construction, SeverityLevels_WARNING) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::WARNING, "message");
    EXPECT_EQ(msg.level, SeverityLvl::WARNING);
}


TEST(LogMessage_Construction, SeverityLevels_CRITICAL) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::CRITICAL, "message");
    EXPECT_EQ(msg.level, SeverityLvl::CRITICAL);
}


TEST(LogMessage_Construction, LongAppName) {
    std::string long_name(500, 'a');
    LogMessage msg(long_name, "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "message");
    EXPECT_EQ(msg.app_name, long_name);
}


TEST(LogMessage_Construction, LongContextName) {
    std::string long_context(500, 'b');
    LogMessage msg("app", long_context, "2024-01-01 12:00:00", SeverityLvl::INFO, "message");
    EXPECT_EQ(msg.context, long_context);
}


TEST(LogMessage_Construction, LongMessageText) {
    std::string long_text(2000, 'c');
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, long_text);
    EXPECT_EQ(msg.text, long_text);
}


TEST(LogMessage_Construction, SpecialCharactersInText) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, 
                   "Special chars: !@#$%^&*(){}[]|\\:;\"'<>,.?/");
    EXPECT_NE(msg.text.find("!@#"), std::string::npos);
}


TEST(LogMessage_Construction, TimeFormatVariations) {
    // ISO 8601 format
    LogMessage msg1("app", "context", "2024-01-15T10:30:45Z", SeverityLvl::INFO, "test");
    EXPECT_EQ(msg1.time, "2024-01-15T10:30:45Z");
    
    // Another format
    LogMessage msg2("app", "context", "01/15/2024 10:30:45", SeverityLvl::INFO, "test");
    EXPECT_EQ(msg2.time, "01/15/2024 10:30:45");
}


TEST(LogMessage_Construction, NewlinesInText) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "line1\nline2\nline3");
    EXPECT_NE(msg.text.find("\n"), std::string::npos);
}


TEST(LogMessage_Construction, TabsAndWhitespace) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, 
                   "text\twith\ttabs and   spaces");
    EXPECT_NE(msg.text.find("\t"), std::string::npos);
}


TEST(LogMessage_Construction, UnicodeCharacters) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::INFO, "Unicode: émojis ✨ 中文");
    EXPECT_NE(msg.text.find("✨"), std::string::npos);
}


TEST(LogMessage_OstreamOperator, OutputFormat) {
    LogMessage msg("MyApp", "MyContext", "2024-01-01 12:00:00", SeverityLvl::INFO, "Test message");
    
    std::stringstream ss;
    ss << msg;
    std::string output = ss.str();
    
    EXPECT_NE(output.find("MyApp"), std::string::npos);
    EXPECT_NE(output.find("MyContext"), std::string::npos);
    EXPECT_NE(output.find("2024-01-01 12:00:00"), std::string::npos);
    EXPECT_NE(output.find("INFO"), std::string::npos);
    EXPECT_NE(output.find("Test message"), std::string::npos);
}


TEST(LogMessage_OstreamOperator, ContainsBrackets) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::WARNING, "message");
    
    std::stringstream ss;
    ss << msg;
    std::string output = ss.str();
    
    // Check for bracket formatting
    EXPECT_GE(std::count(output.begin(), output.end(), '['), 5);
    EXPECT_GE(std::count(output.begin(), output.end(), ']'), 5);
}


TEST(LogMessage_OstreamOperator, EndsWithNewline) {
    LogMessage msg("app", "context", "2024-01-01 12:00:00", SeverityLvl::CRITICAL, "message");
    
    std::stringstream ss;
    ss << msg;
    std::string output = ss.str();
    
    EXPECT_EQ(output.back(), '\n');
}


TEST(LogMessage_OstreamOperator, CorrectFieldOrder) {
    LogMessage msg("app1", "ctx1", "time1", SeverityLvl::INFO, "txt1");
    
    std::stringstream ss;
    ss << msg;
    std::string output = ss.str();
    
    size_t app_pos = output.find("app1");
    size_t time_pos = output.find("time1");
    size_t ctx_pos = output.find("ctx1");
    size_t level_pos = output.find("INFO");
    size_t text_pos = output.find("txt1");
    
    // Verify order: app -> time -> context -> level -> text
    EXPECT_LT(app_pos, time_pos);
    EXPECT_LT(time_pos, ctx_pos);
    EXPECT_LT(ctx_pos, level_pos);
    EXPECT_LT(level_pos, text_pos);
}


TEST(LogMessage_OstreamOperator, AllSeverityLevels) {
    // Test INFO
    {
        LogMessage msg("app", "ctx", "time", SeverityLvl::INFO, "msg");
        std::stringstream ss;
        ss << msg;
        EXPECT_NE(ss.str().find("INFO"), std::string::npos);
    }
    
    // Test WARNING
    {
        LogMessage msg("app", "ctx", "time", SeverityLvl::WARNING, "msg");
        std::stringstream ss;
        ss << msg;
        EXPECT_NE(ss.str().find("WARNING"), std::string::npos);
    }
    
    // Test CRITICAL
    {
        LogMessage msg("app", "ctx", "time", SeverityLvl::CRITICAL, "msg");
        std::stringstream ss;
        ss << msg;
        EXPECT_NE(ss.str().find("CRITICAL"), std::string::npos);
    }
}


TEST(LogMessage_Immutability, FieldModificationAfterConstruction) {
    LogMessage msg("app1", "ctx1", "time1", SeverityLvl::INFO, "msg1");
    
    // Fields should be public and modifiable (based on the header file)
    msg.app_name = "app2";
    msg.context = "ctx2";
    msg.time = "time2";
    msg.level = SeverityLvl::CRITICAL;
    msg.text = "msg2";
    
    EXPECT_EQ(msg.app_name, "app2");
    EXPECT_EQ(msg.context, "ctx2");
    EXPECT_EQ(msg.time, "time2");
    EXPECT_EQ(msg.level, SeverityLvl::CRITICAL);
    EXPECT_EQ(msg.text, "msg2");
}


TEST(LogMessage_MultipleInstances, IndependentState) {
    LogMessage msg1("app1", "ctx1", "time1", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "time2", SeverityLvl::WARNING, "msg2");
    LogMessage msg3("app3", "ctx3", "time3", SeverityLvl::CRITICAL, "msg3");
    
    EXPECT_NE(msg1.app_name, msg2.app_name);
    EXPECT_NE(msg2.app_name, msg3.app_name);
    EXPECT_NE(msg1.level, msg2.level);
    EXPECT_NE(msg2.level, msg3.level);
}


TEST(LogMessage_OutputComparison, DifferentMessages) {
    LogMessage msg1("app1", "ctx1", "time1", SeverityLvl::INFO, "msg1");
    LogMessage msg2("app2", "ctx2", "time2", SeverityLvl::WARNING, "msg2");
    
    std::stringstream ss1, ss2;
    ss1 << msg1;
    ss2 << msg2;
    
    EXPECT_NE(ss1.str(), ss2.str());
}


TEST(LogMessage_OutputComparison, IdenticalMessages) {
    LogMessage msg1("app", "ctx", "time", SeverityLvl::INFO, "text");
    LogMessage msg2("app", "ctx", "time", SeverityLvl::INFO, "text");
    
    std::stringstream ss1, ss2;
    ss1 << msg1;
    ss2 << msg2;
    
    EXPECT_EQ(ss1.str(), ss2.str());
}
