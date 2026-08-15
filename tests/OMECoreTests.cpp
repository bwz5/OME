#include <gtest/gtest.h>

#include "ome/MatchingEngine.hpp"
#include "ome/OrderBook.hpp"

class OrderBookTest : public testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
  ome::OrderBook ob;
};

TEST_F(OrderBookTest, TestOfTest) { ASSERT_EQ(true, true); }

class MatchingEngineTest : public testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
  ome::MatchingEngine me;
};

TEST_F(MatchingEngineTest, TestOfTest) { ASSERT_EQ(true, true); }
