#pragma once

#include <functional>

#include <mochios/message/request.h>
#include <mochios/message/response.h>

namespace mochios {

namespace interceptor {

typedef struct {
  std::vector<std::function<void(mochios::message::Request &)>> request;
  std::vector<std::function<void(mochios::message::Response &)>> response;
} Interceptors;

} // namespace interceptor

} // namespace mochios