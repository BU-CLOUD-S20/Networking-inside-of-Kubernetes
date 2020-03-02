#include "../common/base/Base.h"
#include "LevelEngine.h"
#include "Common.h"
#include <leveldb/db.h>
#include <leveldb/write_batch.h>

using std::cout;
using std::endl;
namespace niok {
namespace kvstore {

namespace {

class LevelWriteBatch : public WriteBatch {
private:
    leveldb::WriteBatch batch_;
    leveldb::DB* db_{nullptr};

public:
    explicit LevelWriteBatch(leveldb::DB* db) : batch_(), db_(db) {}

    virtual ~LevelWriteBatch() = default;

    void put(std::string key, std::string value) override {
        batch_.Put(kvstore::toSlice(key), kvstore::toSlice(value));
    }

    void remove(std::string key) override {
        batch_.Delete(kvstore::toSlice(key));
    }

    void clear() {
        batch_.Clear();
    }

    leveldb::WriteBatch* data() {
        return &batch_;
    }
};

} // Anonymous namespace

LevelEngine::LevelEngine(int spaceId,
                         const std::string& dataPath)
        : KVEngine(spaceId)
        , dataPath_(dataPath + std::to_string(spaceId)) {
    leveldb::Options options;
    leveldb::DB* db = nullptr;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, dataPath_, &db);
    if (!status.ok()) {
        std::cout << status.ToString() << std::endl;
    } 
    db_.reset(db);
}

std::unique_ptr<WriteBatch> LevelEngine::startBatchWrite() {
    return std::make_unique<LevelWriteBatch>(db_.get());
}

kvstore::ResultCode LevelEngine::commitBatchWrite(std::unique_ptr<WriteBatch> batch) {
    leveldb::WriteOptions options;
    auto* b = static_cast<LevelWriteBatch*>(batch.get());
    leveldb::Status status = db_->Write(options, b->data());
    if (status.ok()) {
        return kvstore::ResultCode::SUCCEEDED;
    }
    return kvstore::ResultCode::ERR_UNKNOWN;
}

kvstore::ResultCode LevelEngine::get(const std::string& key, std::string* value) {
    leveldb::ReadOptions options;
    leveldb::Status status = db_->Get(options, leveldb::Slice(key), value);
    if (status.ok()) {
        return kvstore::ResultCode::SUCCEEDED;
    } else if (status.IsNotFound()) {
        std::cout << "Get: " + key + " Not Found" << std::endl;
        return kvstore::ResultCode::ERR_KEY_NOT_FOUND;
    } else {
        std::cout << "Get Failed: " + key + " " + status.ToString() << std::endl;
        return kvstore::ResultCode::ERR_UNKNOWN;
    }
}

kvstore::ResultCode LevelEngine::put(std::string key, std::string value) {
    leveldb::WriteOptions options;
    leveldb::Status status = db_->Put(options, key, value);
    if (status.ok()) {
        return kvstore::ResultCode::SUCCEEDED;
    } else {
        std::cout << "Put Failed: " + key + status.ToString() << std::endl;
        return kvstore::ResultCode::ERR_UNKNOWN;
    }
}

kvstore::ResultCode LevelEngine::remove(const std::string& key) {
    leveldb::WriteOptions options;
    auto status = db_->Delete(options, key);
    if (status.ok()) {
        return kvstore::ResultCode::SUCCEEDED;
    } else {
        std::cout << "Remove Failed: " << key << status.ToString() << std::endl;
        return kvstore::ResultCode::ERR_UNKNOWN;
    }
}

} // kvstore
} // niok
