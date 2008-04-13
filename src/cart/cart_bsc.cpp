#ifdef CART_CPP

void Cartridge::load_cart_bsc(const char *base, const char *slot) {
  if(!base || !*base) return;

  strcpy(cart.fn, base);
  strcpy(bs.fn, slot ? slot : "");
  load_begin(CartridgeBSC);

  uint8_t *data = 0;
  unsigned size;
  load_file(cart.fn, data, size, CompressionAuto);
  cart.rom = data, cart.rom_size = size;

  if(load_file(get_patch_filename(cart.fn, "ups"), data, size, CompressionInspect) == true) {
    apply_patch(data, size, cart.rom, cart.rom_size);
    if(data) { free(data); data = 0; }
  }

  if(*bs.fn) {
    if(load_file(bs.fn, data, size, CompressionAuto) == true) {
      info.bsxflash = true;
      bs.ram = data, bs.ram_size = size;
      if(load_file(get_patch_filename(bs.fn, "ups"), data, size, CompressionInspect) == true) {
        apply_patch(data, size, bs.ram, bs.ram_size);
        if(data) { free(data); data = 0; }
      }
    }
  }

  find_header();
  read_header();

  info.mapper = cartridge.info.header_index == 0x7fc0 ? BSCLoROM : BSCHiROM;
  info.region = NTSC;

  if(info.ram_size > 0) {
    cart.ram = (uint8*)malloc(cart.ram_size = info.ram_size);
    memset(cart.ram, 0xff, cart.ram_size);

    if(load_file(get_save_filename(cart.fn, "srm"), data, size, CompressionNone) == true) {
      memcpy(cart.ram, data, min(size, cart.ram_size));
      safe_free(data);
    }
  }

  load_end();

  //set base filename
  strcpy(info.filename, cart.fn);
  get_base_filename(info.filename);
  if(*bs.fn) {
    char filenameBS[PATH_MAX];
    strcpy(filenameBS, bs.fn);
    get_base_filename(filenameBS);
    strcat(info.filename, " + ");
    strcat(info.filename, filenameBS);
  }
}

void Cartridge::unload_cart_bsc() {
  if(cart.ram) save_file(get_save_filename(cart.fn, "srm"), cart.ram, cart.ram_size);
}

#endif //ifdef CART_CPP
