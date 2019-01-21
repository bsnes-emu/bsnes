auto Flash::reset(natural ID) -> void {
  this->ID = ID;
  rom.reset();
  modified = false;
  vendorID = 0;
  deviceID = 0;
  blocks.reset();
}

auto Flash::allocate(natural size) -> bool {
  if(size ==  4_Mibit) { rom.allocate(size); vendorID = 0x98; deviceID = 0xab; }  //vendorID 0x98 => Toshiba
  if(size ==  8_Mibit) { rom.allocate(size); vendorID = 0x98; deviceID = 0x2c; }  //vendorID 0xb0 => Sharp
  if(size == 16_Mibit) { rom.allocate(size); vendorID = 0x98; deviceID = 0x2f; }  //vendorID 0xec => Samsung
  if(!size) return false;

  for(uint index : range(size / 64_KiB - 1)) blocks.append({true, index * 64_KiB, 64_KiB});
  blocks.append({true, size - 64_KiB, 32_KiB});
  blocks.append({true, size - 32_KiB,  8_KiB});
  blocks.append({true, size - 24_KiB,  8_KiB});
  blocks.append({true, size - 16_KiB, 16_KiB});
  return true;
}

auto Flash::load(vfs::shared::file fp) -> void {
  fp->read(rom.data(), rom.size());
}

auto Flash::power() -> void {
  status(Read);
}

auto Flash::read(uint21 address) -> uint8 {
  if(mode == ReadID) {
    switch((uint15)address) {  //todo: actual mask value unknown
    case 0: return vendorID;
    case 1: return deviceID;
    case 2: return 0x02;  //unknown purpose
    case 3: return 0x80;  //unknown purpose
    }
    return 0xff;  //invalid ReadID address; todo: actual return value unknown
  }
  return rom.read(address);  //todo: what happens when mode != Read here?
}

auto Flash::write(uint21 address, uint8 data) -> void {
  if(mode == Write) return program(address, data);
  if(data == 0xf0) return status(Read);
  address = (uint15)address;
  if(address == 0x5555 && data == 0xaa) return status(Prefix);
  if(mode == Prefix && address == 0x2aaa && data == 0x55) return status(Suffix);
  if(mode == Suffix && address == 0x5555 && data == 0x90) return status(ReadID);
  if(mode == Suffix && address == 0x5555 && data == 0xa0) return status(Write);
  if(mode == Suffix && address == 0x5555 && data == 0xf0) return status(Read);
  if(mode == Suffix && address == 0x5555 && data == 0x80) return status(ExtendedPrefix);
  if(mode == ExtendedPrefix && address == 0x2aaa && data == 0x55) return status(ExtendedSuffix);
  if(mode == ExtendedSuffix && address == 0x5555 && data == 0x10) return eraseAll();
  if(mode == ExtendedSuffix && data == 0x30) return erase((uint6)address);
  if(mode == ExtendedSuffix && data == 0x90) return protect((uint6)address);
  return status(Read);  //invalid or unsupported command
}

auto Flash::status(uint mode_) -> void {
  mode = mode_;
}

auto Flash::program(uint21 address, uint8 data) -> void {
  for(auto& block : blocks) {
    if(address >= block.offset && address < block.offset + block.length && block.writable) {
      if(auto input = rom.read(address); input != (input & data)) {
        modified = true;
        return rom.write(address, input & data);
      }
    }
  }
}

auto Flash::erase(uint6 blockID) -> void {
  //todo: unknown what happens when erasing invalid block IDs
  if(blockID >= blocks.size() || !blocks[blockID].writable) return;
  auto address = blocks[blockID].offset;
  for(auto offset : range(blocks[blockID].length)) rom.write(address + offset, 0xff);
  modified = true;
  return status(Read);
}

auto Flash::eraseAll() -> void {
  for(uint blockID : range(blocks.size())) erase(blockID);
}

auto Flash::protect(uint6 blockID) -> void {
  //todo: unknown what happens when protected invalid block IDs
  if(blockID >= blocks.size() || !blocks[blockID].writable) return;
  blocks[blockID].writable = false;
  modified = true;
  return status(Read);
}
