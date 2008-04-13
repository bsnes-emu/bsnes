#ifdef CART_CPP

void Cartridge::load_cart_st(const char *base, const char *slotA, const char *slotB) {
  if(!base || !*base) return;

  strcpy(cart.fn, base);
  strcpy(stA.fn, slotA ? slotA : "");
  strcpy(stB.fn, slotB ? slotB : "");

  load_begin(CartridgeSufamiTurbo);
  info.st = true;
  info.mapper = STROM;
  info.region = NTSC;

  uint8_t *data = 0;
  unsigned size;
  if(load_file(cart.fn, data, size, CompressionAuto) == true) {
    cart.rom = (uint8*)malloc(cart.rom_size = 0x040000);
    memcpy(cart.rom, data, min(size, cart.rom_size));
    safe_free(data);
    if(load_file(get_patch_filename(cart.fn, "ups"), data, size, CompressionInspect) == true) {
      apply_patch(data, size, cart.rom, cart.rom_size);
      if(data) { free(data); data = 0; }
    }
  }

  if(*stA.fn) {
    if(load_file(stA.fn, data, size, CompressionAuto) == true) {
      stA.rom = (uint8*)malloc(stA.rom_size = 0x100000);
      memcpy(stA.rom, data, min(size, stA.rom_size));
      safe_free(data);
      if(load_file(get_patch_filename(stA.fn, "ups"), data, size, CompressionInspect) == true) {
        apply_patch(data, size, stA.rom, stA.rom_size);
        if(data) { free(data); data = 0; }
      }

      stA.ram = (uint8*)malloc(stA.ram_size = 0x020000);
      memset(stA.ram, 0xff, stA.ram_size);

      if(load_file(get_save_filename(stA.fn, "srm"), data, size, CompressionNone) == true) {
        memcpy(stA.ram, data, min(size, 0x020000U));
        safe_free(data);
      }
    }
  }

  if(*stB.fn) {
    if(load_file(stB.fn, data, size, CompressionAuto) == true) {
      stB.rom = (uint8*)malloc(stB.rom_size = 0x100000);
      memcpy(stB.rom, data, min(size, stB.rom_size));
      safe_free(data);
      if(load_file(get_patch_filename(stB.fn, "ups"), data, size, CompressionInspect) == true) {
        apply_patch(data, size, stB.rom, stB.rom_size);
        if(data) { free(data); data = 0; }
      }

      stB.ram = (uint8*)malloc(stB.ram_size = 0x020000);
      memset(stB.ram, 0xff, stB.ram_size);

      if(load_file(get_save_filename(stB.fn, "srm"), data, size, CompressionNone) == true) {
        memcpy(stB.ram, data, min(size, 0x020000U));
        safe_free(data);
      }
    }
  }

  load_end();

  //set base filename
  if(!*stA.fn && !*stB.fn) {
    strcpy(info.filename, cart.fn);
    get_base_filename(info.filename);
  } else if(*stA.fn && !*stB.fn) {
    strcpy(info.filename, stA.fn);
    get_base_filename(info.filename);
  } else if(!*stA.fn && *stB.fn) {
    strcpy(info.filename, stB.fn);
    get_base_filename(info.filename);
  } else {
    char filenameA[PATH_MAX], filenameB[PATH_MAX];
    strcpy(filenameA, stA.fn);
    get_base_filename(filenameA);
    strcpy(filenameB, stB.fn);
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

#endif //ifdef CART_CPP
