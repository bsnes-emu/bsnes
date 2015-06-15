#ifndef NALL_WINDOWS_SHARED_MEMORY_HPP
#define NALL_WINDOWS_SHARED_MEMORY_HPP

namespace nall {

struct shared_memory {
  shared_memory() = default;
  shared_memory(const shared_memory&) = delete;
  auto operator=(const shared_memory&) -> shared_memory& = delete;

  ~shared_memory() {
    reset();
  }

  explicit operator bool() const { return false; }
  auto empty() const -> bool { return true; }
  auto size() const -> unsigned { return 0; }
  auto acquired() const -> bool { return false; }
  auto acquire() -> uint8_t* { return nullptr; }
  auto release() -> void {}
  auto reset() -> void {}
  auto create(const string& name, unsigned size) -> bool { return false; }
  auto remove() -> void {}
  auto open(const string& name, unsigned size) -> bool { return false; }
  auto close() -> void {}
};

}

#endif
