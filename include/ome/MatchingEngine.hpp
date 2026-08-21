#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include <optional>
#include <string>
#include <unordered_map>

#include "OrderBook.hpp"

namespace ome {
using Ticker = std::string;

class MatchingEngine {
 private:
  std::unordered_map<Ticker, ome::OrderBook> ticker_to_orderBook;

 public:
  // Returns the old orderId if only a quantity decrease, otherwise the previous
  // order is cancelled and a new order is tracked.
  std::optional<ome::OrderId> updateOrder(const Ticker& ticker,
                                          ome::OrderId orderId,
                                          ome::Quantity quantity,
                                          ome::Price price);

  // Returns "true" if the cancel was sucessful, "false" otherwise
  bool cancelOrder(const Ticker& ticker, ome::OrderId orderId);

  // Returns the unique orderId for this specific order. Fulfills the order as
  // much as possible.
  ome::OrderId placeOrder(const Ticker& ticker, ome::Price _p, ome::Quantity _q,
                          ome::OrderSide _oS);

  // Returns the lowest active Ask order if one exists
  std::optional<ome::Order> getBestAsk(const Ticker& ticker) const;

  // Returns the highest active Bid order if one exists
  std::optional<ome::Order> getBestBid(const Ticker& ticker) const;

  // Returns the total Ask quantity arcross all active Ask orders
  ome::Quantity getTotalAskQuantity(const Ticker& ticker) const;

  // Returns the total Bid quantity across all active Bid orders
  ome::Quantity getTotalBidQuantity(const Ticker& ticker) const;

  // Returns the current snapshot of the ticker's Bid order book
  std::vector<ome::Order> getBidSnapshot(const Ticker& ticker) const;

  // Returns the current snapshot of the ticker's Ask order book
  std::vector<ome::Order> getAskSnapshot(const Ticker& ticker) const;

  // Returns if a ticker exists or not in the system
  bool tickerExists(const Ticker& ticker) const;
};
}  // namespace ome

#endif
