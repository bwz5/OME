#include <httplib.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "ome/MatchingEngine.hpp"

using json = nlohmann::json;

int main() {
  httplib::Server svr;
  ome::MatchingEngine me;

  svr.Post("/postOrder/:ticker/:side",
           [&](const httplib::Request &req, httplib::Response &res) {
             ome::Ticker ticker = req.path_params.at("ticker");
             std::string side = req.path_params.at("side");
             std::string priceStr = req.get_param_value("price");
             std::string quantityStr = req.get_param_value("quantity");

             ome::Price price;
             ome::Quantity quantity;

             // input validation
             if (side != "bid" && side != "ask") {
               res.status = 400;
               res.set_content("Error, side must be bid/ask", "text/plain");
               return;
             }

             try {
               price = std::stoll(priceStr);
               quantity = std::stoll(quantityStr);
             } catch (const std::exception &e) {
               res.status = 400;
               res.set_content("Error, please give a valid price/quantity",
                               "text/plain");
               return;
             }

             if (side == "bid") {
               me.placeOrder(ticker, price, quantity, ome::OrderSide::BUY);
             } else {  // confirmed side must be "ask"
               me.placeOrder(ticker, price, quantity, ome::OrderSide::SELL);
             }
             res.status = 200;
             res.set_content("SUCCESS", "text/plain");
             return;
           });

  svr.Patch("/updateOrder/:ticker/:order", [&](const httplib::Request &req,
                                               httplib::Response &res) {
    ome::Ticker ticker = req.path_params.at("ticker");
    ome::OrderId orderId = std::stoi(req.path_params.at("order"));
    std::string priceStr = req.get_param_value("price");
    std::string quantityStr = req.get_param_value("quantity");

    ome::Price price;
    ome::Quantity quantity;

    // input validation
    if (!me.tickerExists(ticker)) {
      res.status = 400;
      res.set_content("Error, ticker not in system.", "text/plain");
      return;
    }

    try {
      price = std::stoll(priceStr);
      quantity = std::stoll(quantityStr);
    } catch (const std::exception &e) {
      res.status = 400;
      res.set_content("Error, please give a valid price/quantity",
                      "text/plain");
      return;
    }

    std::optional<ome::OrderId> resultId =
        me.updateOrder(ticker, orderId, quantity, price);
    if (resultId == std::nullopt) {
      res.status = 400;
      res.set_content("Error, please give a valid orderId", "text/plain");
      return;
    }

    if (orderId == resultId) {
      res.status = 200;
      res.set_content("SUCCESS", "text/plain");
      return;
    }

    res.status = 200;
    res.set_content(
        "SUCCESS, New Order ID: " + std::to_string(resultId.value()),
        "text/plain");
    return;
  });

  svr.Delete("/cancelOrder/:ticker/:order",
             [&](const httplib::Request &req, httplib::Response &res) {
               ome::Ticker ticker = req.path_params.at("ticker");
               ome::OrderId orderId = std::stoi(req.path_params.at("order"));

               // input validation
               if (!me.tickerExists(ticker)) {
                 res.status = 400;
                 res.set_content("Error, ticker not in system.", "text/plain");
                 return;
               }

               bool result = me.cancelOrder(ticker, orderId);

               if (result) {
                 res.status = 200;
                 res.set_content("SUCCESS", "text/plain");
               } else {
                 res.status = 400;
                 res.set_content("Error, failed to cancel order", "text/plain");
               }
             });

  svr.Get("/snapshot/:ticker",
          [&](const httplib::Request &req, httplib::Response &res) {
            ome::Ticker ticker = req.path_params.at("ticker");

            // input validation
            if (!me.tickerExists(ticker)) {
              res.status = 400;
              res.set_content("Error, ticker not in system.", "text/plain");
              return;
            }

            // get the snapshots
            std::vector<ome::Order> bidSnapshot = me.getBidSnapshot(ticker);
            std::vector<ome::Order> askSnapshot = me.getAskSnapshot(ticker);

            // build the json body
            json response;

            response["bids"] = {};
            response["asks"] = {};

            for (const auto &elem : bidSnapshot) {
              json orderJson;
              orderJson["price"] = elem.price;
              orderJson["quantity"] = elem.quantity;
              response["bids"].push_back(orderJson);
            }
            for (const auto &elem : askSnapshot) {
              json orderJson;
              orderJson["price"] = elem.price;
              orderJson["quantity"] = elem.quantity;
              response["asks"].push_back(orderJson);
            }

            res.set_content(response.dump(), "application/json");
            return;
          });

  std::cout << "Starting OME Server\n";
  svr.listen("0.0.0.0", 8080);
  return 0;
}
