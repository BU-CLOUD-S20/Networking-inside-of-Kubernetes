#ifndef KVSTORE_KVENGINE_H_
#define KVSTORE_KVENGINE_H_

#include "base/Base.h"
#include "kvstore/Common.h"

namespace niok{
namespace kvstore {

class WriteBatch {
public:
    virtual ~WriteBatch() = default;

    virtual ResultCode put(folly::StringPiece key, folly::StringPiece value) = 0;

};

class KVEngine {
public:
    explicit KVEngine(SpaceID spaceId) : spaceId_(spaceId) {}
    
    virtual ~KVEngine() = default;

    // Read a single key
    virtual ResultCode get(const std::string& key, std::string* value) = 0;
    // Get all results in range [start, end)
    virtual ResultCode put(std::string key, std::string value) = 0;

protected:
    SpaceID spaceId_;
};

} // namespace kvstore
} // namespace niok
#endif