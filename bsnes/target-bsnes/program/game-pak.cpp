namespace {

auto isMSU1File(const string& name) -> bool {
  if(name == "msu1/data.rom") return true;
  if(!name.beginsWith("msu1/track-") || !name.endsWith(".pcm")) return false;
  if(name.size() <= 15) return false;

  for(uint index = 11; index < name.size() - 4; index++) {
    if(name[index] < '0' || name[index] > '9') return false;
  }

  return true;
}

auto isLink(const string& name) -> bool {
  #if defined(PLATFORM_WINDOWS)
  auto attributes = GetFileAttributes(utf16_t(name));
  return attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_REPARSE_POINT;
  #else
  struct stat information = {};
  return lstat(name, &information) == 0 && S_ISLNK(information.st_mode);
  #endif
}

auto openPakFile(const string& location, string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  name.transform("\\", "/");
  if(!name || name.beginsWith("/") || name.contains(":")) return {};

  for(auto& component : name.split("/")) {
    if(!component || component == "." || component == "..") return {};
  }

  if(name.contains("/") && (mode != vfs::file::mode::read || !isMSU1File(name))) return {};

  string filename{location, name};
  if(mode != vfs::file::mode::read && isLink(filename)) return {};
  return vfs::fs::file::open(filename, mode);
}

}

auto Program::openPakSuperFamicom(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return openPakFile(superFamicom.location, name, mode);
}

auto Program::openPakGameBoy(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return openPakFile(gameBoy.location, name, mode);
}

auto Program::openPakBSMemory(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return openPakFile(bsMemory.location, name, mode);
}

auto Program::openPakSufamiTurboA(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return openPakFile(sufamiTurboA.location, name, mode);
}

auto Program::openPakSufamiTurboB(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return openPakFile(sufamiTurboB.location, name, mode);
}
