#ifndef NALL_HTTP_SERVER_HPP
#define NALL_HTTP_SERVER_HPP

#include <poll.h>
#include <atomic>

#include <nall/service.hpp>
#include <nall/http/role.hpp>

namespace nall {

struct httpServer : httpRole, service {
  inline auto open(unsigned port = 8080, const string& serviceName = "", const string& command = "") -> bool;
  inline auto main(const function<httpResponse (httpRequest&)>& function = {}) -> void;
  inline auto scan() -> string;
  inline auto close() -> void;
  ~httpServer() { close(); }

private:
  signed fd = -1;
  function<httpResponse (httpRequest&)> callback;
  struct sockaddr_in addrin = {0};
  std::atomic<signed> connections{0};
};

auto httpServer::open(unsigned port, const string& serviceName, const string& command) -> bool {
  if(serviceName) {
    if(!service::command(serviceName, command)) return false;
  }

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd < 0) return false;

  {
  #if defined(SO_RCVTIMEO)
  if(settings.timeoutReceive) {
    struct timeval rcvtimeo;
    rcvtimeo.tv_sec  = settings.timeoutReceive / 1000;
    rcvtimeo.tv_usec = settings.timeoutReceive % 1000 * 1000;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &rcvtimeo, sizeof(struct timeval));
  }
  #endif

  #if defined(SO_SNDTIMEO)
  if(settings.timeoutSend) {
    struct timeval sndtimeo;
    sndtimeo.tv_sec  = settings.timeoutSend / 1000;
    sndtimeo.tv_usec = settings.timeoutSend % 1000 * 1000;
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &sndtimeo, sizeof(struct timeval));
  }
  #endif

  #if defined(SO_NOSIGPIPE)  //BSD, OSX
  signed nosigpipe = 1;
  setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &nosigpipe, sizeof(signed));
  #endif

  #if defined(SO_REUSEADDR)  //BSD, Linux, OSX
  signed reuseaddr = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(signed));
  #endif

  #if defined(SO_REUSEPORT)  //BSD, OSX
  signed reuseport = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(signed));
  #endif
  }

  addrin.sin_family = AF_INET;
  addrin.sin_addr.s_addr = htonl(INADDR_ANY);
  addrin.sin_port = htons(port);

  signed result = bind(fd, (struct sockaddr*)&addrin, sizeof(addrin));
  if(result < 0) return close(), false;

  result = listen(fd, SOMAXCONN);  //system-wide limit (per port)
  if(result < 0) return close(), false;

  return true;
}

auto httpServer::main(const function<httpResponse (httpRequest&)>& function) -> void {
  callback = function;
}

auto httpServer::scan() -> string {
  if(auto command = service::receive()) return command;

  if(connections >= settings.connectionLimit) return "busy";

  struct pollfd query = {0};
  query.fd = fd;
  query.events = POLLIN;
  poll(&query, 1, 0);

  if(query.fd == fd && query.revents & POLLIN) {
    ++connections;

    thread::create([&](uintptr_t) {
      thread::detach();

      signed clientfd = -1;
      struct sockaddr_in settings = {0};
      socklen_t socklen = sizeof(sockaddr_in);

      clientfd = accept(fd, (struct sockaddr*)&settings, &socklen);
      if(clientfd < 0) return;

      httpRequest request;
      request._ip = ntohl(settings.sin_addr.s_addr);

      if(download(clientfd, request) && callback) {
        auto response = callback(request);
        upload(clientfd, response);
      } else {
        upload(clientfd, httpResponse());  //"501 Not Implemented"
      }

      ::close(clientfd);
      --connections;
    }, 0, settings.threadStackSize);
  }

  return "ok";
}

auto httpServer::close() -> void {
  if(fd) {
    ::close(fd);
    fd = -1;
  }
}

}

#endif
