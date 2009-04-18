#ifdef CART_CPP

//================
//Normal cartridge
//================

bool Cartridge::load_normal(const char *base) {
  uint8_t *data;
  unsigned size;
  bool patch_applied;
  cart.filename = base;

  load_begin(ModeNormal);
  if(load_image(base, data, size, patch_applied) == false) return false;

  snes.config.path.current = basepath(cart.filename);
  if(patch_applied) set(patched, true);

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = data, cart.rom_size = size);
  set_cartinfo(cartinfo);

  if(cartinfo.ram_size > 0) {
    load_ram(get_filename(base, "srm", snes.config.path.save), cart.ram, cart.ram_size = cartinfo.ram_size, 0xff);
  }

  if(cartinfo.srtc || cartinfo.spc7110rtc) {
    load_ram(get_filename(base, "rtc", snes.config.path.save), cart.rtc, cart.rtc_size = 20, 0x00);
  }

  load_end();
  set(name, basename(base));
  return true;
}

void Cartridge::unload_normal() {
  if(cart.ram) save_file(get_filename(cart.filename, "srm", snes.config.path.save), cart.ram, cart.ram_size);
  if(cart.rtc) save_file(get_filename(cart.filename, "rtc", snes.config.path.save), cart.rtc, cart.rtc_size);
}

//======================
//BS-X slotted cartridge
//======================

bool Cartridge::load_bsx_slotted(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  bool patch_applied;
  cart.filename = base;
  bs.filename = slot;

  load_begin(ModeBsxSlotted);
  if(load_image(base, data, size, patch_applied) == false) return false;

  snes.config.path.current = basepath(cart.filename);
  if(patch_applied) set(patched, true);

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = data, cart.rom_size = size);
  set_cartinfo(cartinfo);

  if(load_image(slot, data, size, patch_applied) == true) {
    set(bsx_flash_loaded, true);
    if(patch_applied) set(patched, true);
    bs.ram = data;
    bs.ram_size = size;
  }

  if(cartinfo.ram_size > 0) {
    load_ram(get_filename(base, "srm", snes.config.path.save), cart.ram, cart.ram_size = cartinfo.ram_size, 0xff);
  }

  load_end();
  string filename = basename(base);
  if(*slot) filename << " + " << basename(slot);
  set(name, filename);
  return true;
}

void Cartridge::unload_bsx_slotted() {
  if(cart.ram) save_file(get_filename(cart.filename, "srm", snes.config.path.save), cart.ram, cart.ram_size);
}

//====================
//BS-X flash cartridge
//====================

bool Cartridge::load_bsx(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  bool patch_applied;
  cart.filename = base;
  bs.filename = slot;

  load_begin(ModeBsx);
  if(load_image(base, data, size, patch_applied) == false) return false;

  snes.config.path.current = basepath(cart.filename);
  if(patch_applied) set(patched, true);

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = data, cart.rom_size = size);
  set_cartinfo(cartinfo);

  cart.ram = 0;
  cart.ram_size = 0;

  memset(bsxcart.sram.handle (), 0x00, bsxcart.sram.size ());
  memset(bsxcart.psram.handle(), 0x00, bsxcart.psram.size());

  if(load_file(get_filename(base, "srm", snes.config.path.save), data, size, CompressionNone) == true) {
    memcpy(bsxcart.sram.handle (), data, min(bsxcart.sram.size (), size));
    delete[] data;
  }

  if(load_file(get_filename(base, "psr", snes.config.path.save), data, size, CompressionNone) == true) {
    memcpy(bsxcart.psram.handle(), data, min(bsxcart.psram.size(), size));
    delete[] data;
  }

  if(load_image(slot, data, size, patch_applied) == true) {
    set(bsx_flash_loaded, true);
    if(patch_applied) set(patched, true);
    bs.ram = data;
    bs.ram_size = size;
  }

  load_end();
  set(name, !*slot ? basename(base) : basename(slot));
  return true;
}

