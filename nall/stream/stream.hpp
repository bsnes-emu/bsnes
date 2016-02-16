#pragma once

namespace nall {

struct stream {
  stream() = default;
  virtual ~stream() = default;

  stream(const stream&) = delete;
  auto operator=(const stream&) -> stream& = delete;

  virtual auto seekable() const -> bool = 0;
  virtual auto readable() const -> bool = 0;
  virtual auto writable() const -> bool = 0;
  virtual auto randomaccess() const -> bool = 0;

  virtual auto data() const -> uint8_t* { return nullptr; }
  virtual auto size() const -> uint = 0;
  virtual auto offset() const -> uint = 0;
  virtual auto seek(uint offset) const -> void = 0;

  virtual auto read() const -> uint8_t = 0;
  virtual auto write(uint8_t data) const -> void = 0;

  virtual auto read(uint) const -> uint8_t { return 0; }
  virtual auto write(uint, uint8_t) const -> void {}

  explicit operator bool() const {
    return size();
  }

  auto empty() const -> bool {
    return size() == 0;
  }

  auto end() const -> bool {
    return offset() >= size();
  }

  auto readl(uint length = 1) const -> uintmax {
    uintmax data = 0, shift = 0;
    while(length--) { data |= read() << shift; shift += 8; }
    return data;
  }

  auto readm(uint length = 1) const -> uintmax {
    uintmax data = 0;
    while(length--) data = (data << 8) | read();
    return data;
  }

  auto read(uint8_t* data, uint length) const -> void {
    while(length--) *data++ = read();
  }

  auto text() const -> string {
    string buffer;
    buffer.resize(size());
    seek(0);
    read((uint8_t*)buffer.get(), size());
    return buffer;
  }

  auto writel(uintmax data, uint length = 1) const -> void {
    while(length--) {
      write(data);
      data >>= 8;
    }
  }

  auto writem(uintmax data, uint length = 1) const -> void {
    uintmax shift = 8 * length;
    while(length--) {
      shift -= 8;
      write(data >> shift);
    }
  }

  auto write(const uint8_t* data, uint length) const -> void {
    while(length--) write(*data++);
  }

  struct byte {
    byte(const stream& s, uint offset) : s(s), offset(offset) {}
    operator uint8_t() const { return s.read(offset); }
    auto operator=(uint8_t data) -> byte& { s.write(offset, data); return *this; }

  private:
    const stream& s;
    const uint offset;
  };

  auto operator[](uint offset) const -> byte {
    return byte(*this, offset);
  }
};

}
