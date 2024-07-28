#include <mochios/client.h>

void sendHelper(const std::string &verb, mochios::Request &request,
                mochios::Response &response, int socket,
                std::function<void(mochios::Response &)> callback = nullptr);

mochios::Client::Client(const std::string &host,
                        const unsigned short &port) noexcept(false) {
  signal(SIGPIPE, SIG_IGN);
  this->socket = brewtils::sys::socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket < 0) {
    logger::error("Socket not created!",
                  "mochios::Client::Client(const std::string &host, const "
                  "unsigned short &port)");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int err = getaddrinfo(host.c_str(), NULL, &hints, &result);
  if (err != 0) {
    logger::error("DNS lookup failed!",
                  "mochios::Client::Client(const std::string &host, const "
                  "unsigned short &port)");
  }

  // struct sockaddr_in *address = (struct sockaddr_in *)result->ai_addr;
  // address->sin_port = htons(port);

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
  char ipStr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(result->ai_family, &(ipv4->sin_addr), ipStr, sizeof(ipStr));

  logger::info("IP address is: " + std::string(ipStr),
               "mochios::Client::Client(const std::string &host, const "
               "unsigned short &port)");

  this->address.sin_family = AF_INET;
  this->address.sin_port = htons(port);
  this->address.sin_addr = ipv4->sin_addr;
  return;
}

mochios::Client::~Client() { return; }

void mochios::Client::get(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::post(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::put(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::del(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::patch(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::options(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::head(
    mochios::Request &request, mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  return this->send(mochios::verbs::GET, request, response, callback);
};

void mochios::Client::send(
    const std::string &verb, mochios::Request &request,
    mochios::Response &response,
    std::function<void(mochios::Response &)> callback) noexcept(false) {
  if (::connect(this->socket, (struct sockaddr *)&this->address,
                sizeof(this->address)) < 0) {
    logger::error("Connection failed!",
                  "mochios::Client::send(const std::string &verb, "
                  "mochios::Request &request, mochios::Response &response, "
                  "std::function<void(mochios::Response &)> callback)");
  }

  if (callback == nullptr) {
    sendHelper(verb, request, response, this->socket);
  } else {
    nexus::async(sendHelper, verb, std::ref(request), std::ref(response),
                 this->socket, callback);
  }
  return;
}

void sendHelper(const std::string &verb, mochios::Request &request,
                mochios::Response &response, int socket,
                std::function<void(mochios::Response &)> callback) {
  std::string headers = verb + " " + request.path + " HTTP/1.1\r\n";
  headers += request.stringify();
  std::string body = request.body.dumps(0);

  if (brewtils::sys::send(socket, headers.c_str(), headers.size(), 0) < 0) {
    logger::error(
        "Send failed!",
        "mochios::Client::sendHelper(const std::string &verb, mochios::Request "
        "&request, mochios::Response &response, int socket, "
        "std::function<void(mochios::Response "
        "&)> callback)");
  }
  if (brewtils::sys::send(socket, body.c_str(), body.size(), 0) < 0) {
    logger::error(
        "Send failed!",
        "mochios::Client::sendHelper(const std::string &verb, mochios::Request "
        "&request, mochios::Response &response, int socket, "
        "std::function<void(mochios::Response "
        "&)> callback)");
  }

  char buffer[4096] = {0};
  if (brewtils::sys::recv(socket, buffer, 4096, 0) < 0) {
    logger::error(
        "Receive failed!",
        "mochios::Client::sendHelper(const std::string &verb, mochios::Request "
        "&request, mochios::Response &response, int socket, "
        "std::function<void(mochios::Response "
        "&)> callback)");
  }

  ::close(socket);
  std::cout << buffer << std::endl;
  response = mochios::Response(buffer);
  if (callback != nullptr) {
    callback(response);
  }

  return;
}
