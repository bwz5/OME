#include "ome/OrderBook.hpp"

ome::OrderBook::OrderBook() {}

bool ome::OrderBook::updateOrder(ome::OrderId orderId, ome::Quantity quantity,
                                 ome::Price price) {
  return true;
}

bool ome::OrderBook::cancelOrder(ome::OrderId orderId) { return true; }

ome::OrderId ome::OrderBook::placeOrder(ome::Price _p, ome::Quantity _q,
                                        ome::OrderSide _oS) {
  return 0;
}

std::optional<ome::Order> ome::OrderBook::getBestAsk() { return std::nullopt; }

std::optional<ome::Order> ome::OrderBook::getBestBid() { return std::nullopt; }

ome::Quantity ome::OrderBook::getTotalAskQuantity() { return 0; }

ome::Quantity ome::OrderBook::getTotalBidQuantity() { return 0; }
