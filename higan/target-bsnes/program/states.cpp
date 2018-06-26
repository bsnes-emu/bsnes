auto Program::loadState(string filename) -> bool {
  if(!emulator->loaded()) return false;

  string prefix = Location::file(filename);
  vector<uint8_t> memory;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    if(!file::exists(location)) return showMessage({"[", prefix, "] not found"}), false;
    if(filename != "quick/recovery") saveRecoveryState();
    memory = file::read(location);
  } else {
    string location = {filename, ".bst"};
    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(file.name != location) continue;
        memory = input.extract(file);
        break;
      }
    }
  }

  if(memory) {
    serializer s{memory.data(), memory.size()};
    if(!emulator->unserialize(s)) return showMessage({"[", prefix, "] is in incompatible format"}), false;
    return showMessage({"Loaded [", prefix, "]"}), true;
  } else {
    return showMessage({"[", prefix, "] not found"}), false;
  }
}

auto Program::saveState(string filename) -> bool {
  if(!emulator->loaded()) return false;

  string prefix = Location::file(filename);
  serializer s = emulator->serialize();
  if(!s.size()) return showMessage({"Failed to save [", prefix, "]"}), false;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    directory::create(Location::path(location));
    if(!file::write(location, s.data(), s.size())) return showMessage({"Unable to write [", prefix, "] to disk"}), false;
  } else {
    string location = {filename, ".bst"};

    struct State {
      string name;
      vector<uint8_t> memory;
    };
    vector<State> states;

    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(!file.name.endsWith(".bst")) continue;
        if(file.name == location) continue;
        states.append({file.name, input.extract(file)});
      }
    }

    Encode::ZIP output{statePath()};
    for(auto& state : states) {
      output.append(state.name, state.memory.data(), state.memory.size());
    }
    output.append(location, s.data(), s.size());
  }

  return showMessage({"Saved [", prefix, "]"}), true;
}

auto Program::saveRecoveryState() -> bool {
  auto statusTime = this->statusTime;
  auto statusMessage = this->statusMessage;
  saveState("quick/recovery");
  this->statusTime = statusTime;
  this->statusMessage = statusMessage;
}
