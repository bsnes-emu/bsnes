#ifndef NALL_STREAM_MEMORY_HPP
#define NALL_STREAM_MEMORY_HPP

#include <nall/stream/stream.hpp>

namespace nall {

struct memorystream : stream {
  using stream::read;
  using stream::write;

  memorystream() = default;

  memorystream(uint8* data, uint size) {
    pdata = data;
    psize = size;
    pwritable = true;
  }

  memorystream(const uint8* data, uint size) {
    pdata = (uint8*)data;
    psize = size;
    pwritable = false;
  }

  auto seekable() const -> bool { return true; }
  auto readable() const -> bool { return true; }
  auto writable() const -> bool { return pwritable; }
  auto randomaccess() const -> bool { return true; }

  auto data() const -> uint8* { return pdata; }
  auto size() const -> uint { return psize; }
  auto offset() const -> uint { return poffset; }
  auto seek(uint offset) const -> void { poffset = offset; }

  auto read() const -> uint8 { return pdata[poffset++]; }
  auto write(uint8 data) const -> void { pdata[poffset++] = data; }

  auto read(uint offset) const -> uint8 { return pdata[offset]; }
  auto write(uint offset, uint8 data) const -> void { pdata[offset] = data; }

protected:
  mutable uint8* pdata = nullptr;
  mutable uint psize = 0;
  mutable uint poffset = 0;
  mutable bool pwritable = false;
};

}

#endif
