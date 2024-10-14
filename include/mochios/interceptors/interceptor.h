#pragma once

#include <functional>

#include <mochios/message/request.h>
#include <mochios/message/response.h>

namespace mochios {

class Client;

namespace interceptor {

template <typename T> class Manager {
private:
  std::vector<std::function<void(T &)>> interceptors;

public:
  Manager() { return; }
  ~Manager() {
    this->interceptors.clear();
    return;
  }

  void use(std::function<void(T &)> interceptor) {
    interceptors.push_back(interceptor);
  }

  friend class mochios::Client;
};

typedef struct {
  mochios::interceptor::Manager<mochios::message::Request> request;
  mochios::interceptor::Manager<mochios::message::Response> response;
} Interceptors;

} // namespace interceptor

} // namespace mochios