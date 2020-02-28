#ifndef KVSTORE_COMMON_H_
#define KVSTORE_COMMON_H_

#include "base/Base.h"

namespace niok {
namespace kvstorage {

enum ResultCode {
    SUCCEEDED               = 0,
    ERR_SPACE_NOT_FOUND     = -1,
    ERR_KEY_NOT_FOUND       = -3,
    ERR_CONSENSUS_ERROR     = -4,
    ERR_UNKNOWN             = -100,
};

inline leveldb::Slice toSlice(const std::string& str) {
    return leveldb::Slice(str.begin(), str.size());
}

} // namespace kvstorage
} // namespace niok
#endif