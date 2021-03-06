#ifndef COMMON_BASE_BASE_H_
#define COMMON_BASE_BASE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <typeinfo>       
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <regex>
#include <chrono>
#include <ctime>
#include <iomanip>
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

#include <netdb.h> /* struct hostent */
#include <arpa/inet.h> /* inet_ntop */
#include <signal.h>

#include "../utils/IPv4.h"
#include "../utils/Time.h"
#include "../utils/tcp_tacopie/TCPClient.h"
#include "../utils/tcp_tacopie/TCPServer.h"
#include "../utils/NetworkUtils.h"
#include <tacopie/tacopie>

#endif