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

  if(path == "filter") value = filter;
  if(path == "shader") value = shader;

  if(value.beginswith(":")) value.ltrim<1>(":");
  if(value == "") value = base;
  return value;
}

void Path::save(const string &path, const string &value) {
  string *output = 0;

  if(path == "sfc") output = &sfc;
  if(path == "bs") output = &bs;
  if(path == "st") output = &st;
  if(path == "gb") output = &gb;

  if(path == "filter") output = &filter;
  if(path == "shader") output = &shader;

  if(output) {
    string &s = *output;
    if(s.beginswith(":") == false && s != "") return;
    s = { ":", value };
    return;
  }
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

  attach(filter = "", "filter");
  attach(shader = "", "shader");

  attach(satellaviewBios = "", "satellaviewBios");
  attach(sufamiTurboBios = "", "sufamiTurboBios");
  attach(superGameBoyBios = "", "superGameBoyBios");
}
