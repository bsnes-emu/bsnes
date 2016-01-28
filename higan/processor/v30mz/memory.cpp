auto V30MZ::readByte() -> uint8 {
  return read((r.cs << 4) + (r.ip++));
}

auto V30MZ::readWord() -> uint16 {
  uint16 word = read((r.cs << 4) + (r.ip++)) << 0;
  return word | read((r.cs << 4) + (r.ip++)) << 8;
}
