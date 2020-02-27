#include "base/Base.h"
#include "kvstore/LevelEngine.h"

namespace niok {
namespace kvstore {

class RocksWriteBatch : public WriteBatch {
private:
    rocksdb::WriteBatch batch_;
    rocksdb::DB* db_{nullptr};

public:
    explicit RocksWriteBatch(rocksdb::DB* db) : batch_(FLAGS_rocksdb_batch_size), db_(db) {}

    virtual ~RocksWriteBatch() = default;

    ResultCode put(folly::StringPiece key, folly::StringPiece value) override {
        if (batch_.Put(toSlice(key), toSlice(value)).ok()) {
            return ResultCode::SUCCEEDED;
        } else {
            return ResultCode::ERR_UNKNOWN;
        }
    }

    ResultCode remove(folly::StringPiece key) override {
        if (batch_.Delete(toSlice(key)).ok()) {
            return ResultCode::SUCCEEDED;
        } else {
            return ResultCode::ERR_UNKNOWN;
        }
    }

    ResultCode removePrefix(folly::StringPiece prefix) override {
        rocksdb::Slice pre(prefix.begin(), prefix.size());
        rocksdb::ReadOptions options;
        std::unique_ptr<rocksdb::Iterator> iter(db_->NewIterator(options));
        iter->Seek(pre);
        while (iter->Valid()) {
            if (iter->key().starts_with(pre)) {
                if (!batch_.Delete(iter->key()).ok()) {
                    return ResultCode::ERR_UNKNOWN;
                }
            } else {
                // Done
                break;
            }
            iter->Next();
        }
        return ResultCode::SUCCEEDED;
    }

    // Remove all keys in the range [start, end)
    ResultCode removeRange(folly::StringPiece start, folly::StringPiece end) override {
        if (batch_.DeleteRange(toSlice(start), toSlice(end)).ok()) {
            return ResultCode::SUCCEEDED;
        } else {
            return ResultCode::ERR_UNKNOWN;
        }
    }

    rocksdb::WriteBatch* data() {
        return &batch_;
    }
};