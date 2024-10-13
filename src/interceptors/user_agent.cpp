#include <mochios/interceptors/user_agent.h>

const std::string mochiosUserAgent = "mochios/" + mochios::version;

void mochios::interceptor::userAgent(mochios::message::Request &request) {
  request.set("User-Agent", mochiosUserAgent);
}