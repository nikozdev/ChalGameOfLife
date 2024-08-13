#ifndef dChalGameOfLife_top_hpp
#define dChalGameOfLife_top_hpp

/* std */

#include <cstdlib>
#include <cstdio>

#include <memory>

#include <algorithm>
#include <array>
#include <vector>
#include <initializer_list>
#include <map>
#include <unordered_map>

#include <optional>
#include <functional>

#include <string>
#include <string_view>

#include <chrono>
namespace nExe {
using namespace std::literals::chrono_literals;
using tTimeCount = std::chrono::nanoseconds;
using tTimePoint = std::chrono::time_point<tTimeCount>;
using tTimeClock = std::chrono::steady_clock;
}

#include <random>
namespace nExe {
static inline std::random_device vRandomDev;
static inline std::mt19937 vRandomGen(vRandomDev());
}

#include <thread>
#include <future>

#include <iostream>

#include <filesystem>

/* fmt */

#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

/* qt6 */

#include <QtGlobal>
#include <QtWidgets>

#endif//dChalGameOfLife_top_hpp
