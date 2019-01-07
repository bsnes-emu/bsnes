template<> auto TLCS900H::fetch<Byte>() -> Byte {
  return 0x00;
}

template<> auto TLCS900H::fetch<Word>() -> Word {
  uint16 data = fetch<Byte>();
  return data | fetch<Byte>() << 8;
}

template<> auto TLCS900H::fetch<uint24>() -> uint24 {
  uint24 data  = fetch<Byte>();
         data |= fetch<Byte>() << 8;
  return data |= fetch<Byte>() << 16;
}

template<> auto TLCS900H::fetch<Long>() -> Long {
  uint32 data  = fetch<Byte>();
         data |= fetch<Byte>() << 8;
         data |= fetch<Byte>() << 16;
  return data |= fetch<Byte>() << 24;
}

template<> auto TLCS900H::fetch< int8>() ->  int8 { return ( int8)fetch< uint8>(); }
template<> auto TLCS900H::fetch<int16>() -> int16 { return (int16)fetch<uint16>(); }
template<> auto TLCS900H::fetch<int24>() -> int24 { return (int24)fetch<uint24>(); }
template<> auto TLCS900H::fetch<int32>() -> int32 { return (int32)fetch<uint32>(); }

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
