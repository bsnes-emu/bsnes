auto Program::openPakSuperFamicom(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return vfs::fs::file::open({superFamicom.location, name}, mode);
}

auto Program::openPakGameBoy(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return vfs::fs::file::open({gameBoy.location, name}, mode);
}

auto Program::openPakBSMemory(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return vfs::fs::file::open({bsMemory.location, name}, mode);
}

auto Program::openPakSufamiTurboA(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return vfs::fs::file::open({sufamiTurboA.location, name}, mode);
}

auto Program::openPakSufamiTurboB(string name, vfs::file::mode mode) -> shared_pointer<vfs::file> {
  return vfs::fs::file::open({sufamiTurboB.location, name}, mode);
}
