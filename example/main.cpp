#include <mochios/client/client.h>

int main(int argc, char **argv) {
  mochios::client::Connection connection;
  connection.host = "expresso.aditjain.me";
  connection.port = 80;

  mochios::Client client(connection);
  client.interceptors.request.push_back([](mochios::message::Request &request) {
    logger::info("Intercepting request!");
    request.print();
  });
  mochios::message::Response response;

  mochios::message::Request healthRequest("/health");
  response = client.get(healthRequest);
  logger::success(response.body);

  json::parser parser;
  mochios::message::Request request("/about");
  response = client.get(request);
  logger::success(parser.loads(response.body).dumps(2));

  return EXIT_SUCCESS;
}