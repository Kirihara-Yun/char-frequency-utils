# char-frequency-utils
高性能、多策略的字符频次统计与校验工具库，支持通用哈希表和高性能数组两种实现，适配不同字符集场景，提供批量校验、大小写兼容等实用功能。

## 功能特性
- ✅ 双策略实现：哈希表（通用字符集）+ 数组（有限字符集，高性能）
- ✅ 自动选优：根据字符集自动选择最优统计策略
- ✅ 多字符集适配：支持小写字母/大小写字母/字母+数字三种数组策略
- ✅ 实用扩展：单条/批量校验、字符频次统计、大小写不敏感处理
- ✅ 高性能优化：提前返回、缓存友好的数组布局、批量校验预计算频次
- ✅ 健壮性保障：完善的异常处理、边界场景全覆盖
- ✅ 工程化设计：命名空间隔离、静态库编译、单元测试全覆盖

## 安装使用
### 环境依赖
- C++17及以上编译器（GCC/Clang/MSVC）
- CMake 3.10+（编译）
- Google Test（单元测试，可选）

### 编译步骤
```bash
# 克隆仓库
git clone https://github.com/Kirihara-Yun/char-frequency-utils.git
cd char-frequency-utils

# 编译构建
mkdir build && cd build
cmake ..
make

# 运行单元测试
./test_char_frequency
