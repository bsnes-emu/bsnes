#include "../base.h"
#include "database.cpp"

#include "cart_normal.cpp"
#include "cart_st.cpp"
#include "cart_stdual.cpp"

#include "cart_file.cpp"
#include "cart_header.cpp"

Cartridge cartridge;

void Cartridge::load_begin(uint cart_type) {
  if(loaded() == true)return;

  info.type = cart_type;

  info.srtc = false;
  info.sdd1 = false;
  info.c4   = false;
  info.dsp1 = false;
  info.dsp2 = false;
  info.obc1 = false;

  info.dsp1_mapper = 0;

  info.header_index = 0xffc0;
  info.mapper = PCB;
  strcpy(info.name, "");
  strcpy(info.pcb,  "");
  info.region = NTSC;
  info.cart_mmio = false;

  info.rom_size = 0;
  info.ram_size = 0;

  file.count = 0;
  for(int i = 0; i < 8; i++) {
    strcpy(file.rom_name[i], "");
    strcpy(file.ram_name[i], "");
    file.rom_size[i] = 0;
    file.ram_size[i] = 0;
    file.rom_data[i] = 0;
    file.ram_data[i] = 0;
  }
}

void Cartridge::load(const char *rom_fn) {
  if(!rom_fn || !*rom_fn)return;

char fn[4096], ram_fn[4096];
  strcpy(fn, rom_fn);
//correct folder slashes
  for(int i = strlen(fn) - 1; i >= 0; i--) {
    if(fn[i] == '\\')fn[i] = '/';
  }

uint i = file.count++;
  strcpy(file.rom_name[i], fn);

  strcpy(fn, rom_fn);
//remove ROM extension
  for(int i = strlen(fn) - 1; i >= 0; i--) {
    if(fn[i] == '.') {
      fn[i] = 0;
      break;
    }
  }

  if(i == 0) {
    strcpy(file.patch_name, fn);
    strcat(file.patch_name, ".ups");
  }
  strcpy(fn, strptr(config::file_updatepath(fn, config::path.save)));
  if(i == 0) {
    strcpy(file.cheat_name, fn);
    strcat(file.cheat_name, ".cht");
  }
  strcpy(file.ram_name[i], fn);
  strcat(file.ram_name[i], ".");
  strcat(file.ram_name[i], config::path.save_ext);
}

bool Cartridge::load_end() {
  for(int i = 0; i < file.count; i++) {
    load_file(file.rom_name[i], file.rom_data[i], file.rom_size[i]);
  }

  if(fexists(file.cheat_name) == true) {
    cheat.clear();
    cheat.load(file.cheat_name);
  }

  switch(info.type) {
  case CART_NORMAL: {
    load_rom_normal();
    load_ram_normal();
  } break;
  case CART_ST: {
    load_rom_st();
    load_ram_st();
  } break;
  case CART_STDUAL: {
    load_rom_stdual();
    load_ram_stdual();
  } break;
  }

  cart_loaded = true;
  r_mem->load_cart();
  return true;
}

bool Cartridge::unload() {
  if(cart_loaded == false)return false;

  r_mem->unload_cart();

  switch(info.type) {
  case CART_NORMAL: {
    save_ram_normal();
  } break;
  case CART_ST: {
    save_ram_st();
  } break;
  case CART_STDUAL: {
    save_ram_stdual();
  } break;
  }

  SafeFree(rom);
  SafeFree(ram);

  if(cheat.count() > 0 || fexists(file.cheat_name) == true) {
    cheat.save(file.cheat_name);
    cheat.clear();
  }

  cart_loaded = false;
  return true;
}

Cartridge::Cartridge() {
  load_database();

  cart_loaded = false;

  rom = 0;
  ram = 0;
}

Cartridge::~Cartridge() {
  if(cart_loaded == true) {
    unload();
  }
}
