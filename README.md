# SentinelStream

SentinelStream is a real-time transaction risk detection pipeline built with C++, Apache Kafka, Docker, CMake, and Google Test.

## Features
- Kafka-based transaction ingestion and risk-event publishing
- Duplicate, high-value, and rapid-transaction detection
- Google Test coverage for core detection rules
- Docker Compose setup for Kafka and the application

## Quick start

```bash
docker compose up --build
```

The application consumes events from `transactions` and publishes risky events to `risk-alerts`.

## Local build

Requires C++17, CMake, and librdkafka.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Sample producer

```bash
pip install kafka-python
python scripts/sample_producer.py
```

Payload format:
`transaction_id,user_id,amount,timestamp,merchant_id`
