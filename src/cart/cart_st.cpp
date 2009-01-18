#ifdef CART_CPP

void Cartridge::load_cart_st(const char *base, const char *slotA, const char *slotB) {
  uint8_t *data;
  unsigned size;
  strcpy(cart.fn, base);
  strcpy(stA.fn, slotA);
  strcpy(stB.fn, slotB);

  load_begin(ModeSufamiTurbo);
  if(load_image(base) == false) return;

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = image.data, cart.rom_size = image.size);
  info = cartinfo;

  if(load_image(slotA)) {
    stA.rom = new(zeromemory) uint8_t[stA.rom_size = 0x100000];
    memcpy(stA.rom, image.data, min(image.size, stA.rom_size));
    delete[] image.data;

    load_ram(get_save_filename(slotA, "srm"), stA.ram, stA.ram_size = 0x020000, 0xff);
  }

  if(load_image(slotB)) {
    stB.rom = new(zeromemory) uint8_t[stB.rom_size = 0x100000];
    memcpy(stB.rom, image.data, min(image.size, stB.rom_size));
    delete[] image.data;

    load_ram(get_save_filename(slotB, "srm"), stB.ram, stB.ram_size = 0x020000, 0xff);
  }

  load_end();

  //set base filename
  if(!*slotA && !*slotB) {
    strcpy(info.filename, cart.fn);
    get_base_filename(info.filename);
  } else if(*slotA && !*slotB) {
    strcpy(info.filename, slotA);
    get_base_filename(info.filename);
  } else if(!*slotA && *slotB) {
    strcpy(info.filename, slotB);
    get_base_filename(info.filename);
  } else {
    char filenameA[PATH_MAX], filenameB[PATH_MAX];
    strcpy(filenameA, slotA);
    get_base_filename(filenameA);
    strcpy(filenameB, slotB);
    get_base_filename(filenameB);
    strcpy(info.filename, filenameA);
    strcat(info.filename, " + ");
    strcat(info.filename, filenameB);
  }
}

void Cartridge::unload_cart_st() {
  if(stA.ram) save_file(get_save_filename(stA.fn, "srm"), stA.ram, stA.ram_size);
  if(stB.ram) save_file(get_save_filename(stB.fn, "srm"), stB.ram, stB.ram_size);
}

#endif
