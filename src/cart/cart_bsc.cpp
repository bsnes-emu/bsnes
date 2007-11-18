void Cartridge::load_cart_bsc(const char *base, const char *slot) {
  if(!base || !*base) return;

  strcpy(cart.fn, base);
  strcpy(bs.fn, slot ? slot : "");
  load_begin(CartridgeBSC);

uint8 *data;
uint size;
  load_file(cart.fn, data, size);
  cart.rom = data, cart.rom_size = size;

  if(*bs.fn) {
    if(load_file(bs.fn, data, size) == true) {
      info.bsxflash = true;
      bs.ram = data, bs.ram_size = size;
    }
  }

  find_header();
  read_header();

  info.mapper = cartridge.info.header_index == 0x7fc0 ? BSCLoROM : BSCHiROM;
  info.region = NTSC;

  if(info.ram_size > 0) {
    cart.ram = (uint8*)malloc(cart.ram_size = info.ram_size);
    memset(cart.ram, 0xff, cart.ram_size);

    if(load_file(get_save_filename(cart.fn, "srm"), data, size) == true) {
      memcpy(cart.ram, data, min(size, cart.ram_size));
      safe_free(data);
    }
  }

  load_end();
}

void Cartridge::unload_cart_bsc() {
  if(cart.ram) save_file(get_save_filename(cart.fn, "srm"), cart.ram, cart.ram_size);
}
