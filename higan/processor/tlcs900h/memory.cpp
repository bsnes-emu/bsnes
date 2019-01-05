template<> auto TLCS900H::fetch<uint8>() -> uint8 {
  return 0x00;
}

template<> auto TLCS900H::fetch<uint16>() -> uint16 {
  uint16 data = fetch<uint8>();
  return data | fetch<uint8>() << 8;
}

template<> auto TLCS900H::fetch<uint24>() -> uint24 {
  uint24 data  = fetch<uint8>();
         data |= fetch<uint8>() << 8;
  return data |= fetch<uint8>() << 16;
}

template<> auto TLCS900H::fetch<uint32>() -> uint32 {
  uint32 data  = fetch<uint8>();
         data |= fetch<uint8>() << 8;
         data |= fetch<uint8>() << 16;
  return data |= fetch<uint8>() << 24;
}

//

#define XSP r.xsp.l.l0

template<> auto TLCS900H::push<Byte>(Byte data) -> void {
  write(--XSP, data);
}

template<> auto TLCS900H::push<Word>(Word data) -> void {
  write(--XSP, data >> 0);
  write(--XSP, data >> 8);
}

template<> auto TLCS900H::push<Long>(Long data) -> void {
  write(--XSP, data >>  0);
  write(--XSP, data >>  8);
  write(--XSP, data >> 16);
  write(--XSP, data >> 24);
}

//

template<> auto TLCS900H::pop<Byte>() -> Byte {
  return read(XSP++);
}

template<> auto TLCS900H::pop<Word>() -> Word {
  uint16 data = read(XSP++) << 0;
  return data | read(XSP++) << 8;
}

template<> auto TLCS900H::pop<Long>() -> Long {
  uint32 data  = read(XSP++) <<  0;
         data |= read(XSP++) <<  8;
         data |= read(XSP++) << 16;
  return data |= read(XSP++) << 24;
}

#undef XSP

//

template<> auto TLCS900H::read<Byte>(Memory memory) -> uint8 {
  uint32 address = memory.value;
  return read(address);
}

template<> auto TLCS900H::read<Word>(Memory memory) -> uint16 {
  uint32 address = memory.value;
  uint16 data = read(address + 0) << 0;
  return data | read(address + 1) << 8;
}

template<> auto TLCS900H::read<Long>(Memory memory) -> uint32 {
  uint32 address = memory.value;
  uint32 data  = read(address + 0) <<  0;
         data |= read(address + 1) <<  8;
         data |= read(address + 2) << 16;
  return data |= read(address + 3) << 24;
}

template<> auto TLCS900H::write<Byte>(Memory memory, uint8 data) -> void {
  uint32 address = memory.value;
  write(address + 0, data >> 0);
}

template<> auto TLCS900H::write<Word>(Memory memory, uint16 data) -> void {
  uint32 address = memory.value;
  write(address + 0, data >> 0);
  write(address + 1, data >> 8);
}

template<> auto TLCS900H::write<Long>(Memory memory, uint32 data) -> void {
  uint32 address = memory.value;
  write(address + 0, data >>  0);
  write(address + 1, data >>  8);
  write(address + 2, data >> 16);
  write(address + 3, data >> 24);
}
