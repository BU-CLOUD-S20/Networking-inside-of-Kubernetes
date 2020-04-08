#include "../../common/base/Base.h"
#include <gtest/gtest.h>
#include <leveldb/db.h>
#include "../LevelEngine.h"
#include "../KVEngine.h"
#include "../Common.h"

namespace niok {
namespace kvstore {

TEST(LevelEngineTest, SimpleTest) {
    std::string rootPath = "/tmp/leveldb_engine_SimpleTest.XXXXXX";
    auto engine = std::make_unique<LevelEngine>(0, rootPath);
    EXPECT_EQ(kvstore::ResultCode::SUCCEEDED, engine->put("key", "val"));
    std::string val;
    EXPECT_EQ(kvstore::ResultCode::SUCCEEDED, engine->get("key", &val));
    EXPECT_EQ("val", val);
}

TEST(LevelEngineTest, RemoveTest) {
    std::string rootPath = "/tmp/rocksdb_engine_RemoveTest.XXXXXX";
    auto engine = std::make_unique<LevelEngine>(0, rootPath);
    EXPECT_EQ(kvstore::ResultCode::SUCCEEDED, engine->put("key", "val"));
    std::string val;
    EXPECT_EQ(kvstore::ResultCode::SUCCEEDED, engine->get("key", &val));
    EXPECT_EQ("val", val);
    EXPECT_EQ(kvstore::ResultCode::SUCCEEDED, engine->remove("key"));
    EXPECT_EQ(kvstore::ResultCode::ERR_KEY_NOT_FOUND, engine->get("key", &val));
}

}  // namespace kvstore
}  // namespace niok

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}