auto Program::stateName(unsigned slot, bool manager) -> string {
  return {
    folderPaths[0], "higan/states/",
    manager ? "managed/" : "quick/",
    "slot-", decimal(slot, 2L), ".bst"
  };
}

auto Program::loadState(unsigned slot, bool manager) -> bool {
  if(!emulator) return false;
  auto location = stateName(slot, manager);
  auto memory = file::read(location);
  if(memory.size() == 0) return showMessage({"Slot ", slot, " does not exist"}), false;
  serializer s(memory.data(), memory.size());
  if(emulator->unserialize(s) == false) return showMessage({"Slot ", slot, " state incompatible"}), false;
  return showMessage({"Loaded from slot ", slot}), true;
}

auto Program::saveState(unsigned slot, bool manager) -> bool {
  if(!emulator) return false;
  auto location = stateName(slot, manager);
  serializer s = emulator->serialize();
  if(s.size() == 0) return showMessage({"Failed to save state to slot ", slot}), false;
  directory::create(pathname(location));
  if(file::write(location, s.data(), s.size()) == false) {
    return showMessage({"Unable to write to slot ", slot}), false;
  }
  return showMessage({"Saved to slot ", slot}), true;
}
