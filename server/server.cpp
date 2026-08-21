#include <httplib.h>

#include <iostream>

#include "ome/MatchingEngine.hpp"

int main() {
  httplib::Server svr;
  ome::MatchingEngine me;

  std::cout << "Starting OME Server\n";
  svr.listen("0.0.0.0", 8080);
  return 0;
}
