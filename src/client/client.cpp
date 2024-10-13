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
  return this->sendHelper(request, mochios::enums::method::GET);
}

mochios::message::Response
mochios::Client::post(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::POST);
}

mochios::message::Response
mochios::Client::put(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::PUT);
}

mochios::message::Response
mochios::Client::patch(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::PATCH);
}

mochios::message::Response
mochios::Client::del(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::DELETE);
}

mochios::message::Response
mochios::Client::options(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::OPTIONS);
}

mochios::message::Response
mochios::Client::head(mochios::message::Request &request) {
  return this->sendHelper(request, mochios::enums::method::HEAD);
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
mochios::Client::sendHelper(mochios::message::Request &request,
                            const mochios::enums::method &method) {
  this->connect();
  request.set("Host", this->connection.host);
  request.set("Accept", "*/*");
  request.set("Connection", "close");
  return mochios::helpers::send(request, method, this->socket);
}