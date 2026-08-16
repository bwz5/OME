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

bool ome::OrderBook::cancelOrder(ome::OrderId orderId) {
  if (orderId_to_node.find(orderId) == orderId_to_node.end()) {
    return false;
  }

  auto& node = orderId_to_node[orderId];
  switch (node->orderSide) {
    case ome::OrderSide::BUY: {
      auto& bidList = bids[node->price];
      totalBidQuantity -= node->quantity;
      bidList.erase(node);
      if (!bidList.size()) {
        bids.erase(node->price);
      }
      break;
    }
    case ome::OrderSide::SELL: {
      auto& askList = asks[node->price];
      totalAskQuantity -= node->quantity;
      askList.erase(node);
      if (!askList.size()) {
        asks.erase(node->price);
      }
      break;
    }
  }
  orderId_to_node.erase(orderId);
  return true;
}

ome::OrderId ome::OrderBook::placeOrder(ome::Price _p, ome::Quantity _q,
                                        ome::OrderSide _oS) {
  ome::Order order(_p, _q, _oS, currentOrderId);
  switch (_oS) {
    case ome::OrderSide::BUY:
      for (auto it = asks.begin(); it != asks.end();) {
        auto& [price, list] = *it;
        if (!order.quantity) {
          break;
        }
        while (list.size() > 0) {
          if (price > order.price) {  // we are no longer willing to buy
            break;
          }
          if (order.quantity < list.front().quantity) {  // then we update front
            list.front().quantity -= order.quantity;
            totalAskQuantity -= order.quantity;
            order.quantity = 0;
            break;
          } else {  // then we pop front off of the list and continue
            order.quantity -= list.front().quantity;
            totalAskQuantity -= list.front().quantity;
            orderId_to_node.erase(list.front().orderId);
            list.pop_front();
          }
        }
        if (list.empty()) {
          it = asks.erase(it);
        } else {
          it++;
        }
      }
      // now let's check if there is still some quantity left over
      if (order.quantity) {
        auto& list = bids[order.price];
        auto node = list.insert(list.end(), order);
        totalBidQuantity += order.quantity;
        orderId_to_node[order.orderId] = node;
      }
      break;
    case ome::OrderSide::SELL:
      for (auto it = bids.begin(); it != bids.end();) {
        auto& [price, list] = *it;
        if (!order.quantity) {
          break;
        }
        while (list.size() > 0) {
          if (price < order.price) {  // we are no longer willing to sell
            break;
          }
          if (order.quantity < list.front().quantity) {  // then we update front
            list.front().quantity -= order.quantity;
            totalBidQuantity -= order.quantity;
            order.quantity = 0;
            break;
          } else {  // then we pop front off of the list and continue
            order.quantity -= list.front().quantity;
            totalBidQuantity -= list.front().quantity;
            orderId_to_node.erase(list.front().orderId);
            list.pop_front();
          }
        }
        if (list.empty()) {
          it = bids.erase(it);
        } else {
          it++;
        }
      }
      // now let's check if there is still some quantity left over
      if (order.quantity) {
        auto& list = asks[order.price];
        auto node = list.insert(list.end(), order);
        totalAskQuantity += order.quantity;
        orderId_to_node[order.orderId] = node;
      }
      break;
  }
  // increment to next unique orderId
  currentOrderId++;
  return currentOrderId - 1;
}

std::optional<ome::Order> ome::OrderBook::getBestAsk() { return std::nullopt; }

std::optional<ome::Order> ome::OrderBook::getBestBid() { return std::nullopt; }

ome::Quantity ome::OrderBook::getTotalAskQuantity() { return totalAskQuantity; }

ome::Quantity ome::OrderBook::getTotalBidQuantity() { return totalBidQuantity; }
