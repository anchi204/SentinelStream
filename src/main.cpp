#include "kafka/kafka_consumer.h"
#include "kafka/kafka_producer.h"
#include "risk/risk_detector.h"
#include <cstdlib>
#include <iostream>
#include <string>
int main() {
    const char* broker_env = std::getenv("KAFKA_BROKERS");
    const std::string brokers = broker_env ? broker_env : "localhost:29092";
    sentinel::KafkaConsumer consumer(brokers, "transactions", "sentinelstream");
    sentinel::KafkaProducer producer(brokers, "risk-alerts");
    sentinel::RiskDetector detector;
    std::cout << "SentinelStream started. Waiting for transactions..." << std::endl;
    sentinel::Transaction transaction;
    while (true) {
        if (consumer.poll(transaction, 1000)) {
            const auto alert = detector.detect(transaction);
            if (alert.is_risky()) {
                const bool published = producer.publish(alert);
                std::cout << "Risk detected for " << transaction.transaction_id
                          << " (" << alert.reasons.size() << " rule(s), publish="
                          << (published ? "ok" : "failed") << ")" << std::endl;
            }
        }
    }
}
