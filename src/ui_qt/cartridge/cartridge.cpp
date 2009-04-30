bool Cartridge::loadNormal(const char *base) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(base, data, size, true)) return false;
  unload();
  mode = SNES::Cartridge::ModeNormal;
  cartName = base;

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::cartridge.load_normal(data, size);
  delete[] data;

  loadMemory(SNES::memory::cartram, cartName, ".srm");
  loadMemory(SNES::memory::cartrtc, cartName, ".rtc");
  loadCheats(cartName);

  name = basename(cartName);
  utility.modifySystemState(Utility::LoadCartridge);
  return true;
}

bool Cartridge::loadBsxSlotted(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(base, data, size, true)) return false;
  unload();
  mode = SNES::Cartridge::ModeBsxSlotted;
  cartName = base;
  slotName[0] = slot;

  uint8_t *slotData;
  unsigned slotSize;
  loadFile(slot, slotData, slotSize, true);

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::cartridge.load_bsx_slotted(data, size, slotData, slotSize);
  delete[] data;
  if(slotData) delete[] slotData;

  loadMemory(SNES::memory::cartram, cartName, ".srm");
  loadCheats(cartName);

  name = basename(cartName);
  if(slotName[0] != "") name << " + " << basename(slotName[0]);
  utility.modifySystemState(Utility::LoadCartridge);
  return true;
}

bool Cartridge::loadBsx(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(base, data, size, true)) return false;
  unload();
  mode = SNES::Cartridge::ModeBsx;
  cartName = base;
  slotName[0] = slot;

  uint8_t *slotData;
  unsigned slotSize;
  loadFile(slot, slotData, slotSize, true);

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::cartridge.load_bsx(data, size, slotData, slotSize);
  delete[] data;
  if(slotData) delete[] slotData;

  loadMemory(SNES::bsxcart.sram,  cartName, ".srm");
  loadMemory(SNES::bsxcart.psram, cartName, ".psr");
  loadCheats(cartName);

  if(slotName[0] != "") name = basename(slotName[0]);
  else name = basename(cartName);
  utility.modifySystemState(Utility::LoadCartridge);
  return true;
}

bool Cartridge::loadSufamiTurbo(const char *base, const char *slotA, const char *slotB) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(base, data, size, true)) return false;
  unload();
  mode = SNES::Cartridge::ModeSufamiTurbo;
  cartName = base;
  slotName[0] = slotA;
  slotName[1] = slotB;

  uint8_t *slotData[2];
  unsigned slotSize[2];
  loadFile(slotA, slotData[0], slotSize[0], true);
  loadFile(slotB, slotData[1], slotSize[1], true);

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::cartridge.load_sufami_turbo(data, size, slotData[0], slotSize[0], slotData[1], slotSize[1]);
  delete[] data;
  if(slotData[0]) delete[] slotData[0];
  if(slotData[1]) delete[] slotData[1];

  loadMemory(SNES::memory::stAram, slotName[0], ".srm");
  loadMemory(SNES::memory::stBram, slotName[1], ".srm");
  loadCheats(cartName);

  if(slotName[0] != "" && slotName[1] != "") name = string() << basename(slotName[0]) << " + " << basename(slotName[1]);
  else if(slotName[0] != "") name = basename(slotName[0]);
  else if(slotName[1] != "") name = basename(slotName[1]);
  else name = basename(cartName);
  utility.modifySystemState(Utility::LoadCartridge);
  return true;
}

bool Cartridge::loadSuperGameboy(const char *base, const char *slot) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(base, data, size, true)) return false;
  unload();
  mode = SNES::Cartridge::ModeSuperGameboy;
  cartName = base;
  slotName[0] = slot;

  uint8_t *slotData;
  unsigned slotSize;
  loadFile(slot, slotData, slotSize, true);

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::cartridge.load_super_gameboy(data, size, slotData, slotSize);
  delete[] data;

  loadMemory(SNES::memory::dmgram, slotName[0], ".sav");
  loadMemory(SNES::memory::dmgrtc, slotName[0], ".rtc");
  loadCheats(cartName);

  if(slotName[0] != "") name = basename(slotName[0]);
  else name = basename(cartName);
  utility.modifySystemState(Utility::LoadCartridge);
  return true;
}

