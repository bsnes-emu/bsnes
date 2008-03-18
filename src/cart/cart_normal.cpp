#ifdef CART_CPP

void Cartridge::load_cart_normal(const char *filename) {
  if(!filename || !*filename) return;

  uint8_t *data = 0;
  unsigned size;
  if(load_file(filename, data, size) == false) return;
  strcpy(cart.fn, filename);

  load_begin(CartridgeNormal);

  //load ROM data, ignore 512-byte header if detected
  if((size & 0x7fff) != 512) {
    cart.rom = (uint8*)malloc(cart.rom_size = size);
    memcpy(cart.rom, data, size);
  } else {
    cart.rom = (uint8*)malloc(cart.rom_size = size - 512);
    memcpy(cart.rom, data + 512, size - 512);
  }
  safe_free(data);

  info.crc32 = crc32_calculate(cart.rom, cart.rom_size);

  find_header();
  read_header();

  if(info.ram_size > 0) {
    cart.ram = (uint8*)malloc(cart.ram_size = info.ram_size);
    memset(cart.ram, 0xff, cart.ram_size);

    if(load_file(get_save_filename(cart.fn, "srm"), data, size) == true) {
      memcpy(cart.ram, data, min(size, cart.ram_size));
      safe_free(data);
    }
  }

  load_end();

  //set base filename
  strcpy(info.filename, cart.fn);
  get_base_filename(info.filename);
}

void Cartridge::unload_cart_normal() {
  if(cart.ram) save_file(get_save_filename(cart.fn, "srm"), cart.ram, cart.ram_size);
}

#endif //ifdef CART_CPP
