#pragma once

#include <nall/stream/stream.hpp>
#include <nall/vector.hpp>

namespace nall {

struct vectorstream : stream {
  using stream::read;
  using stream::write;

  vectorstream(vector<uint8>& memory) : memory(memory), pwritable(true) {}
  vectorstream(const vector<uint8>& memory) : memory((vector<uint8>&)memory), pwritable(false) {}

  auto seekable() const -> bool { return true; }
  auto readable() const -> bool { return true; }
  auto writable() const -> bool { return pwritable; }
  auto randomaccess() const -> bool { return true; }

  auto data() const -> uint8* { return memory.data(); }
  auto size() const -> uint { return memory.size(); }
  auto offset() const -> uint { return poffset; }
  auto seek(unsigned offset) const -> void { poffset = offset; }

  auto read() const -> uint8 { return memory[poffset++]; }
  auto write(uint8 data) const -> void { memory[poffset++] = data; }

  auto read(uint offset) const -> uint8 { return memory[offset]; }
  auto write(uint offset, uint8 data) const -> void { memory[offset] = data; }

protected:
  vector<uint8>& memory;
  mutable uint poffset = 0;
  mutable bool pwritable = false;
};

}
