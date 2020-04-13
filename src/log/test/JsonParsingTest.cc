#include "../../common/base/Base.h"
#include <gtest/gtest.h>
#include "../JsonParsing.h"

namespace niok {
namespace log {

TEST(JsonParsingTest, SimpleTest) {
    JsonParsing *jp = new JsonParsing("exampleLog.json");
    std::string timestamps [5]; 

    // add 5 timestamped values to the log
    for (int i = 0; i < 5; i++) {
        std::string value = "Value" + std::to_string(i);
        timestamps[i] = jp->JsonParsing::addLogToFileTimestamp(value);
    }

    EXPECT_EQ(jp->JsonParsing::getValueFromKey(timestamps[0].substr(2, timestamps.size()-4)), "Value0");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey(timestamps[1].substr(2, timestamps.size()-4)), "Value1");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey(timestamps[2].substr(2, timestamps.size()-4)), "Value2");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey(timestamps[3].substr(2, timestamps.size()-4)), "Value3");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey(timestamps[4].substr(2, timestamps.size()-4)), "Value4");
}

}  // namespace kvstore
}  // namespace log

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}