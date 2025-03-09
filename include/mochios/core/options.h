#pragma once

#include <string>

namespace mochios {

namespace core {

typedef struct {
  std::string host;
  unsigned short port;
} Connection;

} // namespace core

} // namespace mochios