#pragma once

#include <mochios/message/request.h>
#include <mochios/version.h>

namespace mochios {

namespace interceptor {

void userAgent(mochios::message::Request &request);

} // namespace interceptor

} // namespace mochios