#pragma once

#include <nall/filemap.hpp>

namespace nall {

struct mmapstream : stream {
  using stream::read;
  using stream::write;

  mmapstream(const string& filename) {
    pmmap.open(filename, filemap::mode::readwrite);
    pwritable = pmmap.open();
    if(!pwritable) pmmap.open(filename, filemap::mode::read);
    pdata = pmmap.data();
    poffset = 0;
  }

  auto seekable() const -> bool { return true; }
  auto readable() const -> bool { return true; }
  auto writable() const -> bool { return pwritable; }
  auto randomaccess() const -> bool { return true; }

  auto size() const -> uint { return pmmap.size(); }
  auto offset() const -> uint { return poffset; }
  auto seek(uint offset) const -> void { poffset = offset; }

  auto read() const -> uint8_t { return pdata[poffset++]; }
  auto write(uint8_t data) const -> void { pdata[poffset++] = data; }

  auto read(uint offset) const -> uint8_t { return pdata[offset]; }
  auto write(uint offset, uint8_t data) const -> void { pdata[offset] = data; }

private:
  mutable filemap pmmap;
  mutable uint8_t* pdata = nullptr;
  mutable uint poffset = 0;
  mutable bool pwritable = false;
};

}
