#ifdef CART_CPP

bool Cartridge::load_image(const char *filename) {
  if(!filename || !*filename) return false;

  uint8_t *data;
  unsigned size;
  if(!load_file(filename, data, size, CompressionAuto)) return false;

  if((size & 0x7fff) != 512) {
    image.data = data;
    image.size = size;
  } else {
    //remove 512-byte header
    image.data = new uint8_t[image.size = size - 512];
    memcpy(image.data, data + 512, image.size);
  }

  if(load_file(get_patch_filename(filename, "ups"), data, size, CompressionInspect) == true) {
    apply_patch(data, size, image.data, image.size);
    delete[] data;
    info.patched = true;
  }

  return true;
}

bool Cartridge::inspect_image(cartinfo_t &cartinfo, const char *filename) {
  cartinfo.reset();
  if(!load_image(filename)) return false;

  read_header(cartinfo, image.data, image.size);
  delete[] image.data;
  return true;
}

bool Cartridge::load_ram(const char *filename, uint8_t *&data, unsigned size, uint8_t init) {
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
