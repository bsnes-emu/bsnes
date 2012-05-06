#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>

void Application::bootstrap() {
  interface = new Interface;

  emulator.append(new Famicom::Interface);
  emulator.append(new SuperFamicom::Interface);
  emulator.append(new GameBoy::Interface);
  emulator.append(new GameBoyAdvance::Interface);

  for(auto &system : emulator) {
    system->callback.videoColor = {&Interface::videoColor, interface};
    system->callback.videoRefresh = {&Interface::videoRefresh, interface};
    system->callback.audioSample = {&Interface::audioSample, interface};
    system->callback.inputPoll = {&Interface::inputPoll, interface};
    system->callback.mediaRequest = {&Interface::mediaRequest, interface};
    system->callback.dipSettings = {&Interface::dipSettings, interface};
    system->callback.path = {&Interface::path, interface};

    for(auto &firmware : system->firmware) {
      filestream fs{application->path({firmware.name, ".", firmware.type, "/", firmware.path})};
      system->load(firmware.id, fs);
    }
  }
}
