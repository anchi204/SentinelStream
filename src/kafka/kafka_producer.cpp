#include "kafka/kafka_producer.h"
#include <rdkafka/rdkafka.h>
#include <sstream>
namespace sentinel {
struct KafkaProducer::Impl { rd_kafka_t* producer{}; rd_kafka_topic_t* topic{}; };
KafkaProducer::KafkaProducer(const std::string& brokers, const std::string& topic) : impl_(new Impl{}) {
    char errstr[512];
    rd_kafka_conf_t* conf = rd_kafka_conf_new();
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers.c_str(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        rd_kafka_conf_destroy(conf); delete impl_; impl_ = nullptr; return;
    }
    impl_->producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (impl_->producer) impl_->topic = rd_kafka_topic_new(impl_->producer, topic.c_str(), nullptr);
}
KafkaProducer::~KafkaProducer() {
    if (!impl_) return;
    if (impl_->producer) rd_kafka_flush(impl_->producer, 5000);
    if (impl_->topic) rd_kafka_topic_destroy(impl_->topic);
    if (impl_->producer) rd_kafka_destroy(impl_->producer);
    delete impl_;
}
bool KafkaProducer::publish(const RiskAlert& alert) {
    if (!impl_ || !impl_->producer || !impl_->topic) return false;
    std::ostringstream out;
    const auto& tx = alert.transaction;
    out << tx.transaction_id << ',' << tx.user_id << ',' << tx.amount << ',' << tx.timestamp << ',' << tx.merchant_id << '|';
    for (std::size_t i = 0; i < alert.reasons.size(); ++i) {
        if (i) out << ',';
        out << reason_to_string(alert.reasons[i]);
    }
    const std::string payload = out.str();
    const auto err = rd_kafka_produce(impl_->topic, RD_KAFKA_MSG_F_COPY,
        const_cast<char*>(payload.data()), payload.size(), nullptr, 0, nullptr);
    rd_kafka_poll(impl_->producer, 0);
    return err == 0;
}
} // namespace sentinel
