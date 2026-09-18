#include "risk/risk_detector.h"

namespace sentinel {

RiskDetector::RiskDetector(double high_value_threshold,
                           std::int64_t rapid_window_seconds)
    : engine_(high_value_threshold, rapid_window_seconds) {}

RiskAlert RiskDetector::detect(const Transaction& transaction) {
    return engine_.evaluate(transaction);
}

void RiskDetector::reset() {
    engine_.reset();
}

} // namespace sentinel
