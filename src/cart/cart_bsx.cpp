#ifdef CART_CPP

void Cartridge::load_cart_bsx(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  strcpy(cart.fn, base);
  strcpy(bs.fn, slot);

  load_begin(ModeBSX);
  if(load_image(base) == false) return;
  info.bsxcart = true;

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = image.data, cart.rom_size = image.size);
  info = cartinfo;
  cart.ram = 0;
  cart.ram_size = 0;

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

  if(load_image(slot)) {
    info.bsxflash = true;
    bs.ram = image.data;
    bs.ram_size = image.size;
  }

  load_end();

  strcpy(info.filename, !*slot ? base : slot);
  get_base_filename(info.filename);
}

void Cartridge::unload_cart_bsx() {
  save_file(get_save_filename(cart.fn, "srm"), bsxcart.sram.handle (), bsxcart.sram.size ());
  save_file(get_save_filename(cart.fn, "psr"), bsxcart.psram.handle(), bsxcart.psram.size());
}

#endif //ifdef CART_CPP
