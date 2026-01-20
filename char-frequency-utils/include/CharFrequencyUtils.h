#ifndef CHAR_FREQUENCY_UTILS_H
#define CHAR_FREQUENCY_UTILS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <optional>
#include <functional>

// 命名空间隔离，避免命名冲突
namespace char_utils {

// 自定义异常：非法输入异常
class InvalidInputException : public std::invalid_argument {
public:
    explicit InvalidInputException(const std::string& message) 
        : std::invalid_argument(message) {}
};

// 统计策略枚举
enum class StatStrategy {
    HASH_TABLE,  // 哈希表法（通用字符集）
    ARRAY_26,    // 数组法（仅小写字母a-z，高性能）
    ARRAY_52,    // 数组法（大小写字母a-z/A-Z，高性能）
    ARRAY_62     // 数组法（字母+数字，高性能）
};

// 字符频次统计结果（泛型，支持不同统计载体）
template <typename T>
struct FrequencyResult {
    T frequency;          // 频次存储（数组/哈希表）
    StatStrategy strategy;// 使用的统计策略
    size_t total_chars;   // 统计的字符总数
};

// 字符频次工具类（泛型模板，支持多策略、多字符集）
class CharFrequencyUtils {
public:
    // ========== 核心功能1：单条校验（ransomNote能否由magazine构成） ==========
    // 自动选择最优策略（字符集为纯小写字母用数组，否则用哈希表）
    static bool canConstruct(const std::string& ransomNote, const std::string& magazine, 
                             bool caseInsensitive = false);
    
    // 指定策略进行校验
    static bool canConstructWithStrategy(const std::string& ransomNote, const std::string& magazine,
                                         StatStrategy strategy, bool caseInsensitive = false);

    // ========== 核心功能2：批量校验（多个ransomNote能否由magazine构成） ==========
    static std::vector<bool> batchCanConstruct(const std::vector<std::string>& ransomNotes,
                                               const std::string& magazine,
                                               StatStrategy strategy = StatStrategy::HASH_TABLE,
                                               bool caseInsensitive = false);

    // ========== 核心功能3：统计字符频次 ==========
    // 通用哈希表统计（任意字符集）
    static std::unordered_map<char, int> countFrequencyHash(const std::string& str,
                                                            bool caseInsensitive = false);
    
    // 数组统计（指定字符集，高性能）
    static std::vector<int> countFrequencyArray(const std::string& str,
                                                StatStrategy strategy,
                                                bool caseInsensitive = false);

    // ========== 工具方法 ==========
    // 统一字符大小写（内部辅助）
    static std::string normalizeCase(const std::string& str, bool toLower = true);
    
    // 校验两个频次结果的兼容性（ransomNote频次 ≤ magazine频次）
    static bool validateFrequency(const std::unordered_map<char, int>& ransomFreq,
                                  const std::unordered_map<char, int>& magazineFreq);
    
    // 获取策略对应的数组长度
    static size_t getArraySizeByStrategy(StatStrategy strategy);

private:
    // 内部辅助：数组下标映射（根据策略转换字符为数组下标）
    static std::optional<int> charToIndex(char c, StatStrategy strategy, bool caseInsensitive);
    
    // 内部辅助：校验输入合法性
    static void validateInput(const std::string& ransomNote, const std::string& magazine);
};

} // namespace char_utils

#endif // CHAR_FREQUENCY_UTILS_H