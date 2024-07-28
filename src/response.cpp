#include <mochios/response.h>

mochios::Response::Response() { return; }

mochios::Response::Response(const std::string &message) : Message(message) {
  return;
}

mochios::Response::~Response() { return; }