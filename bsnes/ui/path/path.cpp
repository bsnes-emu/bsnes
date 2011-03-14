#include "../base.hpp"
Path path;

string Path::home(const string &filename) {
  string path = { base, filename };
  if(file::exists(path)) return path;

  path = user;
  #if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  path.append(".config/");
  mkdir(path, 0755);
  path.append("bsnes/");
  mkdir(path, 0755);
  #else
  path.append("bsnes/");
  mkdir(path, 0755);
  #endif
  path.append(filename);
  return path;
}

string Path::load(const string &path) {
  string value;

  if(path == "sfc") value = sfc;
  if(path == "bs") value = bs;
  if(path == "st") value = st;
  if(path == "gb") value = gb;

  if(value.beginswith("recent/")) value.ltrim<1>("recent/");
  if(value == "") value = base;
  return value;
}

void Path::save(const string &path, const string &value) {
  string *output = 0;

  if(path == "sfc") output = &sfc;
  if(path == "bs") output = &bs;
  if(path == "st") output = &st;
  if(path == "gb") output = &gb;

  if(output) {
    string &s = *output;
    if(s.beginswith("recent/") == false && s != "") return;
    s = { "recent/", value };
    return;
  }
}

string Path::load(SNES::Cartridge::Path pathHint, const string &hint) {
  string romPath;
  switch(pathHint) {
    case SNES::Cartridge::Path::Base: {
      romPath = cartridge.baseName;
      break;
    }
    case SNES::Cartridge::Path::Bsx: {
      romPath = cartridge.bsxName != "" ? cartridge.bsxName : cartridge.baseName;
      break;
    }
    case SNES::Cartridge::Path::SufamiTurbo: {
      if(cartridge.sufamiTurboAName == "" && cartridge.sufamiTurboBName == "") {
        romPath = cartridge.baseName;
      } else if(cartridge.sufamiTurboAName != "" && cartridge.sufamiTurboBName == "") {
        romPath = cartridge.sufamiTurboAName;
      } else if(cartridge.sufamiTurboAName == "" && cartridge.sufamiTurboBName != "") {
        romPath = cartridge.sufamiTurboBName;
      } else {
        romPath = { cartridge.sufamiTurboAName, "+", notdir(cartridge.sufamiTurboBName) };
      }
      break;
    }
    case SNES::Cartridge::Path::SufamiTurboA: {
      romPath = cartridge.sufamiTurboAName != "" ? cartridge.sufamiTurboAName : cartridge.baseName;
      break;
    }
    case SNES::Cartridge::Path::SufamiTurboB: {
      romPath = cartridge.sufamiTurboBName != "" ? cartridge.sufamiTurboBName : cartridge.baseName;
      break;
    }
    case SNES::Cartridge::Path::GameBoy: {
      romPath = cartridge.gameBoyName != "" ? cartridge.gameBoyName : cartridge.baseName;
      break;
    }

    case SNES::Cartridge::Path::NECDSP: romPath = cartridge.baseName; break;
    case SNES::Cartridge::Path::MSU1: romPath = cartridge.baseName; break;
    case SNES::Cartridge::Path::Serial: romPath = cartridge.baseName; break;
  }

  string path = romPath;
  if(pathHint == SNES::Cartridge::Path::NECDSP && necdsp != "") path = necdsp;
  if(pathHint == SNES::Cartridge::Path::MSU1 && msu1 != "") path = string(msu1, notdir(path));
  if(pathHint == SNES::Cartridge::Path::Serial && serial != "") path = string(serial, notdir(path));

  if(hint == ".srm" && srm != "") path = string(srm, notdir(path));
  if(hint == ".bsp" && bsp != "") path = string(bsp, notdir(path));
  if(hint == ".bss" && bss != "") path = string(bss, notdir(path));
  if(hint == ".sts" && sts != "") path = string(sts, notdir(path));
  if(hint == ".sav" && sav != "") path = string(sav, notdir(path));
  if(hint == ".rtc" && rtc != "") path = string(rtc, notdir(path));

  if(hint == ".bsa" && bsa != "") path = string(bsa, notdir(path));
  if(hint.endswith(".bst") && bst != "") path = string(bst, notdir(path));
  if(hint == ".cht" && cht != "") path = string(cht, notdir(path));
  if(hint == ".log" && log != "") path = string(log, notdir(path));

  if(path.beginswith("user/")) {
    path.ltrim<1>("user/");
    #if defined(PLATFORM_X) || defined(PLATFORM_OSX)
    path = string(user, ".config/bsnes/", path);
    #else
    path = string(user, "bsnes/", path);
    #endif
  } else if(path.beginswith("base/")) {
    path.ltrim<1>("base/");
    path = string(base, path);
  } else if(path.beginswith("./")) {
    path.ltrim<1>("./");
    path = string(dir(romPath), path);
  } else if(path.beginswith("../")) {
    string base = dir(romPath);
    base.rtrim<1>("/");
    path.ltrim<1>("../");
    path = string(dir(base), path);
  }

  if(pathHint == SNES::Cartridge::Path::NECDSP) return { dir(path), hint };
  if(pathHint == SNES::Cartridge::Path::MSU1) return { path, hint };
  if(pathHint == SNES::Cartridge::Path::Serial) return { path, hint };

  return { path, hint };
}

void Path::load() {
  configuration::load(home("paths.cfg"));
}

void Path::save() {
  configuration::save(home("paths.cfg"));
}

Path::Path() {
  attach(sfc = "", "sfc");
  attach(bs = "", "bs");
  attach(st = "", "st");
  attach(gb = "", "gb");

  attach(satellaviewBios = "", "satellaviewBios");
  attach(sufamiTurboBios = "", "sufamiTurboBios");
  attach(superGameBoyBios = "", "superGameBoyBios");

  attach(necdsp = "", "necdsp");
  attach(msu1 = "", "msu1");
  attach(serial = "", "serial");

  attach(srm = "", "srm");
  attach(rtc = "", "rtc");
  attach(bss = "", "bss");
  attach(bsp = "", "bsp");
  attach(sts = "", "sts");
  attach(sav = "", "sav");

  attach(bsa = "", "bsa");
  attach(bst = "", "bst");
  attach(cht = "", "cht");
  attach(log = "", "log");
}
