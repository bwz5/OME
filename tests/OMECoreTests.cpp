#include <gtest/gtest.h>

#include "ome/MatchingEngine.hpp"
#include "ome/OrderBook.hpp"

class OrderBookTest : public testing::Test {
 protected:
  ome::OrderBook ob;

  void SetUp() override {
    ob.placeOrder(static_cast<ome::Price>(10), static_cast<ome::Quantity>(20),
                  ome::OrderSide::BUY);
    ob.placeOrder(static_cast<ome::Price>(11), static_cast<ome::Quantity>(20),
                  ome::OrderSide::BUY);
    ob.placeOrder(static_cast<ome::Price>(12), static_cast<ome::Quantity>(20),
                  ome::OrderSide::BUY);
    ob.placeOrder(static_cast<ome::Price>(13), static_cast<ome::Quantity>(20),
                  ome::OrderSide::SELL);
    ob.placeOrder(static_cast<ome::Price>(14), static_cast<ome::Quantity>(20),
                  ome::OrderSide::SELL);
    ob.placeOrder(static_cast<ome::Price>(15), static_cast<ome::Quantity>(20),
                  ome::OrderSide::SELL);
  }

  void TearDown() override {}
};

TEST_F(OrderBookTest, TestSellWithLowQuantityImmediatelyFulfilled) {
  ome::Quantity starting = ob.getTotalBidQuantity();

  ome::Quantity sellQuantity = static_cast<ome::Quantity>(10);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(5), sellQuantity,
                                   ome::OrderSide::SELL);

  ASSERT_EQ(starting - sellQuantity, ob.getTotalBidQuantity());
  ASSERT_EQ(ob.updateOrder(oId, 2, 2), std::nullopt);
}

TEST_F(OrderBookTest, TestBuyWithLowQuantityImmediatelyFulfilled) {
  ome::Quantity starting = ob.getTotalAskQuantity();

  ome::Quantity buyQuantity = static_cast<ome::Quantity>(10);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(20), buyQuantity,
                                   ome::OrderSide::BUY);

  ASSERT_EQ(starting - buyQuantity, ob.getTotalAskQuantity());
  ASSERT_EQ(ob.updateOrder(oId, 2, 2), std::nullopt);
}

TEST_F(OrderBookTest, TestSellWithHighQuantityImmediatelyFulfilled) {
  ome::Quantity startingBid = ob.getTotalBidQuantity();
  ome::Quantity startingAsk = ob.getTotalAskQuantity();

  ome::Quantity sellQuantity = static_cast<ome::Quantity>(100);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(5), sellQuantity,
                                   ome::OrderSide::SELL);

  ASSERT_EQ(static_cast<ome::Quantity>(0), ob.getTotalBidQuantity());
  ASSERT_EQ(startingAsk + 40, ob.getTotalAskQuantity());
}

TEST_F(OrderBookTest, TestBuyWithHighQuantityImmediatelyFulfilled) {
  ome::Quantity startingBid = ob.getTotalBidQuantity();
  ome::Quantity startingAsk = ob.getTotalAskQuantity();

  ome::Quantity buyQuantity = static_cast<ome::Quantity>(100);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(20), buyQuantity,
                                   ome::OrderSide::BUY);

  ASSERT_EQ(static_cast<ome::Quantity>(0), ob.getTotalAskQuantity());
  ASSERT_EQ(startingBid + 40, ob.getTotalBidQuantity());
}

TEST_F(OrderBookTest, TestCancelOrder) {
  ome::Quantity startingBid = ob.getTotalBidQuantity();
  ome::Quantity startingAsk = ob.getTotalAskQuantity();

  ome::Quantity buyQuantity = static_cast<ome::Quantity>(100);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(20), buyQuantity,
                                   ome::OrderSide::BUY);

  ob.cancelOrder(oId);

  ASSERT_EQ(static_cast<ome::Quantity>(0), ob.getTotalAskQuantity());
  ASSERT_EQ(startingBid, ob.getTotalBidQuantity());
}

TEST_F(OrderBookTest, TestUpdateOrderQuantityDecrease) {
  ome::Quantity startingBid = ob.getTotalBidQuantity();
  ome::Quantity startingAsk = ob.getTotalAskQuantity();

  ome::Quantity buyQuantity = static_cast<ome::Quantity>(100);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(20), buyQuantity,
                                   ome::OrderSide::BUY);

  // should have remaining quantity of 40, we lower here to 20
  ob.updateOrder(oId, static_cast<ome::Quantity>(20),
                 static_cast<ome::Price>(20));

  ASSERT_EQ(static_cast<ome::Quantity>(0), ob.getTotalAskQuantity());
  ASSERT_EQ(startingBid + static_cast<ome::Quantity>(20),
            ob.getTotalBidQuantity());
}

TEST_F(OrderBookTest, TestUpdateOrderPrice) {
  ome::Quantity startingBid = ob.getTotalBidQuantity();
  ome::Quantity startingAsk = ob.getTotalAskQuantity();

  ome::Quantity buyQuantity = static_cast<ome::Quantity>(100);
  ome::OrderId oId = ob.placeOrder(static_cast<ome::Price>(5), buyQuantity,
                                   ome::OrderSide::BUY);

  ASSERT_EQ(static_cast<ome::Quantity>(startingAsk), ob.getTotalAskQuantity());

  // Now we buy everything
  ob.updateOrder(oId, static_cast<ome::Quantity>(100),
                 static_cast<ome::Price>(20));

  ASSERT_EQ(static_cast<ome::Quantity>(0), ob.getTotalAskQuantity());
}

TEST_F(OrderBookTest, TestGetBestAsk) {
  ome::Order bestAsk = ob.getBestAsk().value();
  ASSERT_EQ(bestAsk.price, static_cast<ome::Price>(13));
}

TEST_F(OrderBookTest, TestGetBestBid) {
  ome::Order bestBid = ob.getBestBid().value();
  ASSERT_EQ(bestBid.price, static_cast<ome::Price>(12));
}

class MatchingEngineTest : public testing::Test {
 protected:
  ome::MatchingEngine me;
  void SetUp() override {
    me.placeOrder("AAPL", static_cast<ome::Price>(10),
                  static_cast<ome::Quantity>(20), ome::OrderSide::BUY);
    me.placeOrder("AAPL", static_cast<ome::Price>(11),
                  static_cast<ome::Quantity>(20), ome::OrderSide::BUY);
    me.placeOrder("AAPL", static_cast<ome::Price>(12),
                  static_cast<ome::Quantity>(20), ome::OrderSide::BUY);
    me.placeOrder("AAPL", static_cast<ome::Price>(13),
                  static_cast<ome::Quantity>(20), ome::OrderSide::SELL);
    me.placeOrder("AAPL", static_cast<ome::Price>(14),
                  static_cast<ome::Quantity>(20), ome::OrderSide::SELL);
    me.placeOrder("AAPL", static_cast<ome::Price>(15),
                  static_cast<ome::Quantity>(20), ome::OrderSide::SELL);
  }

  void TearDown() override {}
};

TEST_F(MatchingEngineTest, TestOfTest) { ASSERT_EQ(true, true); }
