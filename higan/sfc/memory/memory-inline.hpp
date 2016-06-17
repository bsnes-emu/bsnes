//Memory

auto Memory::size() const -> uint { return 0; }

//StaticRAM

StaticRAM::StaticRAM(uint size) : _size(size) { _data = new uint8[_size]; }
StaticRAM::~StaticRAM() { delete[] _data; }

auto StaticRAM::data() -> uint8* { return _data; }
auto StaticRAM::size() const -> uint { return _size; }

auto StaticRAM::read(uint24 addr, uint8) -> uint8 { return _data[addr]; }
auto StaticRAM::write(uint24 addr, uint8 data) -> void { _data[addr] = data; }
auto StaticRAM::operator[](uint24 addr) -> uint8& { return _data[addr]; }
auto StaticRAM::operator[](uint24 addr) const -> const uint8& { return _data[addr]; }

//MappedRAM

auto MappedRAM::reset() -> void {
  delete[] _data;
  _data = nullptr;
  _size = 0;
  _writeProtect = false;
}

auto MappedRAM::map(uint8* source, uint length) -> void {
  reset();
  _data = source;
  _size = _data ? length : 0;
}

auto MappedRAM::copy(const stream& memory) -> void {
  if(_data) delete[] _data;
  //round size up to multiple of 256-bytes
  _size = (memory.size() & ~255) + ((bool)(memory.size() & 255) << 8);
  _data = new uint8[_size]();
  memory.read((uint8_t*)_data, memory.size());
}

auto MappedRAM::read(const stream& memory) -> void {
  memory.read((uint8_t*)_data, min(memory.size(), _size));
}

auto MappedRAM::writeProtect(bool writeProtect) -> void { _writeProtect = writeProtect; }
auto MappedRAM::data() -> uint8* { return _data; }
auto MappedRAM::size() const -> uint { return _size; }

auto MappedRAM::read(uint24 addr, uint8) -> uint8 { return _data[addr]; }
auto MappedRAM::write(uint24 addr, uint8 data) -> void { if(!_writeProtect) _data[addr] = data; }
auto MappedRAM::operator[](uint24 addr) const -> const uint8& { return _data[addr]; }

//Bus

auto Bus::mirror(uint addr, uint size) -> uint {
  if(size == 0) return 0;
  uint base = 0;
  uint mask = 1 << 23;
  while(addr >= size) {
    while(!(addr & mask)) mask >>= 1;
    addr -= mask;
    if(size > mask) {
      size -= mask;
      base += mask;
    }
    mask >>= 1;
  }
  return base + addr;
}

auto Bus::reduce(uint addr, uint mask) -> uint {
  while(mask) {
    uint bits = (mask & -mask) - 1;
    addr = ((addr >> 1) & ~bits) | (addr & bits);
    mask = (mask & (mask - 1)) >> 1;
  }
  return addr;
}

auto Bus::read(uint24 addr, uint8 data) -> uint8 {
  data = reader[lookup[addr]](target[addr], data);
  if(cheat.enable()) {
    if(auto result = cheat.find(addr, data)) return result();
  }
  return data;
}

auto Bus::write(uint24 addr, uint8 data) -> void {
  return writer[lookup[addr]](target[addr], data);
}
