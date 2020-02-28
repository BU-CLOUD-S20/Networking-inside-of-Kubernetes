#ifndef COMMON_BASE_BASE_H_
#define COMMON_BASE_BASE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <regex>
#include <chrono>
#include <limits>

#include <functional>
#include <string>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>
#include <tuple>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <cassert>
#include <cmath>

// Formated logging
#define FLOG_FATAL(...) LOG(FATAL) << __VA_ARGS__
#define FLOG_ERROR(...) LOG(ERROR) << __VA_ARGS__
#define FLOG_WARN(...) LOG(WARNING) << __VA_ARGS__
#define FLOG_INFO(...) LOG(INFO) << __VA_ARGS__
#define FVLOG1(...) VLOG(1) << __VA_ARGS__
#define FVLOG2(...) VLOG(2) << __VA_ARGS__
#define FVLOG3(...) VLOG(3) << __VA_ARGS__
#define FVLOG4(...) VLOG(4) << __VA_ARGS__

#include "base/ThriftTypes.h"

#endif