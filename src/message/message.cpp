#include <mochios/message/message.h>

mochios::message::Message::Message() { return; }

mochios::message::Message::~Message() { return; }

void mochios::message::Message::set(const std::string &key,
                                    const std::string &value) {
  this->headers[brewtils::string::lower(key)] = value;
  return;
}

const std::string mochios::message::Message::get(const std::string &key) const {
  return this->headers.at(brewtils::string::lower(key));
}
