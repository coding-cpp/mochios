#pragma once

#include <mochios/message/request.h>
#include <mochios/version.h>

namespace mochios {

namespace interceptor {

namespace request {

void userAgent(mochios::message::Request &request);

} // namespace request

} // namespace interceptor

} // namespace mochios