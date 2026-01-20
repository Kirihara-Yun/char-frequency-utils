#include "CharFrequencyUtils.h"
#include <cctype>
#include <algorithm>
#include <stdexcept>

namespace char_utils {

// ========== 内部辅助方法实现 ==========
void CharFrequencyUtils::validateInput(const std::string& ransomNote, const std::string& magazine) {
    // 边界校验：magazine为空但ransomNote非空
    if (magazine.empty() && !ransomNote.empty()) {
        throw InvalidInputException("Magazine is empty but ransomNote is not");
    }
    // 空字符串合法性校验（ransomNote为空直接合法）
    if (ransomNote.empty()) {
        return;
    }
}

std::string CharFrequencyUtils::normalizeCase(const std::string& str, bool toLower) {
    std::string res = str;
    for (char& c : res) {
        if (toLower) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
    }
    return res;
}

size_t CharFrequencyUtils::getArraySizeByStrategy(StatStrategy strategy) {
    switch (strategy) {
        case StatStrategy::ARRAY_26: return 26;   // 小写字母a-z
        case StatStrategy::ARRAY_52: return 52;   // 大小写字母（a-z:0-25, A-Z:26-51）
        case StatStrategy::ARRAY_62: return 62;   // 字母+数字（a-z:0-25, A-Z:26-51, 0-9:52-61）
        default: throw InvalidInputException("Unsupported array strategy");
    }
}

std::optional<int> CharFrequencyUtils::charToIndex(char c, StatStrategy strategy, bool caseInsensitive) {
    // 先统一大小写（若需要）
    if (caseInsensitive) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    switch (strategy) {
        case StatStrategy::ARRAY_26: {
            if (c >= 'a' && c <= 'z') {
                return c - 'a';
            }
            break;
        }
        case StatStrategy::ARRAY_52: {
            if (c >= 'a' && c <= 'z') {
                return c - 'a';
            } else if (c >= 'A' && c <= 'Z') {
                return 26 + (c - 'A');
            }
            break;
        }
        case StatStrategy::ARRAY_62: {
            if (c >= 'a' && c <= 'z') {
                return c - 'a';
            } else if (c >= 'A' && c <= 'Z') {
                return 26 + (c - 'A');
            } else if (c >= '0' && c <= '9') {
                return 52 + (c - '0');
            }
            break;
        }
        default: break;
    }
    // 字符不在策略支持的范围内
    return std::nullopt;
}

bool CharFrequencyUtils::validateFrequency(const std::unordered_map<char, int>& ransomFreq,
                                           const std::unordered_map<char, int>& magazineFreq) {
    for (const auto& [c, cnt] : ransomFreq) {
        auto it = magazineFreq.find(c);
        if (it == magazineFreq.end() || it->second < cnt) {
            return false;
        }
    }
    return true;
}

// ========== 频次统计实现 ==========
std::unordered_map<char, int> CharFrequencyUtils::countFrequencyHash(const std::string& str,
                                                                    bool caseInsensitive) {
    std::string normalized = caseInsensitive ? normalizeCase(str) : str;
    std::unordered_map<char, int> freq;
    for (char c : normalized) {
        freq[c]++; // 不存在的键自动初始化为0，再+1
    }
    return freq;
}

std::vector<int> CharFrequencyUtils::countFrequencyArray(const std::string& str,
                                                        StatStrategy strategy,
                                                        bool caseInsensitive) {
    size_t arrSize = getArraySizeByStrategy(strategy);
    std::vector<int> freq(arrSize, 0);
    std::string normalized = caseInsensitive ? normalizeCase(str) : str;

    for (char c : normalized) {
        auto idxOpt = charToIndex(c, strategy, caseInsensitive);
        if (idxOpt.has_value()) {
            freq[idxOpt.value()]++;
        } else {
            throw InvalidInputException("Character '" + std::string(1, c) + "' not supported by strategy");
        }
    }
    return freq;
}

// ========== 核心校验实现 ==========
bool CharFrequencyUtils::canConstruct(const std::string& ransomNote, const std::string& magazine,
                                      bool caseInsensitive) {
    // 边界快速返回：ransomNote为空直接返回true
    if (ransomNote.empty()) {
        return true;
    }
    try {
        validateInput(ransomNote, magazine);
    } catch (const InvalidInputException& e) {
        return false;
    }

    // 自动选择策略：检查magazine是否仅包含小写字母，是则用数组，否则用哈希表
    bool isAllLower = true;
    std::string normalizedMag = caseInsensitive ? normalizeCase(magazine) : magazine;
    for (char c : normalizedMag) {
        if (!(c >= 'a' && c <= 'z')) {
            isAllLower = false;
            break;
        }
    }

    if (isAllLower) {
        return canConstructWithStrategy(ransomNote, magazine, StatStrategy::ARRAY_26, caseInsensitive);
    } else {
        return canConstructWithStrategy(ransomNote, magazine, StatStrategy::HASH_TABLE, caseInsensitive);
    }
}

bool CharFrequencyUtils::canConstructWithStrategy(const std::string& ransomNote, const std::string& magazine,
                                                  StatStrategy strategy, bool caseInsensitive) {
    // 边界快速返回
    if (ransomNote.empty()) return true;
    validateInput(ransomNote, magazine);

    // 统一大小写（若需要）
    std::string rNote = caseInsensitive ? normalizeCase(ransomNote) : ransomNote;
    std::string mag = caseInsensitive ? normalizeCase(magazine) : magazine;

    if (strategy == StatStrategy::HASH_TABLE) {
        // 哈希表法：统计+校验（提前返回优化）
        std::unordered_map<char, int> magFreq = countFrequencyHash(mag, false);
        for (char c : rNote) {
            auto it = magFreq.find(c);
            if (it == magFreq.end() || it->second == 0) {
                return false;
            }
            it->second--;
        }
        return true;
    } else {
        // 数组法：统计+校验（高性能，缓存友好）
        size_t arrSize = getArraySizeByStrategy(strategy);
        std::vector<int> magFreq(arrSize, 0);

        // 统计magazine频次
        for (char c : mag) {
            auto idxOpt = charToIndex(c, strategy, false);
            if (idxOpt.has_value()) {
                magFreq[idxOpt.value()]++;
            } else {
                return false; // 包含不支持的字符，直接返回false
            }
        }

        // 校验ransomNote（提前返回）
        for (char c : rNote) {
            auto idxOpt = charToIndex(c, strategy, false);
            if (!idxOpt.has_value() || magFreq[idxOpt.value()] == 0) {
                return false;
            }
            magFreq[idxOpt.value()]--;
        }
        return true;
    }
}

// ========== 批量校验实现 ==========
std::vector<bool> CharFrequencyUtils::batchCanConstruct(const std::vector<std::string>& ransomNotes,
                                                        const std::string& magazine,
                                                        StatStrategy strategy,
                                                        bool caseInsensitive) {
    std::vector<bool> results;
    results.reserve(ransomNotes.size()); // 预分配空间，减少内存重分配

    // 提前统计magazine的频次，避免重复计算（性能优化）
    std::unordered_map<char, int> magFreqHash;
    std::vector<int> magFreqArray;
    if (strategy == StatStrategy::HASH_TABLE) {
        magFreqHash = countFrequencyHash(magazine, caseInsensitive);
    } else {
        magFreqArray = countFrequencyArray(magazine, strategy, caseInsensitive);
    }

    // 批量校验每个ransomNote
    std::string normalizedMag = caseInsensitive ? normalizeCase(magazine) : magazine;
    for (const std::string& note : ransomNotes) {
        if (note.empty()) {
            results.push_back(true);
            continue;
        }
        std::string rNote = caseInsensitive ? normalizeCase(note) : note;
        bool valid = true;

        if (strategy == StatStrategy::HASH_TABLE) {
            // 复制频次表（避免修改原表）
            std::unordered_map<char, int> tempFreq = magFreqHash;
            for (char c : rNote) {
                auto it = tempFreq.find(c);
                if (it == tempFreq.end() || it->second == 0) {
                    valid = false;
                    break;
                }
                it->second--;
            }
        } else {
            std::vector<int> tempFreq = magFreqArray;
            for (char c : rNote) {
                auto idxOpt = charToIndex(c, strategy, false);
                if (!idxOpt.has_value() || tempFreq[idxOpt.value()] == 0) {
                    valid = false;
                    break;
                }
                tempFreq[idxOpt.value()]--;
            }
        }
        results.push_back(valid);
    }
    return results;
}

} // namespace char_utils