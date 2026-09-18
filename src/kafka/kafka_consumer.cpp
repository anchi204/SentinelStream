#include "kafka/kafka_consumer.h"

#include <cstdlib>
#include <sstream>
#include <vector>
#include <rdkafka/rdkafka.h>

namespace sentinel {

namespace {

bool parse_transaction(const std::string& payload, Transaction& tx) {
    std::stringstream ss(payload);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ',')) fields.push_back(field);
    if (fields.size() != 5) return false;

    try {
        tx.transaction_id = fields[0];
        tx.user_id = fields[1];
        tx.amount = std::stod(fields[2]);
        tx.timestamp = std::stoll(fields[3]);
        tx.merchant_id = fields[4];
    } catch (...) {
        return false;
    }
    return true;
}

} // namespace

KafkaConsumer::KafkaConsumer(std::string brokers, std::string topic, std::string group_id)
    : topic_(std::move(topic)) {
    char errstr[512];
    rd_kafka_conf_t* conf = rd_kafka_conf_new();
    rd_kafka_conf_set(conf, "bootstrap.servers", brokers.c_str(), errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "group.id", group_id.c_str(), errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "auto.offset.reset", "earliest", errstr, sizeof(errstr));

    consumer_ = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
    if (!consumer_) return;

    rd_kafka_topic_partition_list_t* topics = rd_kafka_topic_partition_list_new(1);
    rd_kafka_topic_partition_list_add(topics, topic_.c_str(), RD_KAFKA_PARTITION_UA);
    rd_kafka_subscribe(consumer_, topics);
    rd_kafka_topic_partition_list_destroy(topics);
}

KafkaConsumer::~KafkaConsumer() {
    if (consumer_) {
        rd_kafka_consumer_close(consumer_);
        rd_kafka_destroy(consumer_);
    }
}

bool KafkaConsumer::poll(Transaction& transaction, int timeout_ms) {
    if (!consumer_) return false;

    rd_kafka_message_t* message = rd_kafka_consumer_poll(consumer_, timeout_ms);
    if (!message) return false;

    bool ok = false;
    if (!message->err && message->payload && message->len > 0) {
        std::string payload(static_cast<char*>(message->payload), message->len);
        ok = parse_transaction(payload, transaction);
    }

    rd_kafka_message_destroy(message);
    return ok;
}

} // namespace sentinel
