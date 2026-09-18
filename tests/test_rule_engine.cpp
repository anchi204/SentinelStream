#include "risk/rule_engine.h"
#include <gtest/gtest.h>
using sentinel::RiskReason;
using sentinel::RuleEngine;
using sentinel::Transaction;
TEST(RuleEngineTest, DetectsHighValueTransaction) {
    RuleEngine engine(10000.0, 60);
    const auto alert = engine.evaluate({"tx-1", "user-1", 15000.0, 1000, "merchant-1"});
    ASSERT_EQ(alert.reasons.size(), 1u);
    EXPECT_EQ(alert.reasons[0], RiskReason::HighValue);
}
TEST(RuleEngineTest, DetectsDuplicateTransaction) {
    RuleEngine engine;
    Transaction tx{"tx-1", "user-1", 500.0, 1000, "merchant-1"};
    engine.evaluate(tx);
    const auto alert = engine.evaluate(tx);
    ASSERT_EQ(alert.reasons.size(), 1u);
    EXPECT_EQ(alert.reasons[0], RiskReason::Duplicate);
}
TEST(RuleEngineTest, DetectsRapidTransaction) {
    RuleEngine engine(10000.0, 60);
    engine.evaluate({"tx-1", "user-1", 500.0, 1000, "merchant-1"});
    const auto alert = engine.evaluate({"tx-2", "user-1", 700.0, 1030, "merchant-2"});
    ASSERT_EQ(alert.reasons.size(), 1u);
    EXPECT_EQ(alert.reasons[0], RiskReason::RapidTransaction);
}
TEST(RuleEngineTest, AllowsTransactionOutsideRapidWindow) {
    RuleEngine engine(10000.0, 60);
    engine.evaluate({"tx-1", "user-1", 500.0, 1000, "merchant-1"});
    const auto alert = engine.evaluate({"tx-2", "user-1", 700.0, 1100, "merchant-2"});
    EXPECT_TRUE(alert.reasons.empty());
}
TEST(RuleEngineTest, DetectsMultipleRules) {
    RuleEngine engine(10000.0, 60);
    engine.evaluate({"tx-1", "user-1", 500.0, 1000, "merchant-1"});
    const auto alert = engine.evaluate({"tx-2", "user-1", 15000.0, 1030, "merchant-2"});
    EXPECT_EQ(alert.reasons.size(), 2u);
}
