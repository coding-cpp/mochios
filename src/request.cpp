#include <mochios/request.h>

mochios::Request::Request() { return; }

mochios::Request::Request(const std::string &message) : Message(message) {
  return;
}

mochios::Request::~Request() { return; }