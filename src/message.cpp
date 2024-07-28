#include <mochios/message.h>

mochios::Message::Message() { return; }

mochios::Message::Message(const std::string &message) {
  char method[8], tempPath[1024];
  sscanf(message.c_str(), "%s /%s HTTP/1.1", method, tempPath);
  this->path = tempPath;
  this->parsePath();
  this->parseHeaders(message);
  this->parseQueries();
  this->parseBody(message);
  return;
}

mochios::Message::~Message() { return; }

void mochios::Message::set(const std::string &key, const std::string &value) {
  this->headers[key] = value;
  return;
}

std::string mochios::Message::get(const std::string &key) {
  return this->headers[key];
}

std::string mochios::Message::stringify() {
  std::string message = "";
  this->set("Content-Length", std::to_string(this->body.dumps(0).length()));
  for (std::pair<const std::string, std::string> &header : this->headers) {
    message += header.first + ": " + header.second + "\r\n";
  }
  for (mochios::Cookie *cookie : this->cookies) {
    message += "Set-Cookie: " + cookie->serialize() + "\r\n";
  }
  message += "\r\n";
  return message;
}

void mochios::Message::parsePath() {
  this->path = this->path == "HTTP/1.1" ? "" : this->path;
  this->path = "/" + this->path;
  return;
}

void mochios::Message::parseHeaders(const std::string &message) {
  std::string line;
  std::istringstream stream(message);
  while (std::getline(stream, line) && line != "\r") {
    size_t separator = line.find(':', 0);
    if (separator != std::string::npos) {
      std::string key = line.substr(0, separator);
      std::string value =
          line.substr(separator + 2, line.size() - separator - 3);
      this->set(brewtils::string::lower(key), value);
    }
  }
}

void mochios::Message::parseQueries() {
  std::string tempPath = this->path;
  size_t start = tempPath.find("?", 0);
  if (start != std::string::npos) {
    start += 1;
    while (start < tempPath.size()) {
      size_t separator = tempPath.find('=', start);
      if (separator != std::string::npos) {
        std::string key = tempPath.substr(start, separator - start);
        start = separator + 1;
        size_t end = tempPath.find('&', start);
        if (end == std::string::npos) {
          end = tempPath.size();
        }

        std::string value = tempPath.substr(start, end - start);
        this->queries[brewtils::url::decode(key)] =
            brewtils::url::decode(value);
        start = end + 1;
      }
    }
  }

  tempPath = tempPath.substr(0, tempPath.find('?', 0));
  if (tempPath[tempPath.size() - 1] == '/') {
    tempPath = tempPath.substr(0, tempPath.size() - 1);
  }
  this->path = tempPath;
}

void mochios::Message::parseBody(const std::string &message) {
  std::string contentType = this->get("content-type");
  std::string body = message.substr(message.find("\r\n\r\n") + 4);
  if (contentType == "text/plain" || contentType == "application/javascript") {
    this->body = json::object(body);
  } else if (contentType == "application/json") {
    json::parser parser;
    this->body = parser.loads(body);
  } else if (contentType == "application/x-www-form-urlencoded") {
    std::vector<std::string> parts = brewtils::string::split(body, "&");
    std::string key;
    std::string value;
    this->body = json::object(std::map<std::string, json::object>());
    for (auto str : parts) {
      key = brewtils::url::decode(brewtils::string::split(str, "=")[0]);
      value = brewtils::url::decode(brewtils::string::split(str, "=")[1]);
      this->body[key] = json::object(value);
    }
  } else if (brewtils::string::split(contentType, ";")[0] ==
             "multipart/form-data") {
    std::string delimiter = brewtils::string::split(
        brewtils::string::split(contentType, ";")[1], "=")[1];
    std::vector<std::string> parts = brewtils::string::split(body, delimiter);
    std::vector<std::string> data;
    std::string key;
    std::string value;
    this->body = json::object(std::map<std::string, json::object>());
    for (auto str : parts) {
      data = brewtils::string::split(str,
                                     "Content-Disposition: form-data; name=\"");
      if (data.size() == 2) {
        key = brewtils::string::split(data[1], "\r\n")[0];
        key = key.substr(0, key.size() - 1);
        value = brewtils::string::split(data[1], "\r\n\r\n")[1];
        value = value.substr(0, value.size() - 3);
        this->body[key] = json::object(value);
      }
    }
  }
}
