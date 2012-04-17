bool Cartridge::EEPROM::read(unsigned addr) {
  return data[addr >> 3] & 0x80 >> (addr & 7);
}

void Cartridge::EEPROM::write(unsigned addr, bool bit) {
  if(bit == 0) data[addr >> 3] &=~ (0x80 >> (addr & 7));
  if(bit == 1) data[addr >> 3] |=  (0x80 >> (addr & 7));
}

bool Cartridge::EEPROM::read() {
  bool bit = 1;

  if(mode == Mode::ReadData) {
    if(offset >= 4) bit = read(address * 64 + (offset - 4));
    if(++offset == 68) mode = Mode::Wait;
  }

  return bit;
}

void Cartridge::EEPROM::write(bool bit) {
  if(mode == Mode::Wait) {
    if(bit == 1) mode = Mode::Command;
  }

  else if(mode == Mode::Command) {
    if(bit == 0) mode = Mode::WriteAddress;
    if(bit == 1) mode = Mode::ReadAddress;
    offset = 0;
    address = 0;
  }

  else if(mode == Mode::ReadAddress) {
    address = (address << 1) | bit;
    if(++offset == bits) {
      mode = Mode::ReadValidate;
      offset = 0;
    }
  }

  else if(mode == Mode::ReadValidate) {
    if(bit == 1);  //invalid
    mode = Mode::ReadData;
  }

  else if(mode == Mode::WriteAddress) {
    address = (address << 1) | bit;
    if(++offset == bits) {
      mode = Mode::WriteData;
      offset = 0;
    }
  }

  else if(mode == Mode::WriteData) {
    write(address * 64 + offset, bit);
    if(++offset == 64) {
      mode = Mode::WriteValidate;
    }
  }

  else if(mode == Mode::WriteValidate) {
    if(bit == 1);  //invalid
    mode = Mode::Wait;
  }
}

void Cartridge::EEPROM::power() {
  bits = (size <= 512 ? 6 : 14);

  mode = Mode::Wait;
  offset = 0;
  address = 0;
}

void Cartridge::EEPROM::serialize(serializer &s) {
  s.array(data, size);
  s.integer(size);
  s.integer(mask);
  s.integer(test);
  s.integer(bits);
  s.integer((unsigned&)mode);
  s.integer(offset);
  s.integer(address);
}
