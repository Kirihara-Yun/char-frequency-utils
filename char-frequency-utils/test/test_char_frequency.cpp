#include <gtest/gtest.h>
#include "../include/CharFrequencyUtils.h"

using namespace char_utils;

// 基础单条校验测试
TEST(CharFrequencyTest, BasicCanConstruct) {
    // 正常场景：可以构造
    EXPECT_TRUE(CharFrequencyUtils::canConstruct("a", "ab"));
    EXPECT_TRUE(CharFrequencyUtils::canConstruct("aa", "aab"));
    // 正常场景：无法构造
    EXPECT_FALSE(CharFrequencyUtils::canConstruct("a", "b"));
    EXPECT_FALSE(CharFrequencyUtils::canConstruct("aa", "ab"));
    
    // 边界场景：ransomNote为空
    EXPECT_TRUE(CharFrequencyUtils::canConstruct("", "abc"));
    // 边界场景：magazine为空但ransomNote非空
    EXPECT_FALSE(CharFrequencyUtils::canConstruct("a", ""));
}

// 大小写不敏感测试
TEST(CharFrequencyTest, CaseInsensitiveTest) {
    // 区分大小写（默认）：无法构造
    EXPECT_FALSE(CharFrequencyUtils::canConstruct("A", "a"));
    // 不区分大小写：可以构造
    EXPECT_TRUE(CharFrequencyUtils::canConstruct("A", "a", true));
    EXPECT_TRUE(CharFrequencyUtils::canConstruct("Abc", "aBC", true));
}

// 策略指定测试
TEST(CharFrequencyTest, StrategySpecifyTest) {
    // 数组法（ARRAY_26）：纯小写字母
    EXPECT_TRUE(CharFrequencyUtils::canConstructWithStrategy("abc", "abcdef", StatStrategy::ARRAY_26, false));
    // 数组法（ARRAY_52）：包含大小写
    EXPECT_TRUE(CharFrequencyUtils::canConstructWithStrategy("Ab", "aAbB", StatStrategy::ARRAY_52, false));
    // 数组法（ARRAY_62）：包含数字
    EXPECT_TRUE(CharFrequencyUtils::canConstructWithStrategy("a1", "a1b2", StatStrategy::ARRAY_62, false));
    
    // 数组法遇到不支持的字符：返回false
    EXPECT_FALSE(CharFrequencyUtils::canConstructWithStrategy("a!", "a!b", StatStrategy::ARRAY_26, false));
}

// 批量校验测试
TEST(CharFrequencyTest, BatchCanConstruct) {
    std::vector<std::string> notes = {"a", "aa", "aaa", "", "b"};
    std::string mag = "aab";
    auto results = CharFrequencyUtils::batchCanConstruct(notes, mag, StatStrategy::HASH_TABLE, false);
    
    EXPECT_EQ(results.size(), 5);
    EXPECT_TRUE(results[0]);   // "a" → true
    EXPECT_TRUE(results[1]);   // "aa" → true
    EXPECT_FALSE(results[2]);  // "aaa" → false
    EXPECT_TRUE(results[3]);   // "" → true
    EXPECT_TRUE(results[4]);   // "b" → true
}

// 频次统计测试
TEST(CharFrequencyTest, FrequencyCountTest) {
    // 哈希表统计
    std::unordered_map<char, int> freqHash = CharFrequencyUtils::countFrequencyHash("aabcc");
    EXPECT_EQ(freqHash['a'], 2);
    EXPECT_EQ(freqHash['b'], 1);
    EXPECT_EQ(freqHash['c'], 2);
    
    // 数组统计（ARRAY_26）
    std::vector<int> freqArray = CharFrequencyUtils::countFrequencyArray("aabcc", StatStrategy::ARRAY_26, false);
    EXPECT_EQ(freqArray[0], 2);  // 'a' → 0
    EXPECT_EQ(freqArray[1], 1);  // 'b' → 1
    EXPECT_EQ(freqArray[2], 2);  // 'c' → 2
}

// 异常处理测试
TEST(CharFrequencyTest, ExceptionTest) {
    // 非法策略获取数组长度
    EXPECT_THROW(CharFrequencyUtils::getArraySizeByStrategy(StatStrategy::HASH_TABLE), InvalidInputException);
    // 数组统计包含不支持的字符
    EXPECT_THROW(CharFrequencyUtils::countFrequencyArray("a!", StatStrategy::ARRAY_26, false), InvalidInputException);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}