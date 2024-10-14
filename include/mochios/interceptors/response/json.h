#pragma once

#include <json/parse.h>

#include <mochios/message/response.h>

namespace mochios {

namespace interceptor {

namespace response {

void json(mochios::message::Response &response);

} // namespace response

} // namespace interceptor

} // namespace mochios