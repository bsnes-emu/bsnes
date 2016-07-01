#pragma once

#include <nall/range.hpp>
#include <nall/shared-pointer.hpp>

namespace nall { namespace vfs {

struct file {
  enum class mode : uint { read, write, modify, create };
  enum class index : uint { absolute, relative };

  virtual ~file() = default;

  virtual auto size() const -> uintmax_t = 0;
  virtual auto offset() const -> uintmax_t = 0;

  virtual auto seek(intmax_t offset, index = index::absolute) -> void = 0;
  virtual auto read() -> uint8_t = 0;
  virtual auto write(uint8_t data) -> void = 0;
  virtual auto flush() -> void {}

  auto end() const -> bool {
    return offset() >= size();
  }

  auto read(void* vdata, uintmax_t bytes) -> void {
    auto data = (uint8_t*)vdata;
    while(bytes--) *data++ = read();
  }

  auto readl(uint bytes) -> uintmax_t {
    uintmax_t data = 0;
    for(auto n : range(bytes)) data |= (uintmax_t)read() << n * 8;
    return data;
  }

  auto readm(uint bytes) -> uintmax_t {
    uintmax_t data = 0;
    for(auto n : range(bytes)) data = data << 8 | read();
    return data;
  }

  auto reads() -> string {
    string s;
    s.resize(size());
    read(s.get<uint8_t>(), s.size());
    return s;
  }

  auto write(const void* vdata, uintmax_t bytes) -> void {
    auto data = (const uint8_t*)vdata;
    while(bytes--) write(*data++);
  }

  auto writel(uintmax_t data, uint bytes) -> void {
    for(auto n : range(bytes)) write(data), data >>= 8;
  }

  auto writem(uintmax_t data, uint bytes) -> void {
    for(auto n : rrange(bytes)) write(data >> n * 8);
  }

  auto writes(const string& s) -> void {
    write(s.data<uint8_t>(), s.size());
  }
};

}}

namespace nall { namespace vfs { namespace shared {
  using file = shared_pointer<vfs::file>;
}}}

#include <nall/vfs/fs/file.hpp>
#include <nall/vfs/memory/file.hpp>
