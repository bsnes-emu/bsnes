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

string Path::load(SNES::Cartridge::Slot slot, const string &hint) {
  string basePath = basepath(slot);
  string baseName = notdir(basePath);
  string filePath = dir(basePath);

  if(hint == ".srm" && srm != "") filePath = srm;
  if(hint == ".bsp" && bsp != "") filePath = bsp;
  if(hint == ".bss" && bss != "") filePath = bss;
  if(hint == ".sts" && sts != "") filePath = sts;
  if(hint == ".sav" && sav != "") filePath = sav;
  if(hint == ".rtc" && rtc != "") filePath = rtc;

  if(hint.endswith(".dsp") && firmware != "") filePath = firmware;
  if(hint.endswith(".msu") && msu1     != "") filePath = msu1;
  if(hint.endswith(".pcm") && msu1     != "") filePath = msu1;
  if(hint.endswith(".so")  && serial   != "") filePath = serial;

  if(hint.endswith(".bsa") && bsa != "") filePath = bsa;
  if(hint.endswith(".bst") && bst != "") filePath = bst;
  if(hint.endswith(".cht") && cht != "") filePath = cht;
  if(hint.endswith(".log") && log != "") filePath = log;
  if(hint.endswith(".bmp") && bmp != "") filePath = bmp;

  filePath = decode(filePath, basePath);
  return { filePath, baseName, hint };
}

string Path::basepath(SNES::Cartridge::Slot slot) {
  if(slot == SNES::Cartridge::Slot::Base) {
    return cartridge.baseName;
  }

  if(slot == SNES::Cartridge::Slot::Bsx) {
    if(cartridge.bsxName == "") return cartridge.baseName;
    return cartridge.bsxName;
  }

  if(slot == SNES::Cartridge::Slot::SufamiTurbo) {
    if(cartridge.sufamiTurboAName == "" && cartridge.sufamiTurboBName == "") return cartridge.baseName;
    if(cartridge.sufamiTurboAName != "" && cartridge.sufamiTurboBName == "") return cartridge.sufamiTurboAName;
    if(cartridge.sufamiTurboAName == "" && cartridge.sufamiTurboBName != "") return cartridge.sufamiTurboBName;
    return { cartridge.sufamiTurboAName, "+", notdir(cartridge.sufamiTurboBName) };
  }

  if(slot == SNES::Cartridge::Slot::SufamiTurboA) {
    if(cartridge.sufamiTurboAName == "") return cartridge.baseName;
    return cartridge.sufamiTurboAName;
  }

  if(slot == SNES::Cartridge::Slot::SufamiTurboB) {
    if(cartridge.sufamiTurboBName == "") return cartridge.baseName;
    return cartridge.sufamiTurboBName;
  }

  if(slot == SNES::Cartridge::Slot::GameBoy) {
    if(cartridge.gameBoyName == "") return cartridge.baseName;
    return cartridge.gameBoyName;
  }

  throw "Path::basepath(): invalid slot ID.";
}

string Path::decode(const string &filePath, const string &basePath) {
  string path = filePath;

  if(path.beginswith("user/")) {
    path.ltrim<1>("user/");
    #if defined(PLATFORM_X) || defined(PLATFORM_OSX)
    path = { user, ".config/bsnes/", path };
    #else
    path = { user, "bsnes/", path };
    #endif
  } else if(path.beginswith("base/")) {
    path.ltrim<1>("base/");
    path = { base, path };
  } else if(path.beginswith("./")) {
    path.ltrim<1>("./");
    path = { dir(basePath), path };
  } else if(path.beginswith("../")) {
    string base = dir(basePath);
    base.rtrim<1>("/");
    path.ltrim<1>("../");
    path = { dir(base), path };
  }

  return path;
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

  attach(firmware = "", "firmware");
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
  attach(bmp = "", "bmp");
}
