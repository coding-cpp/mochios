#pragma once

#include <mochios/enums/method.h>
#include <mochios/messages/message.h>

namespace mochios {

namespace messages {

class Request : public Message {
private:
public:
  Request(const std::string &path);
  ~Request();

  std::string path;
  mochios::enums::method method;

  void print() override;
};

} // namespace messages

} // namespace mochios