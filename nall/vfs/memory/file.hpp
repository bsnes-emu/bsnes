#pragma once

namespace nall::vfs::memory {

struct file : vfs::file {
  ~file() { delete[] _data; }

  static auto open(const void* data, uintmax size) -> vfs::shared::file {
    auto instance = shared_pointer<file>{new file};
    instance->_open((const uint8_t*)data, size);
    return instance;
  }

  auto size() const -> uintmax override { return _size; }
  auto offset() const -> uintmax override { return _offset; }

  auto seek(intmax offset, index mode) -> void override {
    if(mode == index::absolute) _offset = (uintmax)offset;
    if(mode == index::relative) _offset += (intmax)offset;
  }

  auto read() -> uint8_t override {
    if(_offset >= _size) return 0x00;
    return _data[_offset++];
  }

  auto write(uint8_t data) -> void override {
    if(_offset >= _size) return;
    _data[_offset++] = data;
  }

private:
  file() = default;
  file(const file&) = delete;
  auto operator=(const file&) -> file& = delete;

  auto _open(const uint8_t* data, uintmax size) -> void {
    _size = size;
    _data = new uint8_t[size];
    nall::memory::copy(_data, data, size);
  }

  uint8_t* _data = nullptr;
  uintmax _size = 0;
  uintmax _offset = 0;
};

}
