#include "../tomoko.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include "interface.cpp"
#include "media.cpp"
#include "utility.cpp"
Program* program = nullptr;

Program::Program() {
  program = this;
  directory::create({configpath(), "tomoko/"});
  Application::onMain({&Program::main, this});

  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new GameBoy::Interface);
  emulators.append(new GameBoyAdvance::Interface);
  for(auto& emulator : emulators) emulator->bind = this;

  new ConfigurationManager;
  new InputManager;
  new LibraryManager;
  new SettingsManager;
  new Presentation;

  presentation->setVisible();

  video.driver(config().video.driver);
  video.set(Video::Handle, presentation->viewport.handle());
  video.set(Video::Synchronize, config().video.synchronize);
  if(!video.init()) { video.driver("None"); video.init(); }

  audio.driver(config().audio.driver);
  audio.set(Audio::Handle, presentation->viewport.handle());
  audio.set(Audio::Synchronize, config().audio.synchronize);
  audio.set(Audio::Frequency, 96000u);
  audio.set(Audio::Latency, 80u);
  if(!audio.init()) { audio.driver("None"); audio.init(); }

  input.driver(config().input.driver);
  input.set(Input::Handle, presentation->viewport.handle());
  if(!input.init()) { input.driver("None"); input.init(); }

  dsp.setPrecision(16);
  dsp.setBalance(0.0);
  dsp.setVolume(config().audio.mute ? 0.0 : 1.0);
  dsp.setFrequency(32040);
  dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(96000);

  presentation->drawSplashScreen();

  updateVideoFilter();
}

auto Program::emulator() -> Emulator::Interface& {
  if(activeEmulator == nullptr) throw;
  return *activeEmulator;
}

auto Program::main() -> void {
  inputManager->poll();

  if(activeEmulator == nullptr || emulator().loaded() == false) {
    audio.clear();
    usleep(20 * 1000);
    return;
  }

  emulator().run();
}

auto Program::quit() -> void {
  unloadMedia();
  configurationManager->quit();
  inputManager->quit();
  video.term();
  audio.term();
  input.term();
  Application::quit();
}

auto Program::setEmulator(Emulator::Interface* emulator) -> void {
  activeEmulator = emulator;
}
