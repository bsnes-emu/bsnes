#pragma once

#include <nall/stream/stream.hpp>

namespace nall {

struct memorystream : stream {
  using stream::read;
  using stream::write;

  memorystream() = default;

  memorystream(uint8_t* data, uint size) {
    pdata = data;
    psize = size;
    pwritable = true;
  }

  memorystream(const uint8_t* data, uint size) {
    pdata = (uint8_t*)data;
    psize = size;
    pwritable = false;
  }

  auto seekable() const -> bool { return true; }
  auto readable() const -> bool { return true; }
  auto writable() const -> bool { return pwritable; }
  auto randomaccess() const -> bool { return true; }

  auto data() const -> uint8_t* { return pdata; }
  auto size() const -> uint { return psize; }
  auto offset() const -> uint { return poffset; }
  auto seek(uint offset) const -> void { poffset = offset; }

  auto read() const -> uint8_t { return pdata[poffset++]; }
  auto write(uint8_t data) const -> void { pdata[poffset++] = data; }

  auto read(uint offset) const -> uint8_t { return pdata[offset]; }
  auto write(uint offset, uint8_t data) const -> void { pdata[offset] = data; }

protected:
  mutable uint8_t* pdata = nullptr;
  mutable uint psize = 0;
  mutable uint poffset = 0;
  mutable bool pwritable = false;
};

}
