#pragma once

#include <cstring>
#include <netdb.h>

#include <brewtils/sys.h>
#include <logger/log.h>

#include <mochios/core/options.h>
#include <mochios/interceptors/interceptor.h>
#include <mochios/interceptors/request/cookie.h>
#include <mochios/interceptors/request/user_agent.h>
#include <mochios/interceptors/response/json.h>

namespace mochios {

namespace core {

class Client {
private:
  int socket;
  struct addrinfo *server;

  mochios::core::Connection connection;
  mochios::interceptor::Interceptors internalInterceptors;

  void connect();
  mochios::messages::Response sendHelper(mochios::messages::Request &request);

public:
  Client(const mochios::core::Connection &connection);
  ~Client();

  mochios::interceptor::Interceptors interceptors;

  mochios::messages::Response get(mochios::messages::Request &request);
  mochios::messages::Response post(mochios::messages::Request &request);
  mochios::messages::Response put(mochios::messages::Request &request);
  mochios::messages::Response patch(mochios::messages::Request &request);
  mochios::messages::Response del(mochios::messages::Request &request);
  mochios::messages::Response options(mochios::messages::Request &request);
  mochios::messages::Response head(mochios::messages::Request &request);
};

} // namespace core

} // namespace mochios