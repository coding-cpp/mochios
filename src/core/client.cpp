#include <mochios/core/client.h>
#include <mochios/helpers/client.h>

mochios::core::Client::Client(const mochios::core::Connection &connection)
    : connection(connection) {
  struct addrinfo hints {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(connection.host.c_str(),
                  std::to_string(connection.port).c_str(), &hints,
                  &this->server) != 0) {
    logger::error(
        "Failed to get address info for " + connection.host,
        "mochios::core::Client::Client(const mochios::client::Connection "
        "&connection)");
  }

  this->connect();
  close(this->socket);

  this->internalInterceptors.request.use(
      mochios::interceptor::request::userAgent);
  this->internalInterceptors.request.use(mochios::interceptor::request::cookie);
  this->internalInterceptors.response.use(mochios::interceptor::response::json);
  return;
}

mochios::core::Client::~Client() {
  if (this->socket > 0) {
    close(this->socket);
  }

  return;
}

mochios::messages::Response
mochios::core::Client::get(mochios::messages::Request &request) {
  request.method = mochios::enums::method::GET;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::post(mochios::messages::Request &request) {
  request.method = mochios::enums::method::POST;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::put(mochios::messages::Request &request) {
  request.method = mochios::enums::method::PUT;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::patch(mochios::messages::Request &request) {
  request.method = mochios::enums::method::PATCH;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::del(mochios::messages::Request &request) {
  request.method = mochios::enums::method::DELETE;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::options(mochios::messages::Request &request) {
  request.method = mochios::enums::method::OPTIONS;
  return this->sendHelper(request);
}

mochios::messages::Response
mochios::core::Client::head(mochios::messages::Request &request) {
  request.method = mochios::enums::method::HEAD;
  return this->sendHelper(request);
}

void mochios::core::Client::connect() {
  this->socket =
      brewtils::sys::socket(this->server->ai_family, this->server->ai_socktype,
                            this->server->ai_protocol);
  if (this->socket < 0) {
    freeaddrinfo(this->server);
    logger::error("Failed to create socket",
                  "void mochios::core::Client::connect()");
  }

  if (::connect(this->socket, this->server->ai_addr, this->server->ai_addrlen) <
      0) {
    freeaddrinfo(this->server);
    close(this->socket);
    this->socket = -1;
    logger::error("Failed to connect to server at " + connection.host,
                  "void mochios::core::Client::connect()");
  }
}

mochios::messages::Response
mochios::core::Client::sendHelper(mochios::messages::Request &request) {
  request.set("Host", this->connection.host);
  request.set("Accept", "*/*");
  request.set("Connection", "close");
  for (const std::function<void(mochios::messages::Request &)>
           &requestInterceptor : this->interceptors.request.interceptors) {
    requestInterceptor(request);
  }
  for (const std::function<void(mochios::messages::Request &)>
           &requestInterceptor :
       this->internalInterceptors.request.interceptors) {
    requestInterceptor(request);
  }
  this->connect();
  mochios::messages::Response res =
      mochios::helpers::client::send(request, this->socket);
  for (const std::function<void(mochios::messages::Response &)>
           &responseInterceptor :
       this->internalInterceptors.response.interceptors) {
    responseInterceptor(res);
  }
  for (const std::function<void(mochios::messages::Response &)>
           &responseInterceptor : this->interceptors.response.interceptors) {
    responseInterceptor(res);
  }
  return res;
}