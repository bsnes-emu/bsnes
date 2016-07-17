//effective addressing

//encoding:
//  d7-d5: mode
//  d4-d2: register
//  d1-d0: size (0 = byte, 1 = word, 2 = long)

auto M68K::address(uint8 ea) -> uint32 {
  switch(ea) {

  //data register direct
  case 0b000'000'00 ... 0b000'111'11:
    return r.d(ea >> 2);

  //address register direct
  case 0b001'000'00 ... 0b001'111'11:
    return r.a(ea >> 2);

  //address register indirect
  case 0b010'000'00 ... 0b010'111'11:
    return r.a(ea >> 2);

  //address register indirect with post-increment
  case 0b011'000'00 ... 0b011'111'11:
    return r.a(ea >> 2);

  //address register indirect with pre-decrement
  case 0b100'000'00 ... 0b100'111'11:
    return r.a(ea >> 2);

  //address register indirect with displacement
  case 0b101'000'00 ... 0b101'111'11:
    return r.a(ea >> 2) + (int16)readPC();

  //address register indirect with index
  case 0b110'000'00 ... 0b110'111'11: {
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return r.a(ea >> 2) + index + (int8)word;
  }

  //absolute short indirect
  case 0b111'000'00 ... 0b111'000'11:
    return (int16)readPC();

  //absolute long indirect
  case 0b111'001'00 ... 0b111'001'11: {
    uint32 address = readPC() << 16;
    return address | readPC() <<  0;
  }

  //program counter indirect with displacement
  case 0b111'010'00 ... 0b111'010'11: {
    auto base = r.pc;
    return base + (int16)readPC();
  }

  //program counter indirect with index
  case 0b111'011'00 ... 0b111'011'11: {
    auto base = r.pc;
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return base + index + (int8)word;
  }

  //immediate byte
  case 0b111'100'00:
    return (uint8)readPC();

  //immediate word
  case 0b111'100'01:
    return readPC();

  //immediate long
  case 0b111'100'10: {
    uint32 address = readPC() << 16;
    return address | readPC() <<  0;
  }

  //invalid
  default:
    return 0;

  }
}

template<uint Size> auto M68K::read(EA ea) -> uint32 {
  switch(ea.mode) {

  case 0: {  //data register direct
    return clip<Size>(r.d(ea.reg));
  }

  case 1: {  //address register direct
    return clip<Size>(r.a(ea.reg));
  }

  case 2: {  //address register indirect
    auto& address = r.a(ea.reg);
    return read<Size>(address);
  }

  case 3: {  //address register indirect with post-increment
    auto& address = r.a(ea.reg);
    auto data = read<Size>(address);
    address += Size == Long ? 4 : 2;
    return data;
  }

  case 4: {  //address register indirect with pre-decrement
    auto& address = r.a(ea.reg);
    address -= Size == Long ? 4 : 2;
    return read<Size>(address);
  }

  }
}

template<uint Size> auto M68K::write(EA ea, uint32 data) -> void {
  switch(ea.mode) {

  case 0: {  //data register direct
    r.d(ea.reg) = data;
    return;
  }

  case 1: {  //address register direct
    r.a(ea.reg) = data;
    return;
  }

  }
}

auto M68K::read(uint8 ea) -> uint32 {
  switch(ea) {

  //data register direct
  case 0b000'000'00 ... 0b000'111'11:
    return r.d(ea >> 2);

  //address register direct
  case 0b001'000'00 ... 0b001'111'11:
    return r.a(ea >> 2);

  //address register indirect
  case 0b010'000'00 ... 0b010'111'11: {
    auto address = r.a(ea >> 2);
    return readAbsolute(ea, address);
  }

  //address register indirect with post-increment
  case 0b011'000'00 ... 0b011'111'11: {
    auto& address = r.a(ea >> 2);
    auto data = readAbsolute(ea, address);
    address += 2 + (ea & 2);
    return data;
  }

  //address register indirect with pre-decrement
  case 0b100'000'00 ... 0b100'111'11: {
    auto& address = r.a(ea >> 2);
    address -= 2 + (ea & 2);
    return readAbsolute(ea, address);
  }

  //address register indirect with displacement
  case 0b101'000'00 ... 0b101'111'11:
    return readAbsolute(ea, r.a(ea >> 2) + (int16)readPC());

  //address register indirect with index
  case 0b110'000'00 ... 0b110'111'11: {
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return readAbsolute(ea, r.a(ea >> 2) + index + (int8)word);
  }

  //absolute short indirect
  case 0b111'000'00 ... 0b111'000'11:
    return readAbsolute(ea, (int16)readPC());

  //absolute long indirect
  case 0b111'001'00 ... 0b111'001'11: {
    uint32 address = readPC() << 16;
    return readAbsolute(ea, address | readPC());
  }

  //program counter indirect with displacement
  case 0b111'010'00 ... 0b111'010'11: {
    auto base = r.pc;
    return readAbsolute(ea, base + (int16)readPC());
  }

  //program counter indirect with index
  case 0b111'011'00 ... 0b111'011'11: {
    auto base = r.pc;
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return readAbsolute(ea, base + index + (int8)word);
  }

  //immediate byte
  case 0b111'100'00:
    return (uint8)readPC();

  //immediate word
  case 0b111'100'01:
    return readPC();

  //immediate long
  case 0b111'100'10: {
    uint32 address = readPC() << 16;
    return address | readPC() <<  0;
  }

  //invalid
  default:
    return 0;

  }
}

