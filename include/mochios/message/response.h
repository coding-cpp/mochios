#pragma once

#include <mochios/message/message.h>

namespace mochios {

namespace message {

class Response : public Message {
private:
public:
  Response();
  ~Response();

  int status;
  std::string statusText;

  void print() override;
};

} // namespace message

} // namespace mochios