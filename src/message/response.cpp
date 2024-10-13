#include <mochios/message/response.h>

mochios::message::Response::Response() { return; }

mochios::message::Response::~Response() { return; }

void mochios::message::Response::print() {
  logger::debug("Response:");
  logger::debug("  status: " + std::to_string(this->status));
  logger::debug("  statusText: " + this->statusText);
  
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