auto Program::loadState(uint slot) -> bool {
  if(!emulator->loaded()) return false;
  auto location = path("States", superNintendo.location, {".bs", slot});
  if(!file::exists(location)) return showMessage({"Slot ", slot, " state does not exist"}), false;
  auto memory = file::read(location);
  serializer s{memory.data(), memory.size()};
  if(!emulator->unserialize(s)) return showMessage({"Slot ", slot, " state is incompatible"}), false;
  return showMessage({"Loaded state from slot ", slot}), true;
}

auto Program::saveState(uint slot) -> bool {
  if(!emulator->loaded()) return false;
  auto location = path("States", superNintendo.location, {".bs", slot});
  serializer s = emulator->serialize();
  if(!s.size()) return showMessage({"Failed to save state to slot ", slot}), false;
  if(!file::write(location, s.data(), s.size())) return showMessage({"Unable to write state to slot ", slot}), false;
  return showMessage({"Saved state to slot ", slot}), true;
}
