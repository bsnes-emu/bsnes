#pragma once

#include <nall/service.hpp>
#include <nall/http/role.hpp>

namespace nall::HTTP {

struct Server : Role, service {
  auto open(uint port = 8080, const string& serviceName = "", const string& command = "") -> bool;
  auto main(const function<Response (Request&)>& function = {}) -> void;
  auto scan() -> string;
  auto close() -> void;
  ~Server() { close(); }

private:
  function<Response (Request&)> callback;
  std::atomic<int> connections{0};

  int fd4 = -1;
  int fd6 = -1;
  struct sockaddr_in addrin4 = {0};
  struct sockaddr_in6 addrin6 = {0};

  auto ipv4() const -> bool { return fd4 >= 0; }
  auto ipv6() const -> bool { return fd6 >= 0; }

  auto ipv4_close() -> void { if(fd4 >= 0) ::close(fd4); fd4 = -1; }
  auto ipv6_close() -> void { if(fd6 >= 0) ::close(fd6); fd6 = -1; }

  auto ipv4_scan() -> bool;
  auto ipv6_scan() -> bool;
};

}
