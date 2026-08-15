#ifndef ORDER_HPP
#define ORDER_HPP

#include <chrono>
#include <cstdint>

namespace ome {
using Price = std::int64_t;
using Quantity = std::int64_t;
using OrderId = std::int32_t;
using Timestamp = std::chrono::steady_clock::time_point;
enum class OrderSide : std::int8_t { BUY, SELL };

struct Order {
  ome::Price price;
  ome::Quantity quantity;
  ome::OrderSide orderSide;
  ome::OrderId orderId;
  ome::Timestamp timestamp;

  Order(ome::Price _p, ome::Quantity _q, ome::OrderSide _oS, ome::OrderId _oId)
      : price(_p),
        quantity(_q),
        orderSide(_oS),
        orderId(_oId),
        timestamp(std::chrono::steady_clock::now()) {}
};
}  // namespace ome

#endif
