#include <icarus/heuristics/heuristics.hpp>
#include <icarus/heuristics/heuristics.cpp>
#include <icarus/heuristics/super-famicom.cpp>
#include <icarus/heuristics/game-boy.cpp>
#include <icarus/heuristics/bs-memory.cpp>
#include <icarus/heuristics/sufami-turbo.cpp>

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  //System

  if(id == 0 && name == "manifest.bml" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(Resource::System::Manifest.data(), Resource::System::Manifest.size());
  }

  if(id == 0 && name == "boards.bml" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(Resource::System::Boards.data(), Resource::System::Boards.size());
  }

  if(id == 0 && name == "ipl.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(Resource::System::IPLROM.data(), Resource::System::IPLROM.size());
  }

  //Super Famicom

  if(id == 1 && name == "manifest.bml" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.manifest.data<uint8_t>(), superNintendo.manifest.size());
  }

  if(id == 1 && name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.program.data(), superNintendo.program.size());
  }

  if(id == 1 && name == "data.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.data.data(), superNintendo.data.size());
  }

  if(id == 1 && name == "expansion.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.expansion.data(), superNintendo.expansion.size());
  }

  if(id == 1 && name == "arm6.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x00000], 0x20000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "arm6.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x20000], 0x08000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "hg51bs169.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xc00) {
      return vfs::memory::file::open(superNintendo.firmware.data(), superNintendo.firmware.size());
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=HG51BS169)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "lr35902.boot.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x100) {
      return vfs::memory::file::open(superNintendo.firmware.data(), superNintendo.firmware.size());
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Boot,architecture=LR35902)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".boot.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "upd7725.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x0000], 0x1800);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "upd7725.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x1800], 0x0800);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "upd96050.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x0000], 0xc000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "upd96050.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0xc000], 0x1000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(id == 1 && name == "save.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".srm"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "download.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".psr"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "time.rtc") {
    string location = {Location::notsuffix(superNintendo.location), ".rtc"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "arm6.data.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".srm"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "hg51bs169.data.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".srm"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "upd7725.data.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".srm"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 1 && name == "upd96050.data.ram") {
    string location = {Location::notsuffix(superNintendo.location), ".srm"};
    return vfs::fs::file::open(location, mode);
  }

  //Game Boy

  if(id == 2 && name == "manifest.bml" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(gameBoy.manifest.data<uint8_t>(), gameBoy.manifest.size());
  }

  if(id == 2 && name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(gameBoy.program.data(), gameBoy.program.size());
  }

  if(id == 2 && name == "save.ram") {
    string location = {Location::path(gameBoy.location), Location::prefix(gameBoy.location), ".sav"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 2 && name == "time.rtc") {
    string location = {Location::path(gameBoy.location), Location::prefix(gameBoy.location), ".rtc"};
    return vfs::fs::file::open(location, mode);
  }

  return {};
}

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
  if(id == 1 && name == "Super Famicom" && type == "sfc") {
    if(gameQueue) {
      superNintendo.location = gameQueue.takeLeft();
    } else {
      BrowserDialog dialog;
      dialog.setTitle("Load Super Nintendo");
      dialog.setPath(settings["Path/Recent/SuperNintendo"].text());
      dialog.setFilters({string{"Super Nintendo Games|*.sfc:*.smc:*.zip"}});
      superNintendo.location = dialog.openFile();
    }
    if(file::exists(superNintendo.location)) {
      settings["Path/Recent/SuperNintendo"].setValue(Location::path(superNintendo.location));
      loadSuperNintendo(superNintendo.location);
      return {id, ""};
    }
  }

  if(id == 2 && name == "Game Boy" && type == "gb") {
    if(gameQueue) {
      gameBoy.location = gameQueue.takeLeft();
    } else {
      BrowserDialog dialog;
      dialog.setTitle("Load Game Boy");
      dialog.setPath(settings["Path/Recent/GameBoy"].text());
      dialog.setFilters({string{"Game Boy Games|*.gb:*.gbc:*.zip"}});
      gameBoy.location = dialog.openFile();
    }
    if(file::exists(gameBoy.location)) {
      settings["Path/Recent/GameBoy"].setValue(Location::path(gameBoy.location));
      loadGameBoy(gameBoy.location);
      return {id, ""};
    }
  }

  return {};
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  pitch >>= 2;
  if(presentation->overscanCropping.checked()) {
    data += 16 * pitch;
    height -= 32;
  }

  if(video->lock(output, length, width, height)) {
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    video->unlock();
    video->output();
  }

  static uint frameCounter = 0;
  static uint64 previous, current;
  frameCounter++;

  current = chrono::timestamp();
  if(current != previous) {
    previous = current;
    statusText = {"FPS: ", frameCounter};
    frameCounter = 0;
  }
}

auto Program::audioSample(const double* samples, uint channels) -> void {
  if(presentation->muteAudio.checked()) {
    static const double mutedSamples[] = {0.0, 0.0};
    audio->output(mutedSamples);
  } else {
    audio->output(samples);
  }
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(focused() || settingsWindow->input.allowInput().checked()) {
    inputManager->poll();
    if(auto mapping = inputManager->mapping(port, device, input)) {
      return mapping->poll();
    }
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(focused() || settingsWindow->input.allowInput().checked() || !enable) {
    if(auto mapping = inputManager->mapping(port, device, input)) {
      return mapping->rumble(enable);
    }
  }
}
