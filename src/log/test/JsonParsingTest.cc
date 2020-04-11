#include "../../common/base/Base.h"
#include <gtest/gtest.h>
#include "../JsonParsing.h"

namespace niok {
namespace log {

TEST(JsonParsingTest, SimpleTest) {
    JsonParsing *jp = new JsonParsing("exampleLog.json");

    // add 5 keys and values to the log
    for (int i = 0; i < 5; i++) {
        std::string key = "Timestamp" + std::to_string(i);
        std::string value = "Value" + std::to_string(i);
        jp->JsonParsing::addLogToFile(key, value);
    }
    std::cout << jp->JsonParsing::getValueFromKey("Timestamp0") << std::endl;
    EXPECT_EQ(jp->JsonParsing::getValueFromKey("Timestamp0"), "Value0");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey("Timestamp1"), "Value1");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey("Timestamp2"), "Value2");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey("Timestamp3"), "Value3");
    EXPECT_EQ(jp->JsonParsing::getValueFromKey("Timestamp4"), "Value4");
}

}  // namespace kvstore
}  // namespace log

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}