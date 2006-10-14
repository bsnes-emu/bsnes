#include "../base.h"
#include "database.cpp"

Cartridge cartridge;

void Cartridge::read_dbi() {
  info.srtc = false;
  info.sdd1 = false;
  info.c4   = false;
  info.dsp1 = false;
  info.dsp2 = false;
  info.obc1 = false;

  info.dsp1_mapper = 0;

  info.header_index = 0x7fc0;
  info.mapper = PCB;
  strcpy(info.name, dbi.name);
  strcpy(info.pcb,  dbi.pcb);
  info.region = NTSC;
  info.cart_mmio = false;

  info.rom_size = dbi.rom;
  info.ram_size = dbi.ram;
}

void Cartridge::read_header() {
  info.srtc = false;
  info.sdd1 = false;
  info.c4   = false;
  info.dsp1 = false;
  info.dsp2 = false;
  info.obc1 = false;

  info.dsp1_mapper = 0;

  if(info.header_index == 0x7fc0 && info.rom_size >= 0x401000) {
    info.mapper = EXLOROM;
    strcpy(info.pcb, "UNL-EXLOROM");
  } else if(info.header_index == 0x7fc0 && rom[info.header_index + MAPPER] == 0x32) {
    info.mapper = EXLOROM;
    strcpy(info.pcb, "UNL-EXLOROM");
  } else if(info.header_index == 0x7fc0) {
    info.mapper = LOROM;
    strcpy(info.pcb, "UNL-LOROM");
  } else if(info.header_index == 0xffc0) {
    info.mapper = HIROM;
    strcpy(info.pcb, "UNL-HIROM");
  } else { //info.header_index == 0x40ffc0
    info.mapper = EXHIROM;
    strcpy(info.pcb, "UNL-EXHIROM");
  }

uint8 mapper   = rom[info.header_index + MAPPER];
uint8 rom_type = rom[info.header_index + ROM_TYPE];
  if(mapper == 0x35 && rom_type == 0x55) {
    info.srtc = true;
  }

  if(mapper == 0x32 && (rom_type == 0x43 || rom_type == 0x45)) {
    info.sdd1 = true;
  }

  if(mapper == 0x20 && rom_type == 0xf3) {
    info.c4 = true;
  }

  if((mapper == 0x20 || mapper == 0x21) && rom_type == 0x03) {
    info.dsp1 = true;
  }

  if(mapper == 0x30 && rom_type == 0x05) {
    info.dsp1 = true;
  }

  if(mapper == 0x31 && (rom_type == 0x03 || rom_type == 0x05)) {
    info.dsp1 = true;
  }

  if(info.dsp1 == true) {
    if((mapper & 0x2f) == 0x20 && info.rom_size <= 0x100000) {
      info.dsp1_mapper = DSP1_LOROM_1MB;
    } else if((mapper & 0x2f) == 0x20) {
      info.dsp1_mapper = DSP1_LOROM_2MB;
    } else if((mapper & 0x2f) == 0x21) {
      info.dsp1_mapper = DSP1_HIROM;
    }
  }

  if(mapper == 0x20 && rom_type == 0x05) {
    info.dsp2 = true;
  }

  if(mapper == 0x30 && rom_type == 0x25) {
    info.obc1 = true;
  }

  info.cart_mmio = info.c4 | info.dsp1 | info.dsp2 | info.obc1;

  if(rom[info.header_index + SRAM_SIZE] & 7) {
    info.ram_size = 1024 << (rom[info.header_index + SRAM_SIZE] & 7);
  } else {
    info.ram_size = 0;
  }

  memcpy(&info.name, &rom[info.header_index + CART_NAME], 21);
  info.name[21] = 0;

  for(int i = 0; i < 22; i++) {
    if(info.name[i] & 0x80) {
      info.name[i] = '?';
    }
  }
}

