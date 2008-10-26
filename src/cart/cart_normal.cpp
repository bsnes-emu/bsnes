#ifdef CART_CPP

void Cartridge::load_cart_normal(const char *base) {
  uint8_t *data;
  unsigned size;
  strcpy(cart.fn, base);

  load_begin(ModeNormal);
  if(load_image(base) == false) return;

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = image.data, cart.rom_size = image.size);
  info = cartinfo;

  if(cartinfo.ram_size > 0) {
    load_ram(get_save_filename(base, "srm"), cart.ram, cart.ram_size = cartinfo.ram_size, 0xff);
  }

  if(cartinfo.srtc || cartinfo.spc7110rtc) {
    load_ram(get_save_filename(base, "rtc"), cart.rtc, cart.rtc_size = 20, 0x00);
  }

  load_end();

  //set base filename
  strcpy(info.filename, base);
  get_base_filename(info.filename);
}

void Cartridge::unload_cart_normal() {
  if(cart.ram) save_file(get_save_filename(cart.fn, "srm"), cart.ram, cart.ram_size);
  if(cart.rtc) save_file(get_save_filename(cart.fn, "rtc"), cart.rtc, cart.rtc_size);
}

#endif //ifdef CART_CPP
