#pragma once

#include <string>

namespace mochios {

namespace client {

typedef struct {
  std::string host;
  unsigned short port;
} Connection;

} // namespace client

} // namespace mochios