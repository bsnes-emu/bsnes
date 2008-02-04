void Cartridge::load_cart_st(const char *base, const char *slotA, const char *slotB) {
  if(!base || !*base) return;

  strcpy(cart.fn, base);
  strcpy(stA.fn, slotA ? slotA : "");
  strcpy(stB.fn, slotB ? slotB : "");

  load_begin(CartridgeSufamiTurbo);
  info.st = true;
  info.mapper = STROM;
  info.region = NTSC;

uint8 *data;
uint size;
  if(load_file(cart.fn, data, size) == true) {
    cart.rom = (uint8*)malloc(cart.rom_size = 0x040000);
    memcpy(cart.rom, data, min(size, cart.rom_size));
    safe_free(data);
  }

  if(*stA.fn) {
    if(load_file(stA.fn, data, size) == true) {
      stA.rom = (uint8*)malloc(stA.rom_size = 0x100000);
      memcpy(stA.rom, data, min(size, stA.rom_size));
      safe_free(data);

      stA.ram = (uint8*)malloc(stA.ram_size = 0x020000);
      memset(stA.ram, 0xff, stA.ram_size);

      if(load_file(get_save_filename(stA.fn, "srm"), data, size) == true) {
        memcpy(stA.ram, data, min(size, 0x020000U));
        safe_free(data);
      }
    }
  }

  if(*stB.fn) {
    if(load_file(stB.fn, data, size) == true) {
      stB.rom = (uint8*)malloc(stB.rom_size = 0x100000);
      memcpy(stB.rom, data, min(size, stB.rom_size));
      safe_free(data);

      stB.ram = (uint8*)malloc(stB.ram_size = 0x020000);
      memset(stB.ram, 0xff, stB.ram_size);

      if(load_file(get_save_filename(stB.fn, "srm"), data, size) == true) {
        memcpy(stB.ram, data, min(size, 0x020000U));
        safe_free(data);
      }
    }
  }

  load_end();
}

void Cartridge::unload_cart_st() {
  if(stA.ram) save_file(get_save_filename(stA.fn, "srm"), stA.ram, stA.ram_size);
  if(stB.ram) save_file(get_save_filename(stB.fn, "srm"), stB.ram, stB.ram_size);
}
