#ifndef NALL_HTTP_REQUEST_HPP
#define NALL_HTTP_REQUEST_HPP

#include <nall/http/message.hpp>

namespace nall {

struct httpRequest : httpMessage {
  using type = httpRequest;

  enum class RequestType : unsigned { None, Head, Get, Post };

  explicit operator bool() const { return requestType() != RequestType::None; }

  inline auto head(const function<bool (const uint8_t* data, unsigned size)>& callback) const -> bool;
  inline auto setHead() -> bool;

  inline auto body(const function<bool (const uint8_t* data, unsigned size)>& callback) const -> bool;
  inline auto setBody() -> bool;

  auto ipv4() const -> bool { return _ipv6 == false; }
  auto ipv6() const -> bool { return _ipv6 == true; }
  auto ip() const -> string { return _ip; }

  auto requestType() const -> RequestType { return _requestType; }
  auto setRequestType(RequestType value) -> void { _requestType = value; }

  auto path() const -> string { return _path; }
  auto setPath(const string& value) -> void { _path = value; }

  auto appendHeader(const string& name, const string& value = "") -> type& { return httpMessage::appendHeader(name, value), *this; }
  auto removeHeader(const string& name) -> type& { return httpMessage::removeHeader(name), *this; }
  auto setHeader(const string& name, const string& value = "") -> type& { return httpMessage::setHeader(name, value), *this; }

  auto cookie(const string& name) const -> string { return _cookie.get(name); }
  auto setCookie(const string& name, const string& value = "") -> void { _cookie.set(name, value); }

  auto get(const string& name) const -> string { return _get.get(name); }
  auto setGet(const string& name, const string& value = "") -> void { _get.set(name, value); }

  auto post(const string& name) const -> string { return _post.get(name); }
  auto setPost(const string& name, const string& value = "") -> void { _post.set(name, value); }

//private:
  bool _ipv6 = false;
  string _ip;
  RequestType _requestType = RequestType::None;
  string _path;
  httpVariables _cookie;
  httpVariables _get;
  httpVariables _post;
};

auto httpRequest::head(const function<bool (const uint8_t*, unsigned)>& callback) const -> bool {
  if(!callback) return false;
  string output;

  string request = path();
  if(_get.size()) {
    request.append("?");
    for(auto& get : _get) {
      request.append(get.name, "=", get.value, "&");
    }
    request.rtrim("&", 1L);
  }

  switch(requestType()) {
  case RequestType::Head: output.append("HEAD ", request, " HTTP/1.1\r\n"); break;
  case RequestType::Get : output.append("GET ",  request, " HTTP/1.1\r\n"); break;
  case RequestType::Post: output.append("POST ", request, " HTTP/1.1\r\n"); break;
  default: return false;
  }

  for(auto& header : _header) {
    output.append(header.name, ": ", header.value, "\r\n");
  }
  output.append("\r\n");

  return callback(output.binary(), output.size());
}

auto httpRequest::setHead() -> bool {
  lstring headers = _head.split("\n");
  string request = headers.takeFirst().rtrim("\r");
  string requestHost;

       if(request.iendsWith(" HTTP/1.0")) request.irtrim(" HTTP/1.0", 1L);
  else if(request.iendsWith(" HTTP/1.1")) request.irtrim(" HTTP/1.1", 1L);
  else return false;

       if(request.ibeginsWith("HEAD ")) request.iltrim("HEAD ", 1L), setRequestType(RequestType::Head);
  else if(request.ibeginsWith("GET " )) request.iltrim("GET ",  1L), setRequestType(RequestType::Get );
  else if(request.ibeginsWith("POST ")) request.iltrim("POST ", 1L), setRequestType(RequestType::Post);
  else return false;

  //decode absolute URIs
  request.strip().iltrim("http://", 1L);
  if(!request.beginsWith("/")) {
    lstring components = request.split("/", 1L);
    requestHost = components(0);
    request = {"/", components(1)};
  }

  lstring components = request.split("?", 1L);
  setPath(components(0));

  if(auto queryString = components(1)) {
    for(auto& block : queryString.split("&")) {
      lstring variable = block.split("=", 1L);
      if(variable(0)) setGet(variable(0), variable(1));
    }
  }

  for(auto& header : headers) {
    if(header.beginsWith(" ") || header.beginsWith("\t")) continue;
    auto part = header.split(":", 1L).strip();
    if(!part[0] || part.size() != 2) continue;
    appendHeader(part[0], part[1]);

    if(part[0].iequals("Cookie")) {
      for(auto& block : part[1].split(";")) {
        lstring variable = block.split("=", 1L).strip();
        variable(1).trim("\"", "\"");
        if(variable(0)) setCookie(variable(0), variable(1));
      }
    }
  }

  if(requestHost) setHeader("Host", requestHost);  //request URI overrides host header
  return true;
}

auto httpRequest::body(const function<bool (const uint8_t*, unsigned)>& callback) const -> bool {
  if(!callback) return false;

  if(_body) {
    return callback(_body.binary(), _body.size());
  }

  return true;
}

auto httpRequest::setBody() -> bool {
  if(requestType() == RequestType::Post) {
    if(header("Content-Type").iequals("application/x-www-form-urlencoded")) {
      for(auto& block : _body.split("&")) {
        lstring variable = block.rtrim("\r").split("=", 1L);
        if(variable(0)) setPost(variable(0), variable(1));
      }
    }
  }

  return true;
}

}

#endif
