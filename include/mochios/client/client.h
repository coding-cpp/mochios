#pragma once

#include <cstring>
#include <netdb.h>

#include <brewtils/sys.h>
#include <logger/log.h>

#include <mochios/client/options.h>
#include <mochios/enums/method.h>
#include <mochios/message/request.h>
#include <mochios/message/response.h>

namespace mochios {

class Client {
private:
  int socket;
  struct addrinfo *server;

  mochios::client::Connection connection;

  void connect();
  mochios::message::Response sendHelper(mochios::message::Request &request,
                                        const mochios::enums::method &method);

public:
  Client(const mochios::client::Connection &connection);
  ~Client();

  mochios::message::Response get(mochios::message::Request &request);
  mochios::message::Response post(mochios::message::Request &request);
  mochios::message::Response put(mochios::message::Request &request);
  mochios::message::Response patch(mochios::message::Request &request);
  mochios::message::Response del(mochios::message::Request &request);
  mochios::message::Response options(mochios::message::Request &request);
  mochios::message::Response head(mochios::message::Request &request);
};

} // namespace mochios