#ifdef CART_CPP

void Cartridge::load_cart_bsc(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  strcpy(cart.fn, base);
  strcpy(bs.fn, slot);

  load_begin(ModeBSC);
  if(load_image(base) == false) return;

  cartinfo_t cartinfo;
  read_header(cartinfo, cart.rom = image.data, cart.rom_size = image.size);
  info = cartinfo;

  if(load_image(slot) == true) {
    info.bsxflash = true;
    bs.ram = image.data;
    bs.ram_size = image.size;
  }

  if(cartinfo.ram_size > 0) {
    load_ram(get_save_filename(base, "srm"), cart.ram, cart.ram_size = cartinfo.ram_size, 0xff);
  }

  load_end();

  //set base filename
  strcpy(info.filename, base);
  get_base_filename(info.filename);
  if(*slot) {
    char filenameBS[PATH_MAX];
    strcpy(filenameBS, slot);
    get_base_filename(filenameBS);
    strcat(info.filename, " + ");
    strcat(info.filename, filenameBS);
  }
}

void Cartridge::unload_cart_bsc() {
  if(cart.ram) save_file(get_save_filename(cart.fn, "srm"), cart.ram, cart.ram_size);
}

#endif
