#ifndef KVSTORE_ROCKSENGINE_H_
#define KVSTORE_ROCKSENGINE_H_

#include <leveldb/db.h>
#include "base/Base.h"
#include "kvstore/KVEngine.h"

namespace niok {
namespace kvstore {

class LevelEngine : public KVEngine {
private:

public:
    ResultCode get(const std::string& key, std::string* value) override;
    
    ResultCode put(std::string key, std::string value) override;

};
} // namespace kvstore    
} // namespace niok

#endif