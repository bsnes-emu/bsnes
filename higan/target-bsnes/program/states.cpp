auto Program::availableStates(string type) -> vector<string> {
  vector<string> result;
  if(!emulator->loaded()) return result;

  if(gamePath().endsWith("/")) {
    for(auto& file : directory::ifiles({statePath(), type}, "*.bst")) {
      result.append({type, file.trimRight(".bst", 1L)});
    }
  } else {
    Decode::ZIP input;
    if(input.open(statePath())) {
      vector<string> filenames;
      for(auto& file : input.file) {
        if(file.name.match({type, "*.bst"})) result.append(file.name.trimRight(".bst", 1L));
      }
    }
  }

  result.isort();
  return result;
}

auto Program::stateTimestamp(string filename) -> uint64_t {
  auto timestamp = chrono::timestamp();
  if(!emulator->loaded()) return timestamp;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    timestamp = file::timestamp(location, file::time::modify);
  } else {
    string location = {filename, ".bst"};
    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(file.name != location) continue;
        timestamp = file.timestamp;
        break;
      }
    }
  }

  return timestamp;
}

auto Program::loadState(string filename) -> bool {
  if(!emulator->loaded()) return false;

  string prefix = Location::file(filename);
  vector<uint8_t> memory;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
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
    if(filename != "quick/undo") saveUndoState();
    if(filename == "quick/undo") saveRedoState();
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

    struct State { string name; time_t timestamp; vector<uint8_t> memory; };
    vector<State> states;

    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(!file.name.endsWith(".bst")) continue;
        if(file.name == location) continue;
        states.append({file.name, file.timestamp, input.extract(file)});
      }
    }
    input.close();

    Encode::ZIP output{statePath()};
    for(auto& state : states) {
      output.append(state.name, state.memory.data(), state.memory.size(), state.timestamp);
    }
    output.append(location, s.data(), s.size());
  }

  if(filename.beginsWith("quick/")) presentation->updateStateMenus();
  return showMessage({"Saved [", prefix, "]"}), true;
}

auto Program::saveUndoState() -> bool {
  auto statusTime = this->statusTime;
  auto statusMessage = this->statusMessage;
  auto result = saveState("quick/undo");
  this->statusTime = statusTime;
  this->statusMessage = statusMessage;
  return result;
}

auto Program::saveRedoState() -> bool {
  auto statusTime = this->statusTime;
  auto statusMessage = this->statusMessage;
  auto result = saveState("quick/redo");
  this->statusTime = statusTime;
  this->statusMessage = statusMessage;
  return result;
}

auto Program::removeState(string filename) -> bool {
  if(!emulator->loaded()) return false;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    return file::remove(location);
  } else {
    bool found = false;
    string location = {filename, ".bst"};

    struct State { string name; time_t timestamp; vector<uint8_t> memory; };
    vector<State> states;

    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(file.name == location) { found = true; continue; }
        states.append({file.name, file.timestamp, input.extract(file)});
      }
    }
    input.close();

    if(states) {
      Encode::ZIP output{statePath()};
      for(auto& state : states) {
        output.append(state.name, state.memory.data(), state.memory.size(), state.timestamp);
      }
    } else {
      //remove .bsz file if there are no states left in the archive
      file::remove(statePath());
    }

    return found;
  }
}

auto Program::renameState(string from, string to) -> bool {
  if(!emulator->loaded()) return false;

  if(gamePath().endsWith("/")) {
    from = {statePath(), from, ".bst"};
    to = {statePath(), to, ".bst."};
    return file::rename(from, to);
  } else {
    bool found = false;
    from = {from, ".bst"};
    to = {to, ".bst"};

    struct State { string name; time_t timestamp; vector<uint8_t> memory; };
    vector<State> states;

    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(file.name == from) { found = true; file.name = to; }
        states.append({file.name, file.timestamp, input.extract(file)});
      }
    }
    input.close();

    Encode::ZIP output{statePath()};
    for(auto& state : states) {
      output.append(state.name, state.memory.data(), state.memory.size(), state.timestamp);
    }

    return found;
  }
}
