#include "../base.h"

void Cartridge::read_header() {
  cart.srtc = false;
  cart.sdd1 = false;
  cart.c4   = false;

  if(cart.header_index == 0x7fc0 && cart.rom_size >= 0x401000) {
    cart.mapper = EXLOROM;
  } else if(cart.header_index == 0x7fc0 && rom[cart.header_index + MAPPER] == 0x32) {
    cart.mapper = EXLOROM;
  } else if(cart.header_index == 0x7fc0) {
    cart.mapper = LOROM;
  } else if(cart.header_index == 0xffc0) {
    cart.mapper = HIROM;
  } else { //cart.header_index == 0x40ffc0
    cart.mapper = EXHIROM;
  }

uint8 mapper   = rom[cart.header_index + MAPPER];
uint8 rom_type = rom[cart.header_index + ROM_TYPE];
  if(mapper == 0x35 && rom_type == 0x55) {
    cart.srtc = true;
  }

  if(mapper == 0x32 && (rom_type == 0x43 || rom_type == 0x45)) {
    cart.sdd1 = true;
  }

  if(mapper == 0x20 && rom_type == 0xf3) {
    cart.c4 = true;
  }

  if(rom[cart.header_index + SRAM_SIZE] & 7) {
    cart.sram_size = 1024 << (rom[cart.header_index + SRAM_SIZE] & 7);
  } else {
    cart.sram_size = 0;
  }

  cart.region = (rom[cart.header_index + REGION] < 2) ? NTSC : PAL;

  memcpy(&cart.name, &rom[cart.header_index + CART_NAME], 21);
  cart.name[21] = 0;

  for(int i=0;i<22;i++) {
    if(cart.name[i] & 0x80) {
      cart.name[i] = '?';
    }
  }
}

void Cartridge::find_header() {
int32 score_lo = 0,
      score_hi = 0,
      score_ex = 0;

  if(rom_size < 0x010000) {
  //cart too small to be anything else
    cart.header_index = 0x007fc0;
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

uint16 cksum, icksum;
  cksum  = rom[0x7fc0 +  CKSUM] | (rom[0x7fc0 +  CKSUM + 1] << 8);
  icksum = rom[0x7fc0 + ICKSUM] | (rom[0x7fc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff)score_lo += 8;
  cksum  = rom[0xffc0 +  CKSUM] | (rom[0xffc0 +  CKSUM + 1] << 8);
  icksum = rom[0xffc0 + ICKSUM] | (rom[0xffc0 + ICKSUM + 1] << 8);
  if((cksum + icksum) == 0xffff)score_hi += 8;

  if(rom_size < 0x401000) {
    score_ex = 0;
  } else {
    if(rom[0x7fc0 + MAPPER] == 0x32)score_lo++;
    else score_ex += 16;
  }

  if(score_lo >= score_hi && score_lo >= score_ex) {
    cart.header_index = 0x007fc0;
  } else if(score_hi >= score_ex) {
    cart.header_index = 0x00ffc0;
  } else {
    cart.header_index = 0x40ffc0;
  }
}

void Cartridge::load_sram() {
  if(cart.sram_size == 0) {
    sram = 0;
    return;
  }

FileReader ff;
  if(!ff.open(sram_fn)) {
    sram = (uint8*)malloc(cart.sram_size);
    memset(sram, 0, cart.sram_size);
    return;
  }

  sram = ff.read(cart.sram_size);
  ff.close();
}

void Cartridge::save_sram() {
  if(cart.sram_size == 0)return;

FileWriter ff;
  if(!ff.open(sram_fn))return;

  ff.write(sram, cart.sram_size);
  ff.close();
}

void Cartridge::load_rom(Reader *rf) {
  base_rom = rf->read();
  rom_size = rf->size();

  if((rom_size & 0x7fff) == 0x0200) {
    rom = base_rom + 512;
    rom_size -= 512;
  } else {
    rom = base_rom;
  }

  cart.rom_size = rom_size;
}

bool Cartridge::load(const char *fn) {
  if(cart_loaded == true)return false;
  if(strlen(fn) < 3)return false;

  dprintf("* Loading \"%s\"...", fn);

  strcpy(rom_fn, fn);

  switch(Reader::detect(rom_fn)) {
  case Reader::RF_NORMAL: {
  FileReader ff;
    if(!ff.open(rom_fn)) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    load_rom(static_cast<Reader*>(&ff));
    ff.close();
    break;
  }
#ifdef GZIP_SUPPORT
  case Reader::RF_GZ: {
  GZReader gf;
    if(!gf.open(rom_fn)) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    load_rom(static_cast<Reader*>(&gf));
    gf.close();
    break;
  }
  case Reader::RF_ZIP: {
  ZipReader zf(rom_fn);
    load_rom(static_cast<Reader*>(&zf));
    break;
  }
#endif
#ifdef JMA_SUPPORT
  case Reader::RF_JMA: {
    try {
    JMAReader jf(rom_fn);
      load_rom(static_cast<Reader*>(&jf));
    } catch(JMA::jma_errors jma_error) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    break;
  }
#endif
  }

int i;
  strcpy(sram_fn, fn);
  for(i=strlen(fn)-1;i>=0;i--) {
    if(fn[i] == '.')break;
  }
  if(i != 0)sram_fn[i] = 0;
  strcat(sram_fn, ".srm");

  find_header();
  read_header();
  load_sram();
  cart_loaded = true;
  r_mem->load_cart();
  return true;
}

bool Cartridge::unload() {
  if(cart_loaded == false)return false;

  r_mem->unload_cart();

  if(base_rom) {
    zerofree(base_rom);
  }

  if(sram) {
    save_sram();
    zerofree(sram);
  }

  cart_loaded = false;
  return true;
}

Cartridge::Cartridge() {
  cart_loaded = false;

  base_rom = 0;
  rom      = 0;
  sram     = 0;
  rom_size = 0;
}

Cartridge::~Cartridge() {
  if(cart_loaded == true) {
    unload();
  }
}
