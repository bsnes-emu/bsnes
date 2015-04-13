auto Program::loadState(unsigned slot) -> bool {
  if(!activeEmulator) return false;
  auto memory = file::read({folderPaths[0], "higan/state-", slot, ".bst"});
  if(memory.size() == 0) return showMessage({"Slot ", slot, " does not exist"}), false;
  serializer s(memory.data(), memory.size());
  if(emulator().unserialize(s) == false) return showMessage({"Slot ", slot, " state incompatible"}), false;
  return showMessage({"Loaded from slot ", slot}), true;
}

auto Program::saveState(unsigned slot) -> bool {
  if(!activeEmulator) return false;
  serializer s = emulator().serialize();
  if(s.size() == 0) return showMessage({"Failed to save state to slot ", slot}), false;
  directory::create({folderPaths[0], "higan/"});
  if(file::write({folderPaths[0], "higan/state-", slot, ".bst"}, s.data(), s.size()) == false) {
    return showMessage({"Unable to write to slot ", slot}), false;
  }
  return showMessage({"Saved to slot ", slot}), true;
}
