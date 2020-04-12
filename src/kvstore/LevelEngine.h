#ifndef KVSTORE_LEVELENGINE_H_
#define KVSTORE_LEVELENGINE_H_

#include <leveldb/db.h>
#include "../common/base/Base.h"
#include "KVEngine.h"


namespace niok {
namespace kvstore {

class LevelEngine : public KVEngine {
public:
    LevelEngine(int spaceId,
                const std::string& dataPath);

    ~LevelEngine() {
        std::cout << "Release leveldb on " << dataPath_ << std::endl;
    }

    std::unique_ptr<WriteBatch> startBatchWrite() override;

    kvstore::ResultCode commitBatchWrite(std::unique_ptr<WriteBatch> batch) override;

    kvstore::ResultCode get(const std::string& key, std::string* value) override;

    kvstore::ResultCode put(std::string key, std::string value) override;

    ResultCode remove(const std::string& key) override;

private:
    std::string  dataPath_;
    std::unique_ptr<leveldb::DB> db_{nullptr};
};
} // namespace kvstore    
} // namespace niok

#endif