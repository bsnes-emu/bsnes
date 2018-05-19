#include <icarus/heuristics/heuristics.hpp>
#include <icarus/heuristics/heuristics.cpp>
#include <icarus/heuristics/super-famicom.cpp>
#include <icarus/heuristics/game-boy.cpp>
#include <icarus/heuristics/bs-memory.cpp>
#include <icarus/heuristics/sufami-turbo.cpp>

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
//print("Open ", id, ": ", name, "\n");

  if(id == 0 && name == "manifest.bml") {
    return vfs::memory::file::open(systemManifest.data<uint8_t>(), systemManifest.size());
  }

  if(id == 0 && name == "boards.bml") {
    string location = {Path::local(), "higan/systems/Super Famicom.sys/boards.bml"};
    return vfs::fs::file::open(location, mode);
  }

  if(id == 0 && name == "ipl.rom") {
    return vfs::memory::file::open(systemIPLROM, 64);
  }

  if(id == 1 && name == "manifest.bml") {
    auto data = file::read(context.gameROM);
    Heuristics::SuperFamicom heuristics{data, context.gameROM};
    auto manifest = heuristics.manifest();
    return vfs::memory::file::open(manifest.data<uint8_t>(), manifest.size());
  }

  if(id == 1 && name == "program.rom") {
    return vfs::fs::file::open(context.gameROM, mode);
  }

  if(id == 1 && (name == "save.ram" || name == "upd96050.data.ram")) {
    return vfs::fs::file::open(context.gameRAM, mode);
  }

  return {};
}

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
//print("Load ", id, ": ", name, ", ", type, ", ", options.merge(";"), "\n");

  if(name == "Super Famicom" && type == "sfc") {
    return {1, "Auto"};
  }

  return {};
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  if(video->lock(output, length, width, height)) {
    pitch >>= 2;
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    video->unlock();
    video->output();
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
  inputManager->poll();
  if(auto mapping = inputManager->mapping(port, device, input)) {
    return mapping->poll();
  }
  return 0;
}
