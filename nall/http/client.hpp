#ifndef NALL_HTTP_CLIENT_HPP
#define NALL_HTTP_CLIENT_HPP

#include <nall/http/role.hpp>

namespace nall {

struct httpClient : httpRole {
  inline auto open(const string& hostname, unsigned port = 80) -> bool;
  inline auto upload(const httpRequest& request) -> bool;
  inline auto download(const httpRequest& request) -> httpResponse;
  inline auto close() -> void;
  ~httpClient() { close(); }

private:
  signed fd = -1;
  addrinfo* info = nullptr;
};

auto httpClient::open(const string& hostname, unsigned port) -> bool {
  addrinfo hint = {0};
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_ADDRCONFIG;

  if(getaddrinfo(hostname, string{port}, &hint, &info) != 0) return close(), false;

  fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if(fd < 0) return close(), false;

  if(connect(fd, info->ai_addr, info->ai_addrlen) < 0) return close(), false;
  return true;
}

auto httpClient::upload(const httpRequest& request) -> bool {
  return httpRole::upload(fd, request);
}

auto httpClient::download(const httpRequest& request) -> httpResponse {
  httpResponse response(request);
  httpRole::download(fd, response);
  return response;
}

auto httpClient::close() -> void {
  if(fd) {
    ::close(fd);
    fd = -1;
  }

  if(info) {
    freeaddrinfo(info);
    info = nullptr;
  }
}

}

#endif
