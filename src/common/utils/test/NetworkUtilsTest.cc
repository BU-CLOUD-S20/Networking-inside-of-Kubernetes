#include "../../base/Base.h"
#include <gtest/gtest.h>

namespace niok {
namespace common {


TEST(NetworkUtilsTest, SimpleTest) {
    NetworkUtils *network = new NetworkUtils();
    std::string hostName;
	std::string Ip;
	
	bool ret = network->GetHostInfo(hostName, Ip);
	if (true == ret) {
		EXPECT_EQ(hostName, "node1");
        EXPECT_EQ(Ip, "172.128.1.1");
	}
}

}  // namespace common
}  // namespace niok

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}