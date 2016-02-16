#pragma once

#include <nall/file.hpp>

namespace nall {

struct filestream : stream {
  using stream::read;
  using stream::write;

  filestream(const string& filename) {
    pfile.open(filename, file::mode::readwrite);
    pwritable = pfile.open();
    if(!pwritable) pfile.open(filename, file::mode::read);
  }

  filestream(const string& filename, file::mode mode) {
    pfile.open(filename, mode);
    pwritable = mode == file::mode::write || mode == file::mode::readwrite;
  }

  auto seekable() const -> bool { return true; }
  auto readable() const -> bool { return true; }
  auto writable() const -> bool { return pwritable; }
  auto randomaccess() const -> bool { return false; }

  auto size() const -> uint { return pfile.size(); }
  auto offset() const -> uint { return pfile.offset(); }
  auto seek(uint offset) const -> void { pfile.seek(offset); }

  auto read() const -> uint8_t { return pfile.read(); }
  auto write(uint8_t data) const -> void { pfile.write(data); }

private:
  mutable file pfile;
  bool pwritable;
};

}
