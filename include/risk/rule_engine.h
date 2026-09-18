#pragma once

#include "models/transaction.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class RuleEngine {
public:
    RuleEngine(double high_value_threshold, std::int64_t rapid_window_seconds);

    std::vector<std::string> evaluate(const Transaction& transaction);

private:
    bool isDuplicate(const Transaction& transaction) const;
    bool isHighValue(const Transaction& transaction) const;
    bool isRapid(const Transaction& transaction) const;

    double high_value_threshold_;
    std::int64_t rapid_window_seconds_;
    std::unordered_map<std::string, std::int64_t> last_transaction_by_user_;
    std::unordered_map<std::string, std::size_t> seen_transaction_ids_;
};
