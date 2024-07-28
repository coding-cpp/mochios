#pragma once

#include <mochios/message.h>

namespace mochios {

class Request : public Message {
public:
  Request();
  Request(const std::string &message);
  ~Request();
};

} // namespace mochios