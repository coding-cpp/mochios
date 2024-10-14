#include <mochios/helpers/client.h>

std::pair<std::string, std::string>
mochios::helpers::buildRequest(mochios::message::Request &request) {
  std::ostringstream oss;
  oss << request.method << " " << request.path;
  if (request.queries.size() > 0) {
    oss << "?";
    bool isFirst = true;
    for (const std::pair<std::string, std::string> &query : request.queries) {
      if (isFirst) {
        isFirst = false;
      } else {
        oss << "&";
      }
      oss << brewtils::url::encode(query.first) << "="
          << brewtils::url::encode(query.second);
    }
  }
  oss << " HTTP/1.1\r\n";

  std::string jsonifiedBody = "";
  if (request.method == mochios::enums::method::POST ||
      request.method == mochios::enums::method::PUT ||
      request.method == mochios::enums::method::PATCH) {
    jsonifiedBody = request.body.dumps(0);
    request.set("Content-Type", "application/json");
    request.set("Content-Length", std::to_string(jsonifiedBody.size()));
  }
  for (const std::pair<std::string, std::string> &header : request.headers) {
    oss << header.first << ": " << header.second << "\r\n";
  }
  oss << "\r\n";

  return std::make_pair(oss.str(), jsonifiedBody);
}

void mochios::helpers::buildResponse(mochios::message::Response &res,
                                     std::stringstream &response) {
  std::string line;

  // Headers
  while (std::getline(response, line) && line != "\r") {
    size_t separator = line.find(":");
    if (separator == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, separator);
    std::string value = line.substr(separator + 2, line.size() - separator - 3);
    res.set(key, value);
  }

  // Body
  std::stringstream body;
  while (std::getline(response, line) && line != "\r") {
    body << line;
  }

  res.body = body.str();
  return;
}

mochios::message::Response
mochios::helpers::send(mochios::message::Request &request, const int &socket) {
  std::pair<std::string, std::string> requestString =
      mochios::helpers::buildRequest(request);
  if (brewtils::sys::send(socket, requestString.first.c_str(),
                          requestString.first.size(), 0) < 0) {
    logger::error("Error sending request headers",
                  "mochios::message::Response "
                  "mochios::helpers::send(mochios::message::Request &request, "
                  "const mochios::enums::method &method, const int &socket)");
  }
  if (requestString.second.size() > 0) {
    if (brewtils::sys::send(socket, requestString.second.c_str(),
                            requestString.second.size(), 0) < 0) {
      logger::error(
          "Error sending request body",
          "mochios::message::Response "
          "mochios::helpers::send(mochios::message::Request &request, "
          "const mochios::enums::method &method, const int &socket)");
    }
  }

  std::stringstream oss;
  char buffer[4096];
  int bytesRead;
  while ((bytesRead = brewtils::sys::recv(socket, buffer, 4096, 0)) > 0) {
    oss << buffer;
    memset(buffer, 0, 4096);
  }
  close(socket);

  mochios::message::Response res;
  std::string line;
  std::getline(oss, line);
  std::vector<std::string> parts = brewtils::string::split(line, " ");
  res.status = std::stoi(parts[1]);
  if (parts.size() > 2) {
    res.statusText = parts[2];
  }

  mochios::helpers::buildResponse(res, oss);
  return res;
}