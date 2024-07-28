#include <mochios/client.h>

int main(int argc, char **argv) {
  mochios::Client client("api.nasa.gov", 443);
  mochios::Request request;
  request.path = "/planetary/apod";
  request.queries["api_key"] = "DEMO_KEY";

  mochios::Response response;
  client.get(request, response);

  std::cout << response.body.dumps(2) << std::endl;

  return EXIT_SUCCESS;
}