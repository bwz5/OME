#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

#include "ome/MatchingEngine.hpp"
#include "ome/Order.hpp"

struct GeneratedTestFields {
  ome::Ticker t;
  ome::OrderSide oS;
  ome::Price p;
  ome::Quantity q;

  GeneratedTestFields(const ome::Ticker& _t, ome::OrderSide _oS, ome::Price _p,
                      ome::Quantity _q)
      : t(_t), oS(_oS), p(_p), q(_q) {}
};

float calculateOrdersPerSecond(int n) {
  std::unordered_map<int, ome::Ticker> tickers = {
      {1, "AAPL"}, {2, "GOOGL"}, {3, "AMZN"}, {4, "BEN"}};
  ome::MatchingEngine me;

  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<ome::Price> priceDist(1, 100);
  std::uniform_int_distribution<ome::Quantity> quantityDist(50, 100);
  std::uniform_int_distribution<int> sideDist(0, 1);
  std::uniform_int_distribution<int> tickerDist(1, 4);

  std::vector<GeneratedTestFields> tests;

  for (int i = 0; i < n; i++) {
    ome::Ticker t = tickers[tickerDist(gen)];

    ome::OrderSide oS =
        sideDist(gen) == 0 ? ome::OrderSide::BUY : ome::OrderSide::SELL;

    ome::Price p = priceDist(gen);
    ome::Quantity q = quantityDist(gen);

    tests.emplace_back(t, oS, p, q);
  }

  auto startTime = std::chrono::steady_clock::now();

  for (int i = 0; i < n; i++) {
    me.placeOrder(tests[i].t, tests[i].p, tests[i].q, tests[i].oS);
  }

  std::chrono::duration<float> timeDifference =
      std::chrono::steady_clock::now() - startTime;

  return static_cast<float>(n) / timeDifference.count();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./ome_main N\n";
    return -1;
  }
  int n = std::stoi(argv[1]);
  std::cout << "Begining to parse " << n
            << " orders, returning the number of orders parsed per second\n";

  float ordersPerSecond = calculateOrdersPerSecond(n);
  std::cout << "Orders/sec: " << ordersPerSecond << '\n';
  return 0;
}
