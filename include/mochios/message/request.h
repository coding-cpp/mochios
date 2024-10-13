#pragma once

#include <mochios/message/message.h>

namespace mochios {

namespace message {

class Request : public Message {
private:
public:
  Request(const std::string &path);
  ~Request();

  std::string path;

  void print() override;
};

} // namespace message

} // namespace mochios