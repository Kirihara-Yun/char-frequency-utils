cmake_minimum_required(VERSION 3.10)
project(char-frequency-utils)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项：开启警告、优化、调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -g")

# 包含头文件目录
include_directories(include)

# 核心库（静态库）
add_library(char_frequency_utils STATIC src/CharFrequencyUtils.cpp)

# 单元测试（依赖Google Test）
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_char_frequency test/test_char_frequency.cpp)
target_link_libraries(test_char_frequency char_frequency_utils ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

# 安装目标（便于系统级使用）
install(TARGETS char_frequency_utils DESTINATION lib)
install(FILES include/CharFrequencyUtils.h DESTINATION include)