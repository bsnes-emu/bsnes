M68K::EA::EA(M68K* self, uint2 size, uint3 mode, uint3 reg) : self(self), size(size), mode(mode), reg(reg) {
  if(this->mode == 7) this->mode += this->reg;  //speed hack: convert cases {7; 0-4} to {8-12} for switch jump table
  address = fetch();
}

M68K::EA::~EA() {
  flush();
}

auto M68K::EA::pc() -> uint32& { return self->r.pc; }
auto M68K::EA::d(uint3 reg) -> uint32& { return self->r.d(reg); }
auto M68K::EA::a(uint3 reg) -> uint32& { return self->r.a(reg); }
auto M68K::EA::readPC(uint2 size) -> uint32 { return self->readPC(size); }
auto M68K::EA::read(uint32 addr) -> uint32 { return self->readAbsolute(size, addr); }
auto M68K::EA::write(uint32 addr, uint32 data) -> void { return self->writeAbsolute(size, addr, data); }

auto M68K::EA::fetch() -> uint32 {
  switch(mode) {

  //data register direct
  case 0: return d(reg);

  //address register direct
  case 1: return a(reg);

  //address register indirect
  case 2: return a(reg);

  //address register indirect with post-increment
  case 3: return a(reg);

  //address register indirect with pre-decrement
  case 4: return a(reg);

  //address register with displacement
  case 5: return a(reg) + (int16)readPC(Word);

  //address register with index
  case 6: {
    auto word = readPC(Word);
    auto index = word & 0x8000 ? a(word >> 12) : d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return a(reg) + index + (int8)word;
  }

  //absolute short
  case 7: return (int16)readPC(Word);

  //absolute long
  case 8: return readPC(Long);

  //program counter with displacement
  case 9: {
    auto base = pc();
    return base + (int16)readPC(Word);
  }

  //program counter with index
  case 10: {
    auto base = pc();
    auto word = readPC(Word);
    auto index = word & 0x8000 ? a(word >> 12) : d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return base + index + (int8)word;
  }

  //immediate
  case 11: {
    if(size == Byte) return (uint8)readPC(Word);
    if(size == Word) return readPC(Word);
    if(size == Long) return readPC(Long);
  }

  }

  return 0;
}

auto M68K::EA::read() -> uint32 {
  switch(mode) {
  case  0: return address;
  case  1: return address;
  case  2: return read(address);
  case  3: {
    auto data = read(address);
    address += size == Long ? 4 : 2;
    return data;
  }
  case  4: {
    address -= size == Long ? 4 : 2;
    return read(address);
  }
  case  5: return read(address);
  case  6: return read(address);
  case  7: return read(address);
  case  8: return read(address);
  case  9: return read(address);
  case 10: return read(address);
  case 11: return address;
  }

  return 0;
}

auto M68K::EA::write(uint32 data) -> void {
  switch(mode) {
  case  0: address = data; return;
  case  1: address = data; return;
  case  2: return write(address, data);
  case  3: {
    write(address, data);
    address += size == Long ? 4 : 2;
    return;
  }
  case  4: {
    address -= size == Long ? 4 : 2;
    return write(address, data);
  }
  case  5: return write(address, data);
  case  6: return write(address, data);
  case  7: return write(address, data);
  case  8: return write(address, data);
  case  9: return write(address, data);
  case 10: return write(address, data);
  case 11: address = data; return;
  }
}

auto M68K::EA::flush() -> void {
  //address register indirect with post-increment
  if(mode == 3) a(reg) = address;

  //address register indirect with pre-decrement
  if(mode == 4) a(reg) = address;

  mode = 15;
}
