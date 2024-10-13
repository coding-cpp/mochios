#pragma once

#include <brewtils/string.h>
#include <json/parse.h>

namespace mochios {

namespace message {

class Message {
private:
protected:
public:
  Message();
  ~Message();

  // Don't use headers directly! Use them using the get and set
  // methods
  std::map<std::string, std::string> headers;
  std::map<std::string, std::string> params;
  std::map<std::string, std::string> queries;
  json::object body;

  void set(const std::string &key, const std::string &value);
  const std::string get(const std::string &key) const;

  virtual void print() = 0;
};

} // namespace message

} // namespace mochios