void Cartridge::find_header() {
int32 score_lo = 0,
      score_hi = 0,
      score_ex = 0;

  if(info.rom_size < 0x010000) {
  //cart too small to be anything but lorom
    info.header_index = 0x007fc0;
    return;
  }

  if((rom[0x7fc0 + MAPPER] & ~0x10) == 0x20)score_lo++;
  if((rom[0xffc0 + MAPPER] & ~0x10) == 0x21)score_hi++;

  if(rom[0x7fc0 + ROM_TYPE] < 0x08)score_lo++;
  if(rom[0xffc0 + ROM_TYPE] < 0x08)score_hi++;

  if(rom[0x7fc0 + ROM_SIZE] < 0x10)score_lo++;
  if(rom[0xffc0 + ROM_SIZE] < 0x10)score_hi++;

  if(rom[0x7fc0 + SRAM_SIZE] < 0x08)score_lo++;
  if(rom[0xffc0 + SRAM_SIZE] < 0x08)score_hi++;

  if(rom[0x7fc0 + REGION] < 14)score_lo++;
  if(rom[0xffc0 + REGION] < 14)score_hi++;

  if(rom[0x7fc0 + LICENSE] < 3)score_lo++;
  if(rom[0xffc0 + LICENSE] < 3)score_hi++;

  if(rom[0x7fc0 + RESH] & 0x80)score_lo += 2;
  if(rom[0xffc0 + RESH] & 0x80)score_hi += 2;

uint16 cksum, icksum;
  cksum  = rom[0x7fc0 +  CKSUM] | (rom[0x7fc0 +  CKSUM + 1] << 8);
  icksum = rom[0x7fc0 + ICKSUM] | (rom[0x7fc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff && (cksum != 0) && (icksum != 0)) {
    score_lo += 8;
  }

  cksum  = rom[0xffc0 +  CKSUM] | (rom[0xffc0 +  CKSUM + 1] << 8);
  icksum = rom[0xffc0 + ICKSUM] | (rom[0xffc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff && (cksum != 0) && (icksum != 0)) {
    score_hi += 8;
  }

  if(info.rom_size < 0x401000) {
    score_ex = 0;
  } else {
    if(rom[0x7fc0 + MAPPER] == 0x32)score_lo++;
    else score_ex += 16;
  }

  if(score_lo >= score_hi && score_lo >= score_ex) {
    info.header_index = 0x007fc0;
  } else if(score_hi >= score_ex) {
    info.header_index = 0x00ffc0;
  } else {
    info.header_index = 0x40ffc0;
  }
}

void Cartridge::load_sram() {
  if(info.ram_size == 0) {
    sram = 0;
    return;
  }

FileReader ff(sram_fn);
  if(!ff.ready()) {
    sram = (uint8*)malloc(info.ram_size);
    memset(sram, 0xff, info.ram_size);
    return;
  }

  sram = ff.read(info.ram_size);
}

void Cartridge::save_sram() {
  if(info.ram_size == 0)return;

FileWriter ff(sram_fn);
  if(!ff.ready())return;

  ff.write(sram, info.ram_size);
}

void Cartridge::load_rom(Reader &rf) {
uint size   = rf.size();
bool header = ((size & 0x7fff) == 512);
  info.rom_size = size - (header ? 512 : 0);
  if(info.rom_size & 0x7fff) {
    info.rom_size += 0x8000 - (info.rom_size & 0x7fff);
  }

uint8 *base_rom = rf.read(info.rom_size + (header ? 512 : 0));
  if(header) {
    memcpy(rom_header, base_rom, 512);
  } else {
    memset(rom_header, 0x00, 512);
  }

  rom = (uint8*)malloc(info.rom_size);
  memcpy(rom, base_rom + (header ? 512 : 0), info.rom_size);
  SafeFree(base_rom);

  info.crc32 = 0xffffffff;
  for(int32 i = 0; i < info.rom_size; i++) {
    info.crc32 = crc32_adjust(info.crc32, rom[i]);
  }
  info.crc32 = ~info.crc32;
}

void Cartridge::patch_rom(Reader &rf) {
UPS<ramfile, ramfile, ramfile> ups;
uint   patchsize = rf.size();
uint8 *patchdata = rf.read();

  fopen(ups.original, 0, file::mode_writeread);
  fopen(ups.modified, 0, file::mode_writeread);
  fopen(ups.patch.fp, 0, file::mode_writeread);

  fwrite(ups.original, rom, info.rom_size);
  fwrite(ups.patch.fp, patchdata, patchsize);

  if(ups.apply() == true) {
    info.crc32    = ups.modified_crc32;
    info.rom_size = ups.modified_filesize;
    rom = (uint8*)realloc(rom, info.rom_size);
    fseek(ups.modified, 0, file::seek_start);
    fread(ups.modified, rom, info.rom_size);
  }
}

bool Cartridge::load(const char *fn) {
  if(cart_loaded == true)return false;
  if(strlen(fn) < 3)return false;

  dprintf("* Loading \"%s\"...", fn);

  strcpy(rom_fn, fn);

  switch(Reader::detect(rom_fn)) {

  case Reader::RF_NORMAL: {
  FileReader ff(rom_fn);
    if(!ff.ready()) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    load_rom(ff);
  } break;

#ifdef GZIP_SUPPORT
  case Reader::RF_GZ: {
  GZReader gf(rom_fn);
    if(!gf.ready()) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    load_rom(gf);
  } break;

  case Reader::RF_ZIP: {
  ZipReader zf(rom_fn);
    load_rom(zf);
  } break;
#endif

#ifdef JMA_SUPPORT
  case Reader::RF_JMA: {
    try {
    JMAReader jf(rom_fn);
      load_rom(jf);
    } catch(JMA::jma_errors jma_error) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
  } break;
#endif

  }

//remove ROM extension
  strcpy(sram_fn, fn);
  for(int i = strlen(fn) - 1; i >= 0; i--) {
    if(sram_fn[i] == '.') {
      sram_fn[i] = 0;
      break;
    }
  }

//add SRAM extension
  strcat(sram_fn, ".");
  strcat(sram_fn, config::fs.save_ext.sget());

stringarray save_path;
  strcpy(save_path, config::fs.save_path.sget());
  replace(save_path, "\\", "/");
  if(strlen(save_path) && !strend(save_path, "/")) { strcat(save_path, "/"); }

  if(strlen(save_path) != 0) {
  //override default path (current directory)
  stringarray new_fn, parts;
    strcpy(new_fn, sram_fn);
    replace(new_fn, "\\", "/");
    split(parts, "/", new_fn);

  //add new SRAM path
    strcpy(new_fn, save_path);

  //append fs.base_path if fs.sram_path is not fully-qualified path
    if(strbegin(new_fn, "./") == true) {
      strltrim(new_fn, "./");
      strcpy(new_fn[1], new_fn[0]);
      strcpy(new_fn[0], config::fs.base_path.sget());
      strcat(new_fn[0], new_fn[1]);
    }

  //finally, append SRAM file name
    strcat(new_fn, parts[count(parts) - 1]);
    strcpy(sram_fn, strptr(new_fn));
  }

//load cheat file if it exists
  strcpy(cheat_fn, sram_fn);
  strrtrim(cheat_fn, config::fs.save_ext.sget());
  strrtrim(cheat_fn, ".");
  strcat(cheat_fn, ".cht");

  if(fexists(cheat_fn) == true) {
  FileReader ff(cheat_fn);
    cheat.load(ff);
  }

//load patch file if it exists
  strcpy(patch_fn, sram_fn);
  strrtrim(patch_fn, config::fs.save_ext.sget());
  strrtrim(patch_fn, ".");
  strcat(patch_fn, ".ups");

  if(fexists(patch_fn) == true) {
  FileReader ff(patch_fn);
    patch_rom(ff);
  }

#ifdef GZIP_SUPPORT
  else {
    strrtrim(patch_fn, ".ups");
    strcat(patch_fn, ".upz");
    if(fexists(patch_fn) == true) {
    ZipReader zf(patch_fn);
      patch_rom(zf);
    }
  }
#endif

  if(read_database() == true) {
    read_dbi();
  } else {
    find_header();
    read_header();
  }

  load_sram();
  cart_loaded = true;

  r_mem->load_cart();
  return true;
}

bool Cartridge::unload() {
  if(cart_loaded == false)return false;

  r_mem->unload_cart();

  if(sram) { save_sram(); }
  SafeFree(rom);
  SafeFree(sram);

  if(cheat.count() > 0 || fexists(cheat_fn)) {
  FileWriter ff(cheat_fn);
    cheat.save(ff);
    cheat.clear();
  }

  cart_loaded = false;
  return true;
}

Cartridge::Cartridge() {
  load_database();

  cart_loaded = false;

  rom  = 0;
  sram = 0;
}

Cartridge::~Cartridge() {
  if(cart_loaded == true) {
    unload();
  }
}
