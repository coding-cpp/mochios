#pragma once

#include <cstring>
#include <netdb.h>

#include <brewtils/sys.h>
#include <logger/log.h>

#include <mochios/client/options.h>
#include <mochios/interceptors/interceptor.h>
#include <mochios/interceptors/request/user_agent.h>
#include <mochios/interceptors/response/json.h>

namespace mochios {

class Client {
private:
  int socket;
  struct addrinfo *server;

  mochios::client::Connection connection;

  void connect();
  mochios::message::Response sendHelper(mochios::message::Request &request);

public:
  Client(const mochios::client::Connection &connection);
  ~Client();

  mochios::interceptor::Interceptors interceptors;

  mochios::message::Response get(mochios::message::Request &request);
  mochios::message::Response post(mochios::message::Request &request);
  mochios::message::Response put(mochios::message::Request &request);
  mochios::message::Response patch(mochios::message::Request &request);
  mochios::message::Response del(mochios::message::Request &request);
  mochios::message::Response options(mochios::message::Request &request);
  mochios::message::Response head(mochios::message::Request &request);
};

} // namespace mochios