#include <mochios/client/client.h>
#include <mochios/helpers/client.h>

mochios::Client::Client(const mochios::client::Connection &connection)
    : connection(connection) {
  struct addrinfo hints {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(connection.host.c_str(),
                  std::to_string(connection.port).c_str(), &hints,
                  &this->server) != 0) {
    logger::error("Failed to get address info for " + connection.host,
                  "mochios::Client::Client(const mochios::client::Connection "
                  "&connection)");
  }

  this->connect();
  close(this->socket);

  this->interceptors.request.use(mochios::interceptor::request::userAgent);
  this->interceptors.response.use(mochios::interceptor::response::json);
  return;
}

mochios::Client::~Client() {
  if (this->socket > 0) {
    close(this->socket);
  }

  return;
}

mochios::message::Response
mochios::Client::get(mochios::message::Request &request) {
  request.method = mochios::enums::method::GET;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::post(mochios::message::Request &request) {
  request.method = mochios::enums::method::POST;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::put(mochios::message::Request &request) {
  request.method = mochios::enums::method::PUT;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::patch(mochios::message::Request &request) {
  request.method = mochios::enums::method::PATCH;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::del(mochios::message::Request &request) {
  request.method = mochios::enums::method::DELETE;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::options(mochios::message::Request &request) {
  request.method = mochios::enums::method::OPTIONS;
  return this->sendHelper(request);
}

mochios::message::Response
mochios::Client::head(mochios::message::Request &request) {
  request.method = mochios::enums::method::HEAD;
  return this->sendHelper(request);
}

void mochios::Client::connect() {
  this->socket =
      brewtils::sys::socket(this->server->ai_family, this->server->ai_socktype,
                            this->server->ai_protocol);
  if (this->socket < 0) {
    freeaddrinfo(this->server);
    logger::error("Failed to create socket", "void mochios::Client::connect()");
  }

  if (::connect(this->socket, this->server->ai_addr, this->server->ai_addrlen) <
      0) {
    freeaddrinfo(this->server);
    close(this->socket);
    this->socket = -1;
    logger::error("Failed to connect to server at " + connection.host,
                  "void mochios::Client::connect()");
  }
}

mochios::message::Response
mochios::Client::sendHelper(mochios::message::Request &request) {
  request.set("Host", this->connection.host);
  request.set("Accept", "*/*");
  request.set("Connection", "close");
  for (const std::function<void(mochios::message::Request &)>
           &requestInterceptor : this->interceptors.request.interceptors) {
    requestInterceptor(request);
  }
  this->connect();
  mochios::message::Response res =
      mochios::helpers::send(request, this->socket);
  for (const std::function<void(mochios::message::Response &)>
           &responseInterceptor : this->interceptors.response.interceptors) {
    responseInterceptor(res);
  }
  return res;
}