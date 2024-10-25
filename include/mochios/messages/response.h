#pragma once

#include <mochios/messages/message.h>

namespace mochios {

namespace messages {

class Response : public Message {
private:
public:
  Response();
  ~Response();

  int status;
  std::string statusText;

  void print() override;
};

} // namespace messages

} // namespace mochios