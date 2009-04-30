#ifdef CART_CPP

Cartridge::Type Cartridge::detect_image_type(uint8_t *data, unsigned size) const {
  cartinfo_t info;
  read_header(info, data, size);
  return info.type;
}

//================
//Normal cartridge
//================

bool Cartridge::load_normal(uint8_t *basedata, unsigned basesize) {
  load_begin(ModeNormal);

  memory::cartrom.map(new uint8_t[basesize], basesize);
  memcpy(memory::cartrom.handle(), basedata, basesize);

  cartinfo_t cartinfo;
  read_header(cartinfo, basedata, basesize);
  set_cartinfo(cartinfo);

  if(cartinfo.ram_size) {
    memory::cartram.map(new(zeromemory) uint8_t[cartinfo.ram_size], cartinfo.ram_size);
  }

  if(cartinfo.srtc || cartinfo.spc7110rtc) {
    memory::cartrtc.map(new(zeromemory) uint8_t[20], 20);
  }

  load_end();
  return true;
}

//======================
//BS-X slotted cartridge
//======================

bool Cartridge::load_bsx_slotted(uint8_t *basedata, unsigned basesize, uint8_t *slotdata, unsigned slotsize) {
  load_begin(ModeBsxSlotted);

  memory::cartrom.map(new uint8_t[basesize], basesize);
  memcpy(memory::cartrom.handle(), basedata, basesize);

  cartinfo_t cartinfo;
  read_header(cartinfo, basedata, basesize);
  set_cartinfo(cartinfo);

  if(slotdata) {
    memory::bscram.map(new uint8_t[slotsize], slotsize);
    memcpy(memory::bscram.handle(), slotdata, slotsize);
    set(bsx_flash_loaded, true);
  }

  if(cartinfo.ram_size) {
    memory::cartram.map(new(zeromemory) uint8_t[cartinfo.ram_size], cartinfo.ram_size);
  }

  load_end();
  return true;
}

//====================
//BS-X flash cartridge
//====================

bool Cartridge::load_bsx(uint8_t *basedata, unsigned basesize, uint8_t *slotdata, unsigned slotsize) {
  load_begin(ModeBsx);

  memory::cartrom.map(new uint8_t[basesize], basesize);
  memcpy(memory::cartrom.handle(), basedata, basesize);

  cartinfo_t cartinfo;
  read_header(cartinfo, basedata, basesize);
  set_cartinfo(cartinfo);

  if(slotdata) {
    memory::bscram.map(new uint8_t[slotsize], slotsize);
    memcpy(memory::bscram.handle(), slotdata, slotsize);
    set(bsx_flash_loaded, true);
  }

  memset(bsxcart.sram.handle (), 0x00, bsxcart.sram.size ());
  memset(bsxcart.psram.handle(), 0x00, bsxcart.psram.size());

  load_end();
  return true;
}

//============================
//Sufami Turbo flash cartridge
//============================

bool Cartridge::load_sufami_turbo(uint8_t *basedata, unsigned basesize, uint8_t *slotAdata, unsigned slotAsize, uint8_t *slotBdata, unsigned slotBsize) {
  load_begin(ModeSufamiTurbo);

  memory::cartrom.map(new uint8_t[basesize], basesize);
  memcpy(memory::cartrom.handle(), basedata, basesize);

  cartinfo_t cartinfo;
  read_header(cartinfo, basedata, basesize);
  set_cartinfo(cartinfo);

  if(slotAdata) {
    memory::stArom.map(new uint8_t[slotAsize], slotAsize);
    memory::stAram.map(new uint8_t[0x020000], 0x020000);
    memcpy(memory::stArom.handle(), slotAdata, slotAsize);
  }

  if(slotBdata) {
    memory::stBrom.map(new uint8_t[slotBsize], slotBsize);
    memory::stBram.map(new uint8_t[0x020000], 0x020000);
    memcpy(memory::stBrom.handle(), slotBdata, slotBsize);
  }

  load_end();
  return true;
}

//=======================
//Super Gameboy cartridge
//=======================

bool Cartridge::load_super_gameboy(uint8_t *basedata, unsigned basesize, uint8_t *slotdata, unsigned slotsize) {
  load_begin(ModeSuperGameboy);

  memory::cartrom.map(new uint8_t[basesize], basesize);
  memcpy(memory::cartrom.handle(), basedata, basesize);

  cartinfo_t cartinfo;
  read_header(cartinfo, basedata, basesize);
  set_cartinfo(cartinfo);

  if(slotdata) {
    uint8_t *data = new uint8_t[slotsize];
    memcpy(data, slotdata, slotsize);
    memory::dmgrom.map(data, slotsize);

    //TODO: determine proper RAM size via GB ROM header
    memory::dmgram.map(new uint8_t[524288], 524288);
    memory::dmgrtc.map(new uint8_t[4], 4);
  }

  load_end();
  return true;
}

#endif
