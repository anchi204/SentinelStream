#pragma once
#include "risk/rule_engine.h"
#include <string>
namespace sentinel {
class KafkaProducer {
public:
    KafkaProducer(const std::string& brokers, const std::string& topic);
    ~KafkaProducer();
    bool publish(const RiskAlert& alert);
private:
    struct Impl;
    Impl* impl_{nullptr};
};
} // namespace sentinel
