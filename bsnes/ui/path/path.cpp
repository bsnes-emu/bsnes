#include "../base.hpp"
Path path;

string Path::home(const string &filename) {
  string path = { config.path.base, filename };
  if(file::exists(path)) return path;

  path = config.path.user;
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
