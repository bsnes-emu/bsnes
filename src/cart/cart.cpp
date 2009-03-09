#include <../base.hpp>
#include <../chip/chip.hpp>
#include <../reader/reader.hpp>
#define CART_CPP

#include <nall/crc32.hpp>
#include <nall/ups.hpp>

#include "cart.hpp"
#include "cart_file.cpp"
#include "cart_header.cpp"
#include "cart_loader.cpp"

namespace memory {
  MappedRAM cartrom, cartram, cartrtc;
  MappedRAM bscram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
};

Cartridge cartridge;

void Cartridge::load_begin(Mode cartridge_mode) {
  cart.rom = cart.ram = cart.rtc = 0;
  bs.ram   = 0;
  stA.rom  = stA.ram  = 0;
  stB.rom  = stB.ram  = 0;

  cart.rom_size = cart.ram_size = cart.rtc_size = 0;
  bs.ram_size   = 0;
  stA.rom_size  = stA.ram_size  = 0;
  stB.rom_size  = stB.ram_size  = 0;

  set(loaded,           false);
  set(bsx_flash_loaded, false);
  set(patched,          false);
  set(mode,             cartridge_mode);
}

void Cartridge::load_end() {
  memory::cartrom.map(cart.rom, cart.rom_size);
  memory::cartram.map(cart.ram, cart.ram_size);
  memory::cartrtc.map(cart.rtc, cart.rtc_size);
  memory::bscram.map(bs.ram, bs.ram_size);
  memory::stArom.map(stA.rom, stA.rom_size);
  memory::stAram.map(stA.ram, stA.ram_size);
  memory::stBrom.map(stB.rom, stB.rom_size);
  memory::stBram.map(stB.ram, stB.ram_size);

  memory::cartrom.write_protect(true);
  memory::cartram.write_protect(false);
  memory::bscram.write_protect(true);
  memory::stArom.write_protect(true);
  memory::stAram.write_protect(false);
  memory::stBrom.write_protect(true);
  memory::stBram.write_protect(false);

  string cheat_file = get_filename(cart.filename, "cht", snes.config.path.cheat);
  if(file::exists(cheat_file)) {
    cheat.clear();
    cheat.load(cheat_file);
  }

  bus.load_cart();
  set(loaded, true);
}

void Cartridge::unload() {
  if(loaded() == false) return;
  bus.unload_cart();

  switch(mode()) {
    case ModeNormal:      unload_normal();       break;
    case ModeBsxSlotted:  unload_bsx_slotted();  break;
    case ModeBsx:         unload_bsx();          break;
    case ModeSufamiTurbo: unload_sufami_turbo(); break;
  }

  if(cart.rom) { delete[] cart.rom; cart.rom = 0; }
  if(cart.ram) { delete[] cart.ram; cart.ram = 0; }
  if(cart.rtc) { delete[] cart.rtc; cart.rtc = 0; }
  if(bs.ram)   { delete[] bs.ram;   bs.ram   = 0; }
  if(stA.rom)  { delete[] stA.rom;  stA.rom  = 0; }
  if(stA.ram)  { delete[] stA.ram;  stA.ram  = 0; }
  if(stB.rom)  { delete[] stB.rom;  stB.rom  = 0; }
  if(stB.ram)  { delete[] stB.ram;  stB.ram  = 0; }

  string cheat_file = get_filename(cart.filename, "cht", snes.config.path.cheat);
  if(cheat.count() > 0 || file::exists(cheat_file)) {
    cheat.save(cheat_file);
    cheat.clear();
  }

  set(loaded, false);
}

Cartridge::Cartridge() {
  set(loaded, false);
}

Cartridge::~Cartridge() {
  if(loaded() == true) unload();
}

void Cartridge::set_cartinfo(const Cartridge::cartinfo_t &source) {
  set(region,         source.region);
  set(mapper,         source.mapper);
  set(dsp1_mapper,    source.dsp1_mapper);

  set(has_bsx_slot,   source.bsx_slot);
  set(has_superfx,    source.superfx);
  set(has_sa1,        source.sa1);
  set(has_srtc,       source.srtc);
  set(has_sdd1,       source.sdd1);
  set(has_spc7110,    source.spc7110);
  set(has_spc7110rtc, source.spc7110rtc);
  set(has_cx4,        source.cx4);
  set(has_dsp1,       source.dsp1);
  set(has_dsp2,       source.dsp2);
  set(has_dsp3,       source.dsp3);
  set(has_dsp4,       source.dsp4);
  set(has_obc1,       source.obc1);
  set(has_st010,      source.st010);
  set(has_st011,      source.st011);
  set(has_st018,      source.st018);
}

//==========
//cartinfo_t
//==========

void Cartridge::cartinfo_t::reset() {
  type        = TypeUnknown;
  mapper      = LoROM;
  dsp1_mapper = DSP1Unmapped;
  region      = NTSC;

  rom_size = 0;
  ram_size = 0;

  bsx_slot   = false;
  superfx    = false;
  sa1        = false;
  srtc       = false;
  sdd1       = false;
  spc7110    = false;
  spc7110rtc = false;
  cx4        = false;
  dsp1       = false;
  dsp2       = false;
  dsp3       = false;
  dsp4       = false;
  obc1       = false;
  st010      = false;
  st011      = false;
  st018      = false;
}

Cartridge::cartinfo_t::cartinfo_t() {
  reset();
}

//=======
//utility
//=======

//ensure file path is absolute (eg resolve relative paths)
string Cartridge::filepath(const char *filename, const char *pathname) {
  //if no pathname, return filename as-is
  string file(filename);
  file.replace("\\", "/");

  string path = (!pathname || !*pathname) ? (const char*)snes.config.path.current : pathname;
  //ensure path ends with trailing '/'
  path.replace("\\", "/");
  if(!strend(path, "/")) path.append("/");

  //replace relative path with absolute path
  if(strbegin(path, "./")) {
    ltrim(path, "./");
    path = string() << snes.config.path.base << path;
  }

  //remove folder part of filename
  lstring part;
  part.split("/", file);
  return path << part[part.size() - 1];
}

//remove directory information and file extension ("/foo/bar.ext" -> "bar")
string Cartridge::basename(const char *filename) {
  string name(filename);

  //remove extension
  for(signed i = strlen(name) - 1; i >= 0; i--) {
    if(name[i] == '.') {
      name[i] = 0;
      break;
    }
  }

  //remove directory information
  for(signed i = strlen(name) - 1; i >= 0; i--) {
    if(name[i] == '/' || name[i] == '\\') {
      i++;
      char *output = name();
      while(true) {
        *output++ = name[i];
        if(!name[i]) break;
        i++;
      }
      break;
    }
  }

  return name;
}

//remove filename and return path only ("/foo/bar.ext" -> "/foo/bar/")
string Cartridge::basepath(const char *filename) {
  string path(filename);
  path.replace("\\", "/");

  //remove filename
  for(signed i = strlen(path) - 1; i >= 0; i--) {
    if(path[i] == '/') {
      path[i] = 0;
      break;
    }
  }

  if(!strend(path, "/")) path.append("/");
  return path;
}