void Cartridge::unload() {
  switch(mode) {
    case SNES::Cartridge::ModeNormal:
    case SNES::Cartridge::ModeBsxSlotted: {
      saveMemory(SNES::memory::cartram, cartName, ".srm");
      saveMemory(SNES::memory::cartrtc, cartName, ".rtc");
    } break;

    case SNES::Cartridge::ModeBsx: {
      saveMemory(SNES::bsxcart.sram,  cartName, ".srm");
      saveMemory(SNES::bsxcart.psram, cartName, ".psr");
    } break;

    case SNES::Cartridge::ModeSufamiTurbo: {
      saveMemory(SNES::memory::stAram, slotName[0], ".srm");
      saveMemory(SNES::memory::stBram, slotName[1], ".srm");
    } break;

    case SNES::Cartridge::ModeSuperGameboy: {
      SNES::sgb.unload();
      saveMemory(SNES::memory::dmgram, slotName[0], ".sav");
      saveMemory(SNES::memory::dmgrtc, slotName[0], ".rtc");
    } break;
  };

  saveCheats(cartName);
  utility.modifySystemState(Utility::UnloadCartridge);
}

void Cartridge::loadCheats(const char *filename) {
  string name;
  name << (config.path.cheat != "" ? config.path.cheat : basepath(filename));
  name << basename(filename);
  name << ".cht";
  if(file::exists(name)) SNES::cheat.load(name);
}

void Cartridge::saveCheats(const char *filename) {
  string name;
  name << (config.path.cheat != "" ? config.path.cheat : basepath(filename));
  name << basename(filename);
  name << ".cht";
  if(file::exists(name) || SNES::cheat.count() > 0) SNES::cheat.save(name);
}

//

void Cartridge::loadMemory(SNES::MappedRAM &memory, const char *filename, const char *extension) {
  if(*filename && memory.size() > 0 && memory.size() != -1U) {
    string name;
    name << (config.path.save != "" ? config.path.save : basepath(filename));
    name << basename(filename);
    name << extension;

    uint8_t *data;
    unsigned size;
    if(loadFile(name, data, size, false)) {
      memcpy(memory.handle(), data, min(memory.size(), size));
      delete[] data;
    }
  }
}

void Cartridge::saveMemory(SNES::MappedRAM &memory, const char *filename, const char *extension) {
  if(*filename && memory.size() > 0 && memory.size() != -1U) {
    string name;
    name << (config.path.save != "" ? config.path.save : basepath(filename));
    name << basename(filename);
    name << extension;

    file fp;
    if(fp.open(name, file::mode_write)) {
      fp.write(memory.handle(), memory.size());
      fp.close();
    }
  }
}

bool Cartridge::loadFile(const char *name, uint8_t *&data, unsigned &size, bool compression) {
  data = 0;
  size = 0;

  switch(Reader::detect(name, config.file.autodetect_type)) {
    case Reader::Normal: {
      FileReader fp(name);
      if(!fp.ready()) return false;
      size = fp.size();
      data = fp.read();
    } break;

    #ifdef GZIP_SUPPORT
    case Reader::GZIP: {
      GZReader fp(name);
      if(!fp.ready()) return false;
      size = fp.size();
      data = fp.read();
    } break;

    case Reader::ZIP: {
      ZipReader fp(name);
      if(!fp.ready()) return false;
      size = fp.size();
      data = fp.read();
    } break;
    #endif

    #ifdef JMA_SUPPORT
    case Reader::JMA: {
      try {
        JMAReader fp(fp);
        size = fp.size();
        data = fp.read();
      } catch(JMA::jma_errors jma_error) {
        return false;
      }
    } break;
    #endif

    default: return false;
  }

  return true;
}

SNES::Cartridge::Type Cartridge::detectImageType(const char *filename) {
  uint8_t *data;
  unsigned size;
  if(!loadFile(filename, data, size, true)) return SNES::Cartridge::TypeUnknown;

  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);
  SNES::Cartridge::Type type = SNES::cartridge.detect_image_type(data, size);
  delete[] data;
  return type;
}

//

//ensure file path is absolute (eg resolve relative paths)
string Cartridge::filepath(const char *filename, const char *pathname) {
  //if no pathname, return filename as-is
  string file(filename);
  file.replace("\\", "/");

  string path = (!pathname || !*pathname) ? (const char*)config.path.current : pathname;
  //ensure path ends with trailing '/'
  path.replace("\\", "/");
  if(!strend(path, "/")) path.append("/");

  //replace relative path with absolute path
  if(strbegin(path, "./")) {
    ltrim(path, "./");
    path = string() << config.path.base << path;
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

//remove filename and return path only ("/foo/bar.ext" -> "/foo/")
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
