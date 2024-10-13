#pragma once

#include <mochios/enums/method.h>
#include <mochios/message/message.h>

namespace mochios {

namespace message {

class Request : public Message {
private:
public:
  Request(const std::string &path);
  ~Request();

  std::string path;
  mochios::enums::method method;

  void print() override;
};

} // namespace message

} // namespace mochios