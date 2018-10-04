const uint Program::State::Signature = 0x5a22'0000;

auto Program::availableStates(string type) -> vector<State> {
  vector<State> result;
  if(!emulator->loaded()) return result;

  if(gamePath().endsWith("/")) {
    for(auto& file : directory::ifiles({statePath(), type}, "*.bst")) {
      auto timestamp = file::timestamp({statePath(), type, file}, file::time::modify);
      result.append({{type, file.trimRight(".bst", 1L)}, timestamp});
    }
  } else {
    Decode::ZIP input;
    if(input.open(statePath())) {
      for(auto& file : input.file) {
        if(!file.name.match({type, "*.bst"})) continue;
        result.append({file.name.trimRight(".bst", 1L), (uint64_t)file.timestamp});
      }
    }
  }

  return result;
}

auto Program::hasState(string filename) -> bool {
  if(!emulator->loaded()) return false;

  if(gamePath().endsWith("/")) {
    return file::exists({statePath(), filename, ".bst"});
  } else {
    auto type = string{filename.split("/").first(), "/"};
    return (bool)availableStates(type).find([&](auto& state) { return state.name == filename; });
  }
}

auto Program::loadStateData(string filename) -> vector<uint8_t> {
  if(!emulator->loaded()) return {};

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

  if(memory.size() < 3 * sizeof(uint)) return {};  //too small to be a valid state file
  if(memory::readl<sizeof(uint)>(memory.data()) != State::Signature) return {};  //wrong format or version
  return memory;
}

auto Program::loadState(string filename) -> bool {
  string prefix = Location::file(filename);
  if(auto memory = loadStateData(filename)) {
    if(filename != "Quick/Undo") saveUndoState();
    if(filename == "Quick/Undo") saveRedoState();
    auto serializerRLE = Decode::RLE<1>({memory.data() + 3 * sizeof(uint), memory.size() - 3 * sizeof(uint)});
    serializer s{serializerRLE.data(), serializerRLE.size()};
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
  auto serializerRLE = Encode::RLE<1>({s.data(), s.size()});

  vector<uint8_t> previewRLE;
  //this can be null if a state is captured before the first frame of video output after power/reset
  if(screenshot.data) {
    image preview;
    preview.copy(screenshot.data, screenshot.pitch, screenshot.width, screenshot.height);
    if(preview.width() != 256 || preview.height() != 240) preview.scale(256, 240, true);
    preview.transform(0, 15, 0x8000, 0x7c00, 0x03e0, 0x001f);
    previewRLE = Encode::RLE<2>({preview.data(), preview.size()});
  }

  vector<uint8_t> saveState;
  saveState.resize(3 * sizeof(uint));
  memory::writel<sizeof(uint)>(saveState.data() + 0 * sizeof(uint), State::Signature);
  memory::writel<sizeof(uint)>(saveState.data() + 1 * sizeof(uint), serializerRLE.size());
  memory::writel<sizeof(uint)>(saveState.data() + 2 * sizeof(uint), previewRLE.size());
  saveState.append(serializerRLE);
  saveState.append(previewRLE);

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    directory::create(Location::path(location));
    if(!file::write(location, saveState)) {
      return showMessage({"Unable to write [", prefix, "] to disk"}), false;
    }
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
    output.append(location, saveState.data(), saveState.size());
  }

  if(filename.beginsWith("Quick/")) presentation.updateStateMenus();
  stateManager.stateEvent(filename);
  return showMessage({"Saved [", prefix, "]"}), true;
}

auto Program::saveUndoState() -> bool {
  auto statusTime = this->statusTime;
  auto statusMessage = this->statusMessage;
  auto result = saveState("Quick/Undo");
  this->statusTime = statusTime;
  this->statusMessage = statusMessage;
  return result;
}

auto Program::saveRedoState() -> bool {
  auto statusTime = this->statusTime;
  auto statusMessage = this->statusMessage;
  auto result = saveState("Quick/Redo");
  this->statusTime = statusTime;
  this->statusMessage = statusMessage;
  return result;
}

auto Program::removeState(string filename) -> bool {
  if(!emulator->loaded()) return false;
  bool result = false;

  if(gamePath().endsWith("/")) {
    string location = {statePath(), filename, ".bst"};
    result = file::remove(location);
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

    result = found;
  }

  if(result) {
    presentation.updateStateMenus();
    stateManager.stateEvent(filename);
  }
  return result;
}

auto Program::renameState(string from_, string to_) -> bool {
  if(!emulator->loaded()) return false;
  bool result = false;

  if(gamePath().endsWith("/")) {
    string from = {statePath(), from_, ".bst"};
    string to = {statePath(), to_, ".bst"};
    result = file::rename(from, to);
  } else {
    bool found = false;
    string from = {from_, ".bst"};
    string to = {to_, ".bst"};

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

    result = found;
  }

  if(result) {
    stateManager.stateEvent(to_);
  }
  return result;
}
