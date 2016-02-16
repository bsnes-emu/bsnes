//Memory

auto Memory::size() const -> uint { return 0; }

//StaticRAM

StaticRAM::StaticRAM(uint n) : size_(n) { data_ = new uint8[size_]; }
StaticRAM::~StaticRAM() { delete[] data_; }

auto StaticRAM::data() -> uint8* { return data_; }
auto StaticRAM::size() const -> uint { return size_; }

auto StaticRAM::read(uint24 addr, uint8) -> uint8 { return data_[addr]; }
auto StaticRAM::write(uint24 addr, uint8 data) -> void { data_[addr] = data; }
auto StaticRAM::operator[](uint24 addr) -> uint8& { return data_[addr]; }
auto StaticRAM::operator[](uint24 addr) const -> const uint8& { return data_[addr]; }

//MappedRAM

auto MappedRAM::reset() -> void {
  if(data_) {
    delete[] data_;
    data_ = nullptr;
  }
  size_ = 0;
  write_protect_ = false;
}

auto MappedRAM::map(uint8* source, uint length) -> void {
  reset();
  data_ = source;
  size_ = data_ ? length : 0;
}

auto MappedRAM::copy(const stream& memory) -> void {
  if(data_) delete[] data_;
  //round size up to multiple of 256-bytes
  size_ = (memory.size() & ~255) + ((bool)(memory.size() & 255) << 8);
  data_ = new uint8[size_]();
  memory.read((uint8_t*)data_, memory.size());
}

auto MappedRAM::read(const stream& memory) -> void {
  memory.read((uint8_t*)data_, min(memory.size(), size_));
}

auto MappedRAM::write_protect(bool status) -> void { write_protect_ = status; }
auto MappedRAM::data() -> uint8* { return data_; }
auto MappedRAM::size() const -> uint { return size_; }

auto MappedRAM::read(uint24 addr, uint8) -> uint8 { return data_[addr]; }
auto MappedRAM::write(uint24 addr, uint8 data) -> void { if(!write_protect_) data_[addr] = data; }
auto MappedRAM::operator[](uint24 addr) const -> const uint8& { return data_[addr]; }

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
