# Architecture

```text
Transaction Producer
        |
        v
 Kafka: transactions
        |
        v
 C++ Kafka Consumer
        |
        v
   Risk Detector
        |
        v
    Rule Engine
   /     |      \
duplicate high   rapid
        |
        v
 Kafka: risk-alerts
```

SentinelStream consumes transaction events from Kafka, converts each event into a typed C++ object, evaluates independent risk rules, and publishes risky transactions to the risk-alerts topic.

The rule engine keeps lightweight in-memory state for transaction IDs and recent user activity.
