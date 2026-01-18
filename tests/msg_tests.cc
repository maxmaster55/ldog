#include <gtest/gtest.h>
#include "LogManager.h"
#include "LogMessage.h"


TEST(Message_Tests, BasicMSGTest_SHOULD_PASS)
{
    auto msg = LogMessage("app1", "context1", "2024-01-01 12:00:00", SeverityLvl::INFO, "This is a test log message.");
    EXPECT_EQ(msg.app_name, "app1");
    EXPECT_EQ(msg.context, "context1");
    EXPECT_EQ(msg.time, "2024-01-01 12:00:00");
    EXPECT_EQ(msg.level, SeverityLvl::INFO);
    EXPECT_EQ(msg.text, "This is a test log message.");
}

