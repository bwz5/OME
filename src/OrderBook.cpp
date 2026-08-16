#include "ome/OrderBook.hpp"

#include "ome/Order.hpp"

std::optional<ome::OrderId> ome::OrderBook::updateOrder(ome::OrderId orderId,
                                                        ome::Quantity quantity,
                                                        ome::Price price) {
  if (orderId_to_node.find(orderId) == orderId_to_node.end()) {
    return std::nullopt;
  }
  auto& node = orderId_to_node[orderId];
  if (quantity < node->quantity && quantity > 0 && price == node->price) {
    if (node->orderSide == ome::OrderSide::BUY)
      totalBidQuantity -= (node->quantity - quantity);
    if (node->orderSide == ome::OrderSide::SELL)
      totalAskQuantity -= (node->quantity - quantity);

    node->quantity = quantity;
    return std::make_optional<ome::OrderId>(orderId);
  }
  ome::OrderId oldId = node->orderId;
  ome::OrderSide orderSide = node->orderSide;
  cancelOrder(oldId);
  std::optional<ome::OrderId> newOrder = std::nullopt;
  if (quantity > 0) {
    newOrder = std::make_optional<ome::OrderId>(
        placeOrder(price, quantity, orderSide));
  }
  return newOrder;
}

template <typename Map>
bool ome::OrderBook::eraseFromBook(Map& book, ome::Quantity& totalSideQuantity,
                                   std::list<ome::Order>::iterator node) {
  auto& list = book[node->price];
  totalSideQuantity -= node->quantity;
  if (!list.size()) {
    book.erase(node->price);
  }

  orderId_to_node.erase(node->orderId);
  list.erase(node);
  return true;
}

bool ome::OrderBook::cancelOrder(ome::OrderId orderId) {
  if (orderId_to_node.find(orderId) == orderId_to_node.end()) {
    return false;
  }
  auto& node = orderId_to_node[orderId];

  switch (node->orderSide) {
    case ome::OrderSide::BUY:
      eraseFromBook(bids, totalBidQuantity, node);
      return true;
    case ome::OrderSide::SELL:
      eraseFromBook(asks, totalAskQuantity, node);
      return true;
  }
}

template <typename OpposingMap, typename LikeMap, typename Comparator>
void ome::OrderBook::addOrderToBook(OpposingMap& opposingBook,
                                    LikeMap& likeBook, ome::Order& order,
                                    Comparator comp,
                                    ome::Quantity& opposingQuantity,
                                    ome::Quantity& likeQuantity) {
  for (auto it = opposingBook.begin(); it != opposingBook.end();) {
    auto& [price, list] = *it;
    if (!order.quantity) {
      break;
    }
    while (list.size() > 0) {
      if (comp(price, order.price)) {  // we are no longer willing to transact
        break;
      }
      if (order.quantity < list.front().quantity) {  // then we update front
        list.front().quantity -= order.quantity;
        opposingQuantity -= order.quantity;
        order.quantity = 0;
        break;
      } else {  // then we pop front off of the list and continue
        order.quantity -= list.front().quantity;
        opposingQuantity -= list.front().quantity;
        orderId_to_node.erase(list.front().orderId);
        list.pop_front();
      }
    }
    if (list.empty()) {
      it = opposingBook.erase(it);
    } else {
      it++;
    }
  }
  // now let's check if there is still some quantity left over
  if (order.quantity) {
    auto& list = likeBook[order.price];
    auto node = list.insert(list.end(), order);
    likeQuantity += order.quantity;
    orderId_to_node[order.orderId] = node;
  }
}

ome::OrderId ome::OrderBook::placeOrder(ome::Price _p, ome::Quantity _q,
                                        ome::OrderSide _oS) {
  ome::Order order(_p, _q, _oS, currentOrderId);
  switch (_oS) {
    case ome::OrderSide::BUY:
      addOrderToBook(asks, bids, order, std::greater<ome::Price>(),
                     totalAskQuantity, totalBidQuantity);
      break;
    case ome::OrderSide::SELL:
      addOrderToBook(bids, asks, order, std::less<ome::Price>(),
                     totalBidQuantity, totalAskQuantity);
      break;
  }

  // increment to next unique orderId
  currentOrderId++;
  return currentOrderId - 1;
}

std::optional<ome::Order> ome::OrderBook::getBestAsk() const {
  if (asks.empty()) {
    return std::nullopt;
  }
  auto it = asks.begin();
  const auto& [price, list] = *it;

  return std::make_optional<ome::Order>(list.front());
}

std::optional<ome::Order> ome::OrderBook::getBestBid() const {
  if (bids.empty()) {
    return std::nullopt;
  }
  auto it = bids.begin();
  const auto& [price, list] = *it;

  return std::make_optional<ome::Order>(list.front());
}

ome::Quantity ome::OrderBook::getTotalAskQuantity() const {
  return totalAskQuantity;
}

ome::Quantity ome::OrderBook::getTotalBidQuantity() const {
  return totalBidQuantity;
}
