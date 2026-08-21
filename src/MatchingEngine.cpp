#include "ome/MatchingEngine.hpp"

std::optional<ome::OrderId> ome::MatchingEngine::updateOrder(
    const Ticker& ticker, ome::OrderId orderId, ome::Quantity quantity,
    ome::Price price) {
  auto& ob = ticker_to_orderBook[ticker];

  return ob.updateOrder(orderId, quantity, price);
}

bool ome::MatchingEngine::cancelOrder(const Ticker& ticker,
                                      ome::OrderId orderId) {
  auto& ob = ticker_to_orderBook[ticker];
  return ob.cancelOrder(orderId);
}

ome::OrderId ome::MatchingEngine::placeOrder(const Ticker& ticker,
                                             ome::Price _p, ome::Quantity _q,
                                             ome::OrderSide _oS) {
  auto& ob = ticker_to_orderBook[ticker];
  return ob.placeOrder(_p, _q, _oS);
}

std::optional<ome::Order> ome::MatchingEngine::getBestAsk(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return std::nullopt;
  }
  return it->second.getBestAsk();
}

std::optional<ome::Order> ome::MatchingEngine::getBestBid(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return std::nullopt;
  }
  return it->second.getBestBid();
}

ome::Quantity ome::MatchingEngine::getTotalAskQuantity(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return static_cast<ome::Quantity>(0);
  }

  return it->second.getTotalAskQuantity();
}

ome::Quantity ome::MatchingEngine::getTotalBidQuantity(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return static_cast<ome::Quantity>(0);
  }

  return it->second.getTotalBidQuantity();
}

std::vector<ome::Order> ome::MatchingEngine::getBidSnapshot(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return {};
  }

  return it->second.getBidSnapshot();
}

std::vector<ome::Order> ome::MatchingEngine::getAskSnapshot(
    const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return {};
  }

  return it->second.getAskSnapshot();
}

bool ome::MatchingEngine::tickerExists(const Ticker& ticker) const {
  auto it = ticker_to_orderBook.find(ticker);
  if (it == ticker_to_orderBook.end()) {
    return false;
  }
  return true;
}
