#include <mochios/message/request.h>

mochios::message::Request::Request(const std::string &path) : path(path) {
  return;
}

mochios::message::Request::~Request() { return; }

void mochios::message::Request::print() {
  logger::debug("Request:");
  logger::debug("  path: " + this->path);
  logger::debug("  method: " + this->method);
  logger::debug("  headers:");
  for (const std::pair<std::string, std::string> &header : this->headers) {
    logger::debug("    " + header.first + ": " + header.second);
  }

  logger::debug("  params:");
  for (const std::pair<std::string, std::string> &param : this->params) {
    logger::debug("    " + param.first + ": " + param.second);
  }

  logger::debug("  queries:");
  for (const std::pair<std::string, std::string> &query : this->queries) {
    logger::debug("    " + query.first + ": " + query.second);
  }

  logger::debug("  body:");
  logger::debug("    " + this->body.dumps(2, 2));
  return;
}