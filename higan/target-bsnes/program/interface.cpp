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
      result = vfs::memory::file::open(superNintendo.manifest.data<uint8_t>(), superNintendo.manifest.size());
    } else if(superNintendo.location.endsWith("/")) {
      result = openPakSFC(name, mode);
    } else {
      result = openRomSFC(name, mode);
    }
  }

  if(id == 2) {  //Game Boy
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(gameBoy.manifest.data<uint8_t>(), gameBoy.manifest.size());
    } else if(gameBoy.location.endsWith("/")) {
      result = openPakGB(name, mode);
    } else {
      result = openRomGB(name, mode);
    }
  }

  if(!result && required) {
    MessageDialog({"Error: missing required data: ", name}).setParent(*presentation).error();
  }

  return result;
}

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
  if(id == 1 && name == "Super Famicom" && type == "sfc") {
    if(gameQueue) {
      superNintendo.location = gameQueue.takeLeft();
    } else {
      BrowserDialog dialog;
      dialog.setTitle("Load Super Nintendo");
      dialog.setPath(path("Games", settings["Path/Recent/SuperNintendo"].text()));
      dialog.setFilters({string{"Super Nintendo Games|*.sfc:*.smc:*.zip"}});
      superNintendo.location = dialog.openObject();
    }
    if(inode::exists(superNintendo.location)) {
      settings["Path/Recent/SuperNintendo"].setValue(Location::dir(superNintendo.location));
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
      dialog.setPath(path("Games", settings["Path/Recent/GameBoy"].text()));
      dialog.setFilters({string{"Game Boy Games|*.gb:*.gbc:*.zip"}});
      gameBoy.location = dialog.openObject();
    }
    if(inode::exists(gameBoy.location)) {
      settings["Path/Recent/GameBoy"].setValue(Location::dir(gameBoy.location));
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
