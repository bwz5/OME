#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <list>
#include <map>
#include <optional>
#include <unordered_map>

#include "Order.hpp"

namespace ome {
class OrderBook {
 private:
  std::map<ome::Price, std::list<ome::Order>, std::greater<ome::Price>> bids;
  std::map<ome::Price, std::list<ome::Order>> asks;

  std::unordered_map<ome::OrderId, std::list<ome::Order>::iterator>
      orderId_to_node;

  ome::Quantity totalBidQuantity = 0;
  ome::Quantity totalAskQuantity = 0;

  ome::OrderId currentOrderId = 1;

  // templated helper for cancelOrder()
  template <typename Map>
  bool eraseFromBook(Map& book, ome::Quantity& totalSideQuantity,
                     std::list<ome::Order>::iterator node);

  template <typename OpposingMap, typename LikeMap, typename Comparator>
  void addOrderToBook(OpposingMap& opposingBook, LikeMap& likeBook,
                      ome::Order& order, Comparator comp,
                      ome::Quantity& opposingQuantity,
                      ome::Quantity& likeQuantity);

 public:
  OrderBook() = default;

  // Returns the old orderId if only a quantity decrease, otherwise the previous
  // order is cancelled and a new order is tracked.
  std::optional<ome::OrderId> updateOrder(ome::OrderId orderId,
                                          ome::Quantity quantity,
                                          ome::Price price);

  // Returns "true" if the cancel was sucessful, "false" otherwise
  bool cancelOrder(ome::OrderId orderId);

  // Returns the unique orderId for this specific order. Fulfills the order as
  // much as possible.
  ome::OrderId placeOrder(ome::Price _p, ome::Quantity _q, ome::OrderSide _oS);

  // Returns the lowest active Ask order if one exists
  std::optional<ome::Order> getBestAsk() const;

  // Returns the highest active Bid order if one exists
  std::optional<ome::Order> getBestBid() const;

  // Returns the total Ask quantity arcross all active Ask orders
  ome::Quantity getTotalAskQuantity() const;

  // Returns the total Bid quantity across all active Bid orders
  ome::Quantity getTotalBidQuantity() const;
};
}  // namespace ome

#endif
