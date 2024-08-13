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

#include <random>
namespace nExe {
inline static std::random_device vRandomDev;
inline static std::mt19937			 vRandomGen(vRandomDev());
}//namespace nExe

#include <thread>
#include <future>

#include <iostream>

#include <filesystem>

#include <exception>

/* fmt */

#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#define fLog(vStream, vHeader, vString, ...)                \
	({                                                        \
		fmt::println(                                           \
			vStream,                                              \
			"[{Header}]=(\n{String}\n)=[{Header}]",               \
			fmt::arg("Header", vHeader),                          \
			fmt::arg("String", fmt::format(vString, __VA_ARGS__)) \
		);                                                      \
	})//fLog
#define fLogOut(...) ({ fLog(stdout, __VA_ARGS__); })
#define fLogErr(...) ({ fLog(stderr, __VA_ARGS__); })

/* qt6 */

#include <QtGlobal>
#include <QtWidgets>

#endif//dChalGameOfLife_top_hpp
