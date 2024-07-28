#pragma once

#include <arpa/inet.h>
#include <csignal>
#include <functional>
#include <netdb.h>
#include <netinet/in.h>

#include <brewtils/sys.h>
#include <logger/log.h>
#include <nexus/async.h>

#include <mochios/request.h>
#include <mochios/response.h>
#include <mochios/verbs.h>

namespace mochios {

class Client {
private:
  int socket;
  struct sockaddr_in address;

  void send(const std::string &verb, mochios::Request &request,
            mochios::Response &response,
            std::function<void(mochios::Response &)> callback =
                nullptr) noexcept(false);

public:
  Client(const std::string &host, const unsigned short &port) noexcept(false);
  ~Client();

  void get(mochios::Request &request, mochios::Response &response,
           std::function<void(mochios::Response &)> callback =
               nullptr) noexcept(false);
  void post(mochios::Request &request, mochios::Response &response,
            std::function<void(mochios::Response &)> callback =
                nullptr) noexcept(false);
  void put(mochios::Request &request, mochios::Response &response,
           std::function<void(mochios::Response &)> callback =
               nullptr) noexcept(false);
  void del(mochios::Request &request, mochios::Response &response,
           std::function<void(mochios::Response &)> callback =
               nullptr) noexcept(false);
  void patch(mochios::Request &request, mochios::Response &response,
             std::function<void(mochios::Response &)> callback =
                 nullptr) noexcept(false);
  void options(mochios::Request &request, mochios::Response &response,
               std::function<void(mochios::Response &)> callback =
                   nullptr) noexcept(false);
  void head(mochios::Request &request, mochios::Response &response,
            std::function<void(mochios::Response &)> callback =
                nullptr) noexcept(false);
};

} // namespace mochios