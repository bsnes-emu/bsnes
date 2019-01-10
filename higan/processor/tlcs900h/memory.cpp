template<> auto TLCS900H::fetch< uint8>() ->  uint8 {
  return rand();
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

template<> auto TLCS900H::fetch< int8>() ->  int8 { return ( int8)fetch< uint8>(); }
template<> auto TLCS900H::fetch<int16>() -> int16 { return (int16)fetch<uint16>(); }
template<> auto TLCS900H::fetch<int24>() -> int24 { return (int24)fetch<uint24>(); }
template<> auto TLCS900H::fetch<int32>() -> int32 { return (int32)fetch<uint32>(); }

template<typename T> auto TLCS900H::fetchRegister() -> Register<T> { return Register<T>{fetch<uint8>()}; }
template<typename T, typename U> auto TLCS900H::fetchMemory() -> Memory<T> { return Memory<T>{fetch<U>()}; }
template<typename T> auto TLCS900H::fetchImmediate() -> Immediate<T> { return Immediate<T>{fetch<T>()}; }

//

#define XSP r.xsp.l.l0

template<typename T> auto TLCS900H::pop(T data) -> void {
  auto value = typename T::type();
  if constexpr(T::bits >=  8) value.byte(0) = read(XSP++);
  if constexpr(T::bits >= 16) value.byte(1) = read(XSP++);
  if constexpr(T::bits >= 24) value.byte(2) = read(XSP++);
  if constexpr(T::bits >= 32) value.byte(3) = read(XSP++);
  store(data, value);
}

template<typename T> auto TLCS900H::push(T data) -> void {
  auto value = load(data);
  if constexpr(T::bits >=  8) write(--XSP, value >>  0);
  if constexpr(T::bits >= 16) write(--XSP, value >>  8);
  if constexpr(T::bits >= 24) write(--XSP, value >> 16);
  if constexpr(T::bits >= 32) write(--XSP, value >> 24);
}

#undef XSP

//

template<> auto TLCS900H::load(Memory< uint8> memory) ->  uint8 {
  return read(memory.address);
}

template<> auto TLCS900H::load(Memory<uint16> memory) -> uint16 {
  uint16 data = read(memory.address + 0) << 0;
  return data | read(memory.address + 1) << 8;
}

template<> auto TLCS900H::load(Memory<uint32> memory) -> uint32 {
  uint32 data  = read(memory.address + 0) <<  0;
         data |= read(memory.address + 1) <<  8;
         data |= read(memory.address + 2) << 16;
  return data |= read(memory.address + 3) << 24;
}

template<> auto TLCS900H::load(Memory< int8> memory) ->  int8 { return  (int8)load< uint8>(Memory< uint8>{memory.address}); }
template<> auto TLCS900H::load(Memory<int16> memory) -> int16 { return (int16)load<uint16>(Memory<uint16>{memory.address}); }
template<> auto TLCS900H::load(Memory<int32> memory) -> int32 { return (int32)load<uint32>(Memory<uint32>{memory.address}); }

template<> auto TLCS900H::store(Memory< uint8> memory, uint32 data) -> void {
  write(memory.address, data);
}

template<> auto TLCS900H::store(Memory<uint16> memory, uint32 data) -> void {
  write(memory.address + 0, data >> 0);
  write(memory.address + 1, data >> 8);
}

template<> auto TLCS900H::store(Memory<uint32> memory, uint32 data) -> void {
  write(memory.address + 0, data >>  0);
  write(memory.address + 1, data >>  8);
  write(memory.address + 2, data >> 16);
  write(memory.address + 3, data >> 24);
}
