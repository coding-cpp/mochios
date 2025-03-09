#include <mochios/core/client.h>

int main(int argc, char **argv) {
  mochios::core::Connection connection;
  connection.host = "expresso.aditjain.me";
  connection.port = 80;

  mochios::core::Client client(connection);
  client.interceptors.request.use([](mochios::messages::Request &request) {
    logger::info("Intercepting request!");
    request.print();
  });
  mochios::messages::Response response;

  mochios::messages::Request healthRequest("/health");
  response = client.get(healthRequest);
  logger::success(response.body);

  mochios::messages::Request request("/about");
  response = client.get(request);
  logger::success(response.body.dumps(2));

  return EXIT_SUCCESS;
}