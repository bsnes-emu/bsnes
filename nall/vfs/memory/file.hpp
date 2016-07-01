#pragma once

namespace nall { namespace vfs { namespace memory {

struct file : vfs::file {
  ~file() { delete[] _data; }

  static auto open(const uint8_t* data, uintmax_t size) -> vfs::shared::file {
    auto instance = shared_pointer<file>{new file};
    instance->_open(data, size);
    return instance;
  }

  auto size() const -> uintmax_t override { return _size; }
  auto offset() const -> uintmax_t override { return _offset; }

  auto seek(intmax_t offset, index mode) -> void override {
    if(mode == index::absolute) _offset = (uintmax_t)offset;
    if(mode == index::relative) _offset += (intmax_t)offset;
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

  auto _open(const uint8_t* data, uintmax_t size) -> void {
    _size = size;
    _data = new uint8_t[size];
    nall::memory::copy(_data, data, size);
  }

  uint8_t* _data = nullptr;
  uintmax_t _size = 0;
  uintmax_t _offset = 0;
};

}}}
