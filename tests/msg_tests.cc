#include <gtest/gtest.h>
#include "LogManager.h"
#include "LogMessage.h"

class MSG_Test : public ::testing::Test
{
    protected:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
};


TEST_F(MSG_Test, BasicMSGTest_SHOULD_PASS)
{
    auto msg = LogMessage("app1", "context1", "2024-01-01 12:00:00", SeverityLvl::INFO, "This is a test log message.");
    EXPECT_EQ(msg.app_name, "app1");
    EXPECT_EQ(msg.context, "context1");
    EXPECT_EQ(msg.time, "2024-01-01 12:00:00");
    EXPECT_EQ(msg.level, SeverityLvl::INFO);
    EXPECT_EQ(msg.text, "This is a test log message.");
}