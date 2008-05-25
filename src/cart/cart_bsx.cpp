#ifdef CART_CPP

void Cartridge::load_cart_bsx(const char *base, const char *slot) {
  if(!base || !*base) return;

  strcpy(cart.fn, base);
  strcpy(bs.fn, slot ? slot : "");

  load_begin(CartridgeBSX);
  info.bsxbase = true;
  info.bsxcart = true;
  info.mapper = BSXROM;
  info.region = NTSC;

  uint8_t *data = 0;
  unsigned size;
  load_file(cart.fn, data, size, CompressionAuto);
  cart.rom = data, cart.rom_size = size;
  cart.ram = 0, cart.ram_size = 0;

  if(load_file(get_patch_filename(cart.fn, "ups"), data, size, CompressionInspect) == true) {
    apply_patch(data, size, cart.rom, cart.rom_size);
    delete[] data;
  }

  memset(bsxcart.sram.handle (), 0x00, bsxcart.sram.size ());
  memset(bsxcart.psram.handle(), 0x00, bsxcart.psram.size());

  if(load_file(get_save_filename(cart.fn, "srm"), data, size, CompressionNone) == true) {
    memcpy(bsxcart.sram.handle (), data, min(bsxcart.sram.size (), size));
    delete[] data;
  }

  if(load_file(get_save_filename(cart.fn, "psr"), data, size, CompressionNone) == true) {
    memcpy(bsxcart.psram.handle(), data, min(bsxcart.psram.size(), size));
    delete[] data;
  }

  if(*bs.fn) {
    if(load_file(bs.fn, data, size, CompressionAuto) == true) {
      info.bsxflash = true;
      bs.ram = data, bs.ram_size = size;
      if(load_file(get_patch_filename(bs.fn, "ups"), data, size, CompressionInspect) == true) {
        apply_patch(data, size, bs.ram, bs.ram_size);
        delete[] data;
      }
    }
  }

  load_end();

  strcpy(info.filename, !*bs.fn ? cart.fn : bs.fn);
  get_base_filename(info.filename);
}

void Cartridge::unload_cart_bsx() {
  save_file(get_save_filename(cart.fn, "srm"), bsxcart.sram.handle (), bsxcart.sram.size ());
  save_file(get_save_filename(cart.fn, "psr"), bsxcart.psram.handle(), bsxcart.psram.size());
}

#endif //ifdef CART_CPP
