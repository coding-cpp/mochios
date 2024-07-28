#pragma once

#include <unordered_map>

#include <brewtils/string.h>
#include <json/parse.h>

#include <mochios/cookie.h>

namespace mochios {

class Message {
private:
  void parsePath();
  void parseHeaders(const std::string &message);
  void parseQueries();
  void parseBody(const std::string &message);

protected:
  std::vector<mochios::Cookie *> cookies;
  std::unordered_map<std::string, std::string> headers;

public:
  Message();
  Message(const std::string &message);
  ~Message();

  std::string path;
  json::object body;
  std::unordered_map<std::string, std::string> queries;

  void set(const std::string &key, const std::string &value);
  std::string get(const std::string &key);

  std::string stringify();
};

} // namespace mochios