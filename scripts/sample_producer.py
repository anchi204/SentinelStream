import os
import time
import uuid
from kafka import KafkaProducer

BROKERS = os.getenv("KAFKA_BROKERS", "localhost:29092")
producer = KafkaProducer(bootstrap_servers=BROKERS, value_serializer=lambda value: value.encode("utf-8"))
now = int(time.time())
samples = [
    (f"tx-{uuid.uuid4().hex[:8]}", "user-1", 250.0, now, "merchant-a"),
    (f"tx-{uuid.uuid4().hex[:8]}", "user-1", 15000.0, now + 10, "merchant-b"),
    ("duplicate-demo", "user-2", 500.0, now + 20, "merchant-c"),
    ("duplicate-demo", "user-2", 500.0, now + 25, "merchant-c"),
]
for tx_id, user_id, amount, timestamp, merchant in samples:
    payload = f"{tx_id},{user_id},{amount},{timestamp},{merchant}"
    producer.send("transactions", payload)
    print("sent:", payload)
producer.flush()