auto M68K::write(uint8 ea, uint32 data) -> void {
  switch(ea) {

  //data register direct
  case 0b000'000'00 ... 0b000'111'11:
    r.d(ea >> 2) = data;
    return;

  //address register direct
  case 0b001'000'00 ... 0b001'111'11:
    r.a(ea >> 2) = data;
    return;

  //address register indirect
  case 0b010'000'00 ... 0b010'111'11: {
    auto address = r.a(ea >> 2);
    return writeAbsolute(ea, address, data);
  }

  //address register indirect with post-increment
  case 0b011'000'00 ... 0b011'111'11: {
    auto& address = r.a(ea >> 2);
    writeAbsolute(ea, address, data);
    address += 2 + (ea & 2);
    return;
  }

  //address register indirect with pre-decrement
  case 0b100'000'00 ... 0b100'111'11: {
    auto& address = r.a(ea >> 2);
    address -= 2 + (ea & 2);
    return writeAbsolute(ea, address, data);
  }

  //address register indirect with displacement
  case 0b101'000'00 ... 0b101'111'11:
    return writeAbsolute(ea, r.a(ea >> 2) + (int16)readPC(), data);

  //address register indirect with index
  case 0b110'000'00 ... 0b110'111'11: {
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return writeAbsolute(ea, r.a(ea >> 2) + index + (int8)word, data);
  }

  //absolute short indirect
  case 0b111'000'00 ... 0b111'000'11:
    return writeAbsolute(ea, (int16)readPC(), data);

  //absolute long indirect
  case 0b111'001'00 ... 0b111'001'11: {
    uint32 address = readPC() << 16;
    return writeAbsolute(ea, address | readPC(), data);
  }

  //program counter indirect with displacement
  case 0b111'010'00 ... 0b111'010'11: {
    auto base = r.pc;
    return writeAbsolute(ea, base + (int16)readPC(), data);
  }

  //program counter indirect with index
  case 0b111'011'00 ... 0b111'011'11: {
    auto base = r.pc;
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    return writeAbsolute(ea, base + index + (int8)word, data);
  }

  }
}

auto M68K::modify(uint8 ea, uint32 data, const function<uint32 (uint32, uint32)>& op) -> uint32 {
  switch(ea) {

  //data register direct
  case 0b000'000'00 ... 0b000'111'11: {
    auto& address = r.d(ea >> 2);
    return address = op(address, data);
  }

  //address register direct
  case 0b001'000'00 ... 0b001'111'11: {
    auto& address = r.a(ea >> 2);
    return address = op(address, data);
  }

  //address register indirect
  case 0b010'000'00 ... 0b010'111'11: {
    auto address = r.a(ea >> 2);
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //address register indirect with post-increment
  case 0b011'000'00 ... 0b011'111'11: {
    auto& address = r.a(ea >> 2);
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    address += 2 + (ea & 2);
    return memory;
  }

  //address register indirect with pre-decrement
  case 0b100'000'00 ... 0b100'111'11: {
    auto& address = r.a(ea >> 2);
    address -= 2 + (ea & 2);
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //address register indirect with displacement
  case 0b101'000'00 ... 0b101'111'11: {
    auto address = r.a(ea >> 2) + (int16)readPC();
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //address register indirect with index
  case 0b110'000'00 ... 0b110'111'11: {
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x800) index = (int16)index;
    auto address = r.a(ea >> 2) + index + (int8)word;
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //absolute short indirect
  case 0b111'000'00 ... 0b111'000'11: {
    auto address = (int16)readPC();
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //absolute long indirect
  case 0b111'001'00 ... 0b111'001'11: {
    auto word = readPC();
    uint32 address = word << 16 | readPC();
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //program counter indirect with displacement
  case 0b111'010'00 ... 0b111'010'11: {
    auto address = r.pc;
    address += (int16)readPC();
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //program counter indirect with index
  case 0b111'011'00 ... 0b111'011'11: {
    auto address = r.pc;
    auto word = readPC();
    auto index = word & 0x8000 ? r.a(word >> 12) : r.d(word >> 12);
    if(word & 0x8000) index = (int16)index;
    address += index + (int8)word;
    auto memory = readAbsolute(ea, address);
    writeAbsolute(ea, address, memory = op(memory, data));
    return memory;
  }

  //immediate byte
  case 0b111'100'00:
    return op((uint8)readPC(), data);

  //immediate word
  case 0b111'100'01:
    return op(readPC(), data);

  //immediate long
  case 0b111'100'10: {
    uint32 immediate = readPC() << 16;
    immediate |= readPC();
    return op(immediate, data);
  }

  }
}

auto M68K::flush(uint8 ea, uint32 address) -> void {
  //address register indirect with post-increment
  //address register indirect with pre-decrement
  if(ea >= 0b011'000'00 && ea <= 0b100'111'11) {
    r.a(ea >> 2) = address;
  }
}
