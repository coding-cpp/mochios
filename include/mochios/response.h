#pragma once

#include <mochios/message.h>

namespace mochios {

class Response : public Message {
public:
  Response();
  Response(const std::string &message);
  ~Response();
};

} // namespace mochios