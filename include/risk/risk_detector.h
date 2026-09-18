#pragma once

#include "risk/rule_engine.h"

namespace sentinel {

class RiskDetector {
public:
    RiskDetector(double high_value_threshold = 10000.0,
                 std::int64_t rapid_window_seconds = 60);

    RiskAlert detect(const Transaction& transaction);
    void reset();

private:
    RuleEngine engine_;
};

} // namespace sentinel
