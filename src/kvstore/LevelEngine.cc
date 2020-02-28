#include "base/Base.h"
#include "kvstore/LevelEngine.h"

namespace niok {
namespace kvstore {

namespace {
class LevelWriteBatch : public WriteBatch {
private:
    leveldb::WriteBatch batch_;
    leveldb::DB* db_{nullptr};

public:
    explicit LevelWriteBatch(leveldb::DB* db) : batch_(FLAGS_rocksdb_batch_size), db_(db) {}

    virtual ~RocksWriteBatch() = default;

    ResultCode put(string key, string value) override {
        if (batch_.Put(toSlice(key), toSlice(value)).ok()) {
            return ResultCode::SUCCEEDED;
        } else {
            return ResultCode::ERR_UNKNOWN;
        }
    }

    leveldb::WriteBatch* data() {
        return &batch_;
    }
};
} // Anonymous namespace

LevelEngine::LevelEngine(SpaceID spaceId,
                         const std::string& dataPath)
        : KVEngine(spaceId)
        , dataPath_(dataPath + "/niok/" + to_string(spaceId)) {
    auto path = dataPath_.c_str() + "/data";
    /*
    if (FileUtils::fileType(path.c_str()) == FileType::NOTEXIST) {
        FileUtils::makeDir(path);
    }
    LOG(INFO) << "open rocksdb on " << path;
    */
    leveldb::Options options;
    leveldb::DB* db = nullptr;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    if (!status.ok()) std::cout << status.ToString() << std::endl;
}

ResultCode LevelEngine::get(const std::string& key, std::string* value) {
    leveldb::ReadOptions options;
    leveldb::Status status = db_->Get(options, leveldb::Slice(key), value);
    if (status.ok()) {
        return ResultCode::SUCCEEDED;
    } else if (status.IsNotFound()) {
        VLOG(3) << "Get: " + key + " Not Found";
        return ResultCode::ERR_KEY_NOT_FOUND;
    } else {
        VLOG(3) << "Get Failed: " + key + " " + status.ToString();
        return ResultCode::ERR_UNKNOWN;
    }
}

ResultCode LevelEngine::put(std::string key, std::string value) {
    leveldb::WriteOptions options;
    leveldb::Status status = db_->Put(options, key, value);
    if (status.ok()) {
        return ResultCode::SUCCEEDED;
    } else {
        VLOG(3) << "Put Failed: " + key + status.ToString();
        return ResultCode::ERR_UNKNOWN;
    }
}