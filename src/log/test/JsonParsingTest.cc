#include "../../common/base/Base.h"
#include <gtest/gtest.h>
#include "../JsonParsing.cc"

namespace niok {
namespace log {

TEST(JsonParsingTest, SimpleTest) {
    std::string logFile = "exampleLog.json";

    // add 5 keys and values to the log
    for (int i = 0; i < 5; i++) {
        std::string key = "Timestamp" + std::to_string(i);
        std::string value = "Value" + std::to_string(i);
        log::addLogToFile(logFile, key, value);
    }

    EXPECT_EQ(log::getValueFromKey(logFile, "Timestamp0"), "Value0");
    EXPECT_EQ(log::getValueFromKey(logFile, "Timestamp1"), "Value1");
    EXPECT_EQ(log::getValueFromKey(logFile, "Timestamp2"), "Value2");
    EXPECT_EQ(log::getValueFromKey(logFile, "Timestamp3"), "Value3");
    EXPECT_EQ(log::getValueFromKey(logFile, "Timestamp4"), "Value4");
}

}  // namespace kvstore
}  // namespace log

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}