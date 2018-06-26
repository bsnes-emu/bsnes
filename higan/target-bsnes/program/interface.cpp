#include <icarus/heuristics/heuristics.hpp>
#include <icarus/heuristics/heuristics.cpp>
#include <icarus/heuristics/super-famicom.cpp>
#include <icarus/heuristics/game-boy.cpp>
#include <icarus/heuristics/bs-memory.cpp>
#include <icarus/heuristics/sufami-turbo.cpp>

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  vfs::shared::file result;

  if(id == 0) {  //System
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(Resource::System::Manifest.data(), Resource::System::Manifest.size());
    }

    if(name == "boards.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(Resource::System::Boards.data(), Resource::System::Boards.size());
    }

    if(name == "ipl.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(Resource::System::IPLROM.data(), Resource::System::IPLROM.size());
    }
  }

  if(id == 1) {  //Super Famicom
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(superFamicom.manifest.data<uint8_t>(), superFamicom.manifest.size());
    } else if(superFamicom.location.endsWith("/")) {
      result = openPakSuperFamicom(name, mode);
    } else {
      result = openRomSuperFamicom(name, mode);
    }
  }

  if(id == 2) {  //Game Boy
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(gameBoy.manifest.data<uint8_t>(), gameBoy.manifest.size());
    } else if(gameBoy.location.endsWith("/")) {
      result = openPakGameBoy(name, mode);
    } else {
      result = openRomGameBoy(name, mode);
    }
  }

  if(id == 3) {  //BS Memory
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(bsMemory.manifest.data<uint8_t>(), bsMemory.manifest.size());
    } else if(bsMemory.location.endsWith("/")) {
      result = openPakBSMemory(name, mode);
    } else {
      result = openRomBSMemory(name, mode);
    }
  }

  if(id == 4) {  //Sufami Turbo - Slot A
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboA.manifest.data<uint8_t>(), sufamiTurboA.manifest.size());
    } else if(sufamiTurboA.location.endsWith("/")) {
      result = openPakSufamiTurboA(name, mode);
    } else {
      result = openRomSufamiTurboA(name, mode);
    }
  }

  if(id == 5) {  //Sufami Turbo - Slot B
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboB.manifest.data<uint8_t>(), sufamiTurboB.manifest.size());
    } else if(sufamiTurboB.location.endsWith("/")) {
      result = openPakSufamiTurboB(name, mode);
    } else {
      result = openRomSufamiTurboB(name, mode);
    }
  }

  if(!result && required) {
    MessageDialog({"Error: missing required data: ", name}).setParent(*presentation).error();
  }

  return result;
}

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
  BrowserDialog dialog;
  dialog.setOptions(options);

  if(id == 1 && name == "Super Famicom" && type == "sfc") {
    if(gameQueue) {
      superFamicom.location = gameQueue.takeLeft();
    } else {
      dialog.setTitle("Load Super Famicom");
      dialog.setPath(path("Games", settings["Path/Recent/SuperFamicom"].text()));
      dialog.setFilters({string{"Super Famicom Games|*.sfc:*.smc:*.zip"}});
      superFamicom.location = dialog.openObject();
    }
    if(inode::exists(superFamicom.location)) {
      settings["Path/Recent/SuperFamicom"].setValue(Location::dir(superFamicom.location));
      loadSuperFamicom(superFamicom.location);
      return {id, dialog.option()};
    }
  }

  if(id == 2 && name == "Game Boy" && type == "gb") {
    if(gameQueue) {
      gameBoy.location = gameQueue.takeLeft();
    } else {
      dialog.setTitle("Load Game Boy");
      dialog.setPath(path("Games", settings["Path/Recent/GameBoy"].text()));
      dialog.setFilters({string{"Game Boy Games|*.gb:*.gbc:*.zip"}});
      gameBoy.location = dialog.openObject();
    }
    if(inode::exists(gameBoy.location)) {
      settings["Path/Recent/GameBoy"].setValue(Location::dir(gameBoy.location));
      loadGameBoy(gameBoy.location);
      return {id, dialog.option()};
    }
  }

  if(id == 3 && name == "BS Memory" && type == "bs") {
    if(gameQueue) {
      bsMemory.location = gameQueue.takeLeft();
    } else {
      dialog.setTitle("Load BS Memory");
      dialog.setPath(path("Games", settings["Path/Recent/BSMemory"].text()));
      dialog.setFilters({string{"BS Memory Games|*.bs:*.zip"}});
      bsMemory.location = dialog.openObject();
    }
    if(inode::exists(bsMemory.location)) {
      settings["Path/Recent/BSMemory"].setValue(Location::dir(bsMemory.location));
      loadBSMemory(bsMemory.location);
      return {id, dialog.option()};
    }
  }

  if(id == 4 && name == "Sufami Turbo" && type == "st") {
    if(gameQueue) {
      sufamiTurboA.location = gameQueue.takeLeft();
    } else {
      dialog.setTitle("Load Sufami Turbo - Slot A");
      dialog.setPath(path("Games", settings["Path/Recent/SufamiTurboA"].text()));
      dialog.setFilters({string{"Sufami Turbo Games|*.st:*.zip"}});
      sufamiTurboA.location = dialog.openObject();
    }
    if(inode::exists(sufamiTurboA.location)) {
      settings["Path/Recent/SufamiTurboA"].setValue(Location::dir(sufamiTurboA.location));
      loadSufamiTurboA(sufamiTurboA.location);
      return {id, dialog.option()};
    }
  }

  if(id == 5 && name == "Sufami Turbo" && type == "st") {
    if(gameQueue) {
      sufamiTurboB.location = gameQueue.takeLeft();
    } else {
      dialog.setTitle("Load Sufami Turbo - Slot B");
      dialog.setPath(path("Games", settings["Path/Recent/SufamiTurboB"].text()));
      dialog.setFilters({string{"Sufami Turbo Games|*.st:*.zip"}});
      sufamiTurboB.location = dialog.openObject();
    }
    if(inode::exists(sufamiTurboB.location)) {
      settings["Path/Recent/SufamiTurboB"].setValue(Location::dir(sufamiTurboB.location));
      loadSufamiTurboB(sufamiTurboB.location);
      return {id, dialog.option()};
    }
  }

  return {};
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  pitch >>= 2;
  if(presentation->overscanCropping.checked()) {
    if(height == 240) data +=  8 * pitch, height -= 16;
    if(height == 480) data += 16 * pitch, height -= 32;
  }

  if(video->lock(output, length, width, height)) {
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
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
    showFrameRate({frameCounter, " FPS"});
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
