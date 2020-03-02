#ifndef KVSTORE_KVENGINE_H_
#define KVSTORE_KVENGINE_H_

#include "../common/base/Base.h"
#include "Common.h"

namespace niok{
namespace kvstore {

class WriteBatch {
public:
    virtual ~WriteBatch() = default;

    virtual void put(std::string key, std::string value) = 0;

    virtual void remove(std::string key) = 0;

};

class KVEngine {
public:
    explicit KVEngine(int spaceId) : spaceId_(spaceId) {}
    
    virtual ~KVEngine() = default;

    virtual std::unique_ptr<WriteBatch> startBatchWrite() = 0;

    virtual kvstore::ResultCode commitBatchWrite(std::unique_ptr<WriteBatch> batch) = 0;

    // Read a single key
    virtual kvstore::ResultCode get(const std::string& key, std::string* value) = 0;

    // Get all results in range [start, end)
    virtual kvstore::ResultCode put(std::string key, std::string value) = 0;

    // Remove a single key
    virtual ResultCode remove(const std::string& key) = 0;

protected:
    int spaceId_;
};

} // namespace kvstore
} // namespace niok
#endif