void Cartridge::unload_bsx() {
  save_file(get_filename(cart.filename, "srm", snes.config.path.save), bsxcart.sram.handle (), bsxcart.sram.size ());
  save_file(get_filename(cart.filename, "psr", snes.config.path.save), bsxcart.psram.handle(), bsxcart.psram.size());
}

//============================
//Sufami Turbo flash cartridge
//============================

bool Cartridge::load_sufami_turbo(const char *base, const char *slotA, const char *slotB) {
  uint8_t *data;
  unsigned size;
  bool patch_applied;
  cart.filename = base;
  stA.filename = slotA;
  stB.filename = slotB;

  load_begin(ModeSufamiTurbo);
  if(load_image(base, data, size, patch_applied) == false) return false;

  snes.config.path.current = basepath(cart.filename);
  if(patch_applied) set(patched, true);

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = data, cart.rom_size = size);
  set_cartinfo(cartinfo);

  if(load_image(slotA, data, size, patch_applied) == true) {
    if(patch_applied) set(patched, true);
    stA.rom = new(zeromemory) uint8_t[stA.rom_size = 0x100000];
    memcpy(stA.rom, data, min(size, stA.rom_size));
    delete[] data;

    load_ram(get_filename(slotA, "srm", snes.config.path.save), stA.ram, stA.ram_size = 0x020000, 0xff);
  }

  if(load_image(slotB, data, size, patch_applied) == true) {
    if(patch_applied) set(patched, true);
    stB.rom = new(zeromemory) uint8_t[stB.rom_size = 0x100000];
    memcpy(stB.rom, data, min(size, stB.rom_size));
    delete[] data;

    load_ram(get_filename(slotB, "srm", snes.config.path.save), stB.ram, stB.ram_size = 0x020000, 0xff);
  }

  load_end();
  string filename;
       if(!*slotA && !*slotB) filename << basename(base);
  else if( *slotA && !*slotB) filename << basename(slotA);
  else if(!*slotA &&  *slotB) filename << basename(slotB);
  else filename << basename(slotA) << " + " << basename(slotB);
  set(name, filename);
  return true;
}

void Cartridge::unload_sufami_turbo() {
  if(stA.ram) save_file(get_filename(stA.filename, "srm", snes.config.path.save), stA.ram, stA.ram_size);
  if(stB.ram) save_file(get_filename(stB.filename, "srm", snes.config.path.save), stB.ram, stB.ram_size);
}

//=================
//utility functions
//=================

Cartridge::Type Cartridge::detect_image_type(const char *filename) const {
  uint8_t *data;
  unsigned size;
  bool patch_applied;
  if(!load_image(filename, data, size, patch_applied)) return TypeUnknown;

  cartinfo_t info;
  read_header(info, data, size);
  delete[] data;
  return info.type;
}

bool Cartridge::load_image(const char *filename, uint8_t *&data, unsigned &size, bool &patched) const {
  if(!filename || !*filename) return false;
  if(!load_file(filename, data, size, CompressionAuto)) return false;

  if((size & 0x7fff) == 512) {
    //remove 512-byte header
    memmove(data, data + 512, size -= 512);
  }

  uint8_t *pdata;
  unsigned psize;
  string path = (snes.config.path.patch == "" ? basepath(filename) : snes.config.path.patch);
  if(load_file(get_filename(filename, "ups", path), pdata, psize, CompressionInspect) == true) {
    bool result = apply_patch(pdata, psize, data, size);
    delete[] pdata;
    patched = result;
  } else {
    patched = false;
  }

  return true;
}

bool Cartridge::load_ram(const char *filename, uint8_t *&data, unsigned size, uint8_t init) const {
  data = new uint8_t[size];
  memset(data, init, size);

  uint8_t *savedata;
  unsigned savesize;
  if(load_file(filename, savedata, savesize, CompressionNone) == false) return false;

  memcpy(data, savedata, min(size, savesize));
  delete[] savedata;
  return true;
}

#endif
