#include <nall/encode/bmp.hpp>
#include <icarus/heuristics/heuristics.hpp>
#include <icarus/heuristics/heuristics.cpp>
#include <icarus/heuristics/super-famicom.cpp>
#include <icarus/heuristics/game-boy.cpp>
#include <icarus/heuristics/bs-memory.cpp>
#include <icarus/heuristics/sufami-turbo.cpp>

//ROM data is held in memory to support compressed archives, soft-patching, and game hacks
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
    } else if(name == "program.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(superFamicom.program.data(), superFamicom.program.size());
    } else if(name == "data.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(superFamicom.data.data(), superFamicom.data.size());
    } else if(name == "expansion.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(superFamicom.expansion.data(), superFamicom.expansion.size());
    } else if(superFamicom.location.endsWith("/")) {
      result = openPakSuperFamicom(name, mode);
    } else {
      result = openRomSuperFamicom(name, mode);
    }
  }

  if(id == 2) {  //Game Boy
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(gameBoy.manifest.data<uint8_t>(), gameBoy.manifest.size());
    } else if(name == "program.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(gameBoy.program.data(), gameBoy.program.size());
    } else if(gameBoy.location.endsWith("/")) {
      result = openPakGameBoy(name, mode);
    } else {
      result = openRomGameBoy(name, mode);
    }
  }

  if(id == 3) {  //BS Memory
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(bsMemory.manifest.data<uint8_t>(), bsMemory.manifest.size());
    } else if(name == "program.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
    } else if(name == "program.flash") {
      //writes are not flushed to disk in bsnes
      result = vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
    } else if(bsMemory.location.endsWith("/")) {
      result = openPakBSMemory(name, mode);
    } else {
      result = openRomBSMemory(name, mode);
    }
  }

  if(id == 4) {  //Sufami Turbo - Slot A
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboA.manifest.data<uint8_t>(), sufamiTurboA.manifest.size());
    } else if(name == "program.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboA.program.data(), sufamiTurboA.program.size());
    } else if(sufamiTurboA.location.endsWith("/")) {
      result = openPakSufamiTurboA(name, mode);
    } else {
      result = openRomSufamiTurboA(name, mode);
    }
  }

  if(id == 5) {  //Sufami Turbo - Slot B
    if(name == "manifest.bml" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboB.manifest.data<uint8_t>(), sufamiTurboB.manifest.size());
    } else if(name == "program.rom" && mode == vfs::file::mode::read) {
      result = vfs::memory::file::open(sufamiTurboB.program.data(), sufamiTurboB.program.size());
    } else if(sufamiTurboB.location.endsWith("/")) {
      result = openPakSufamiTurboB(name, mode);
    } else {
      result = openRomSufamiTurboB(name, mode);
    }
  }

  if(!result && required) {
    if(MessageDialog({
      "Error: missing required data: ", name, "\n\n",
      "Would you like to view the online documentation for more information?"
    }).setParent(*presentation).error({"Yes", "No"}) == "Yes") {
      presentation->documentation.doActivate();
    }
  }

  return result;
}

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
  BrowserDialog dialog;
  dialog.setParent(*presentation);
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
      if(loadSuperFamicom(superFamicom.location)) {
        return {id, dialog.option()};
      }
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
      if(loadGameBoy(gameBoy.location)) {
        return {id, dialog.option()};
      }
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
      if(loadBSMemory(bsMemory.location)) {
        return {id, dialog.option()};
      }
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
      if(loadSufamiTurboA(sufamiTurboA.location)) {
        return {id, dialog.option()};
      }
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
      if(loadSufamiTurboB(sufamiTurboB.location)) {
        return {id, dialog.option()};
      }
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

  //this relies on the UI only running between Emulator::Scheduler::Event::Frame events
  //this will always be the case; so we can avoid an unnecessary copy or one-frame delay here
  //if the core were to exit between a frame event, the next frame might've been only partially rendered
  screenshot.data = data;
  screenshot.pitch = pitch << 2;
  screenshot.width = width;
  screenshot.height = height;

  if(video->lock(output, length, width, height)) {
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }

    video->unlock();
    video->output();
  }

  if(frameAdvance) {
    frameAdvance = false;
    presentation->pauseEmulation.setChecked();
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
  audio->output(samples);
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
