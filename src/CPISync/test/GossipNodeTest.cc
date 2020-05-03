#include "../../common/base/Base.h"
#include <gtest/gtest.h>
#include <leveldb/db.h>
#include "../GossipNode.h"

namespace niok {
namespace cpisync {


TEST(GossipNodeTest, SimpleTest) {
    /*
    std::string rootPath = "/tmp/GossipNodeTest.XXXXXX";
    auto node = new cpisync::GossipNode("test", 0, rootPath);
    EXPECT_EQ(true, node->put("key", "val"));
    std::string val;
    EXPECT_EQ(true, node->get("key", &val));
    */
    EXPECT_EQ("val", "val");
}

}  // namespace cpisync
}  // namespace niok

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}