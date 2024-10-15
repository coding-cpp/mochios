#pragma once

#include <cstring>
#include <sstream>
#include <unistd.h>

#include <brewtils/sys.h>
#include <brewtils/url.h>

#include <mochios/enums/method.h>
#include <mochios/message/request.h>
#include <mochios/message/response.h>

namespace mochios {

namespace helpers {

std::pair<std::string, std::string>
buildRequest(mochios::message::Request &request);

void buildResponse(mochios::message::Response &res,
                   std::stringstream &response);

mochios::message::Response send(mochios::message::Request &request,
                                const int &socket);

} // namespace helpers

} // namespace mochios
