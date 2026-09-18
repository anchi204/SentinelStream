#pragma once

#include "models/transaction.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace sentinel {

enum class RiskReason {
    Duplicate,
    HighValue,
    RapidTransaction
};

struct RiskAlert {
    Transaction transaction;
    std::vector<RiskReason> reasons;

    bool is_risky() const { return !reasons.empty(); }
};

class RuleEngine {
public:
    RuleEngine(double high_value_threshold = 10000.0,
               std::int64_t rapid_window_seconds = 60);

    RiskAlert evaluate(const Transaction& transaction);
    void reset();

private:
    bool is_duplicate(const Transaction& transaction) const;
    bool is_high_value(const Transaction& transaction) const;
    bool is_rapid(const Transaction& transaction) const;

    double high_value_threshold_;
    std::int64_t rapid_window_seconds_;
    std::unordered_map<std::string, std::int64_t> last_user_timestamp_;
    std::unordered_map<std::string, std::string> seen_transactions_;
};

std::string reason_to_string(RiskReason reason);

} // namespace sentinel
