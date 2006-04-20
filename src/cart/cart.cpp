#include "../base.h"

void Cartridge::read_header() {
  cart.srtc = false;
  cart.sdd1 = false;
  cart.c4   = false;
  cart.dsp2 = false;
  cart.obc1 = false;

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

  if(mapper == 0x20 && rom_type == 0x05) {
    cart.dsp2 = true;
  }

  if(mapper == 0x30 && rom_type == 0x25) {
    cart.obc1 = true;
  }

  cart.cart_mmio = cart.c4 | cart.dsp2 | cart.obc1;

  if(rom[cart.header_index + SRAM_SIZE] & 7) {
    cart.sram_size = 1024 << (rom[cart.header_index + SRAM_SIZE] & 7);
  } else {
    cart.sram_size = 0;
  }

  cart.region = ((rom[cart.header_index + REGION] & 0x7f) < 2) ? NTSC : PAL;

  memcpy(&cart.name, &rom[cart.header_index + CART_NAME], 21);
  cart.name[21] = 0;

  for(int i = 0; i < 22; i++) {
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

FileReader ff(sram_fn);
  if(!ff.ready()) {
    sram = (uint8*)malloc(cart.sram_size);
    memset(sram, 0, cart.sram_size);
    return;
  }

  sram = ff.read(cart.sram_size);
}

void Cartridge::save_sram() {
  if(cart.sram_size == 0)return;

FileWriter ff(sram_fn);
  if(!ff.ready())return;

  ff.write(sram, cart.sram_size);
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

  cart.crc32 = 0xffffffff;
  for(int32 i = 0; i < cart.rom_size; i++) {
    cart.crc32 = crc32_adjust(cart.crc32, rom[i]);
  }
  cart.crc32 = ~cart.crc32;
}

void Cartridge::patch_rom(Reader *rf) {
uint8 *patch_data = rf->read();
uint32 patch_size = rf->size();
BPF bpf;
  if(patch_size < 34)return;

uint32 target;
  target  = patch_data[BPF::INDEX_FORMAT + 0] <<  0;
  target |= patch_data[BPF::INDEX_FORMAT + 1] <<  8;
  target |= patch_data[BPF::INDEX_FORMAT + 2] << 16;
  target |= patch_data[BPF::INDEX_FORMAT + 3] << 24;
  if(target != BPF::FORMAT_SNES) {
    alert("Warning: BPF patch file is not in SNES format!\n\n"
      "The patch will still be applied, but it will not be "
      "possible to determine whether the patch was created "
      "against an image with a header or without a header.\n\n"
      "bsnes is now forced to assume the patch was created "
      "against a headerless source image. If this is not the "
      "case, then patching will fail!\n\n"
      "If you are the author of this patch, please recreate "
      "the patch in SNES format.\n\n"
      "If you are not the patch author, please contact the "
      "author and ask them to create an SNES format BPF patch.");
  }

  if(bpf.apply_patch(patch_size, patch_data, rom_size, rom) == true) {
    SafeFree(base_rom);
  uint8 *temp = bpf.get_output_handle(rom_size);
    base_rom = (uint8*)malloc(rom_size);
    memcpy(base_rom, temp, rom_size);
    rom = base_rom;
    cart.rom_size = rom_size;
  } else {
    alert("Failed to apply patch.\n\nThis could be because the patch itself "
      "does not match its internal checksum, because the ROM loaded does not "
      "match the patch information for either the original or modified file, "
      "or because the patched file does not match the checksum of either the "
      "original or modified file that is stored inside the patch.\n\n"
      "The original ROM image will be used instead.");
  }
  SafeFree(patch_data);
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
    load_rom(static_cast<Reader*>(&ff));
    break;
  }
#ifdef GZIP_SUPPORT
  case Reader::RF_GZ: {
  GZReader gf(rom_fn);
    if(!gf.ready()) {
      alert("Error loading image file (%s)!", rom_fn);
      return false;
    }
    load_rom(static_cast<Reader*>(&gf));
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

//remove ROM extension
  strcpy(sram_fn, fn);
  for(int i = strlen(fn) - 1; i >= 0; i--) {
    if(sram_fn[i] == '.') {
      sram_fn[i] = 0;
      break;
    }
  }

//check for bpf patch
#ifdef GZIP_SUPPORT
  strcpy(patch_fn, sram_fn);
  strcat(patch_fn, ".bpz");
  if(fexists(patch_fn)) {
  ZipReader zf(patch_fn);
    patch_rom(static_cast<Reader*>(&zf));
  } else {
#endif
    strcpy(patch_fn, sram_fn);
    strcat(patch_fn, ".bpf");
    if(fexists(patch_fn)) {
    FileReader ff(patch_fn);
      patch_rom(static_cast<Reader*>(&ff));
    }
#ifdef GZIP_SUPPORT
  }
#endif

//add SRAM extension
  strcat(sram_fn, ".");
  strcat(sram_fn, config::fs.save_ext.sget());

//override default path (current directory)?
  if(strmatch(config::fs.save_path.sget(), "") == false) {
  //remove path if fs.sram_path was specified
  string new_fn, parts;
    strcpy(new_fn, sram_fn);
    replace(new_fn, "\\", "/");
    split(parts, "/", new_fn);

  //add new SRAM path
    strcpy(new_fn, config::fs.save_path.sget());

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
    cheat.load(static_cast<Reader*>(&ff));
  }

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
    SafeFree(base_rom);
  }

  if(sram) {
    save_sram();
    SafeFree(sram);
  }

  if(cheat.count() > 0 || fexists(cheat_fn)) {
  FileWriter ff(cheat_fn);
    cheat.save(static_cast<Writer*>(&ff));
    cheat.clear();
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
