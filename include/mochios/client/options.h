#pragma once

#include <string>

namespace mochios {

namespace client {

struct Connection {
  std::string host;
  unsigned short port;
  unsigned int timeout = 2;
};

} // namespace client

} // namespace mochios