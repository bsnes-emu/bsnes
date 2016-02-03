auto V30MZ::read(Size size, uint16 segment, uint16 address) -> uint32 {
  uint32 data = read(segment * 16 + address);
  if(size == Word) data |= read(segment * 16 + ++address) << 8;
  if(size == Long) data |= read(segment * 16 + ++address) << 16;
  if(size == Long) data |= read(segment * 16 + ++address) << 24;
  return data;
}

auto V30MZ::write(Size size, uint16 segment, uint16 address, uint16 data) -> void {
  write(segment * 16 + address, data);
  if(size == Word) write(segment * 16 + ++address, data >> 8);
}

//

auto V30MZ::in(Size size, uint16 address) -> uint16 {
  uint16 data = in(address);
  if(size == Word) data |= in(++address) << 8;
  return data;
}

auto V30MZ::out(Size size, uint16 address, uint16 data) -> void {
  out(address, data);
  if(size == Word) out(++address, data >> 8);
}

//

auto V30MZ::fetch(Size size) -> uint16 {
  uint16 data = read(size, r.cs, r.ip);
  return r.ip += size, data;
}

//

auto V30MZ::pop() -> uint16 {
  uint16 data = read(Word, r.ss, r.sp);
  return r.sp += Word, data;
}

auto V30MZ::push(uint16 data) -> void {
  r.sp -= Word;
  write(Word, r.ss, r.sp, data);
}
