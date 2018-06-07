auto Program::statePath() -> string {
  if(!emulator->loaded()) return "";
  return path("States", superNintendo.location, ".bst/");
}

auto Program::loadState(string filename) -> bool {
  if(!emulator->loaded()) return false;
  string location = {statePath(), filename, ".bst"};
  string prefix = Location::prefix(location);
  if(!file::exists(location)) return showMessage({"[", prefix, "] not found"}), false;
  if(filename != "Quick/Recovery Slot") saveRecoveryState();
  auto memory = file::read(location);
  serializer s{memory.data(), memory.size()};
  if(!emulator->unserialize(s)) return showMessage({"[", prefix, "] is in incompatible format"}), false;
  return showMessage({"Loaded [", prefix, "]"}), true;
}

auto Program::saveState(string filename) -> bool {
  if(!emulator->loaded()) return false;
  directory::create({statePath(), "Quick/"});
  string location = {statePath(), filename, ".bst"};
  string prefix = Location::prefix(location);
  serializer s = emulator->serialize();
  if(!s.size()) return showMessage({"Failed to save [", prefix, "]"}), false;
  if(!file::write(location, s.data(), s.size())) return showMessage({"Unable to write [", prefix, "] to disk"}), false;
  return showMessage({"Saved [", prefix, "]"}), true;
}

auto Program::saveRecoveryState() -> bool {
  if(!emulator->loaded()) return false;
  directory::create({statePath(), "Quick/"});
  serializer s = emulator->serialize();
  if(!s.size()) return false;
  if(!file::write({statePath(), "Quick/Recovery Slot.bst"}, s.data(), s.size())) return false;
  return true;
}
