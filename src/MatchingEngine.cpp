#include "ome/MatchingEngine.hpp"

bool ome::MatchingEngine::updateOrder(const Ticker& ticker,
                                      ome::OrderId orderId,
                                      ome::Quantity quantity,
                                      ome::Price price) {
  return true;
}

bool ome::MatchingEngine::cancelOrder(const Ticker& ticker,
                                      ome::OrderId orderId) {
  return true;
}

ome::OrderId ome::MatchingEngine::placeOrder(const Ticker& ticker,
                                             ome::Price _p, ome::Quantity _q,
                                             ome::OrderSide _oS) {
  return 0;
}

std::optional<ome::Order> ome::MatchingEngine::getBestAsk(
    const Ticker& ticker) const {
  return std::nullopt;
}

std::optional<ome::Order> ome::MatchingEngine::getBestBid(
    const Ticker& ticker) const {
  return std::nullopt;
}

ome::Quantity ome::MatchingEngine::getTotalAskQuantity(
    const Ticker& ticker) const {
  return 0;
}

ome::Quantity ome::MatchingEngine::getTotalBidQuantity(
    const Ticker& ticker) const {
  return 0;
}
