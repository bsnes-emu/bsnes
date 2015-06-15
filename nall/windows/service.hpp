#ifndef NALL_WINDOWS_SERVICE_HPP
#define NALL_WINDOWS_SERVICE_HPP

namespace nall {

struct service {
  explicit operator bool() const { return false; }
  auto command(const string& name, const string& command) -> bool { return false; }
  auto receive() -> string { return ""; }
  auto name() const -> string { return ""; }
  auto stop() const -> bool { return false; }
};

}

#endif
