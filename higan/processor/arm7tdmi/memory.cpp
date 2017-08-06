auto ARM7TDMI::idle() -> void {
  pipeline.nonsequential = true;
  sleep();
}

auto ARM7TDMI::read(uint mode, uint32 address) -> uint32 {
  return get(mode, address);
}

auto ARM7TDMI::load(uint mode, uint32 address) -> uint32 {
  pipeline.nonsequential = true;
  auto word = get(Load | mode, address);
  if(mode & Half) {
    address &= 1;
    word = mode & Signed ? (uint16)(int16)word : (uint16)word;
  }
  if(mode & Byte) {
    address &= 0;
    word = mode & Signed ? (uint8)(int8)word : (uint8)word;
  }
  if(mode & Signed) {
    word = ASR(word, address.bits(0,1) << 3);
  } else {
    word = ROR(word, address.bits(0,1) << 3);
  }
  idle();
  return word;
}

auto ARM7TDMI::write(uint mode, uint32 address, uint32 word) -> void {
  pipeline.nonsequential = true;
  return set(mode, address, word);
}

auto ARM7TDMI::store(uint mode, uint32 address, uint32 word) -> void {
  pipeline.nonsequential = true;
  if(mode & Half) { word &= 0xffff; word |= word << 16; }
  if(mode & Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }
  return set(Store | mode, address, word);
}
