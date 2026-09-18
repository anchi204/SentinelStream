#include "risk/rule_engine.h"

#include <cstdlib>

namespace sentinel {

RuleEngine::RuleEngine(double high_value_threshold,
                       std::int64_t rapid_window_seconds)
    : high_value_threshold_(high_value_threshold),
      rapid_window_seconds_(rapid_window_seconds) {}

bool RuleEngine::is_duplicate(const Transaction& transaction) const {
    return seen_transactions_.find(transaction.transaction_id) != seen_transactions_.end();
}

bool RuleEngine::is_high_value(const Transaction& transaction) const {
    return transaction.amount >= high_value_threshold_;
}

bool RuleEngine::is_rapid(const Transaction& transaction) const {
    const auto it = last_user_timestamp_.find(transaction.user_id);
    if (it == last_user_timestamp_.end()) return false;

    const auto delta = std::llabs(transaction.timestamp - it->second);
    return delta <= rapid_window_seconds_;
}

RiskAlert RuleEngine::evaluate(const Transaction& transaction) {
    RiskAlert alert{transaction, {}};

    if (is_duplicate(transaction)) {
        alert.reasons.push_back(RiskReason::Duplicate);
    }
    if (is_high_value(transaction)) {
        alert.reasons.push_back(RiskReason::HighValue);
    }
    if (is_rapid(transaction)) {
        alert.reasons.push_back(RiskReason::RapidTransaction);
    }

    seen_transactions_[transaction.transaction_id] = transaction.user_id;
    last_user_timestamp_[transaction.user_id] = transaction.timestamp;

    return alert;
}

void RuleEngine::reset() {
    seen_transactions_.clear();
    last_user_timestamp_.clear();
}

std::string reason_to_string(RiskReason reason) {
    switch (reason) {
        case RiskReason::Duplicate: return "duplicate";
        case RiskReason::HighValue: return "high_value";
        case RiskReason::RapidTransaction: return "rapid_transaction";
    }
    return "unknown";
}

} // namespace sentinel
