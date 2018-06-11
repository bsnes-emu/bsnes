auto Program::openPakSFC(string name, vfs::file::mode mode) -> vfs::shared::file {
  return vfs::fs::file::open({superNintendo.location, name}, mode);
}

auto Program::openPakGB(string name, vfs::file::mode mode) -> vfs::shared::file {
  return vfs::fs::file::open({gameBoy.location, name}, mode);
}
