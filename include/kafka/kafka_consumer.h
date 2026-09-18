#pragma once

#include "models/transaction.h"

#include <memory>
#include <string>

struct rd_kafka_s;
using rd_kafka_t = rd_kafka_s;

namespace sentinel {

class KafkaConsumer {
public:
    KafkaConsumer(std::string brokers, std::string topic, std::string group_id);
    ~KafkaConsumer();

    KafkaConsumer(const KafkaConsumer&) = delete;
    KafkaConsumer& operator=(const KafkaConsumer&) = delete;

    bool poll(Transaction& transaction, int timeout_ms);

private:
    rd_kafka_t* consumer_{nullptr};
    std::string topic_;
};

} // namespace sentinel
