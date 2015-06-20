#include "../tomoko.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include "interface.cpp"
#include "media.cpp"
#include "state.cpp"
#include "utility.cpp"
Program* program = nullptr;

Program::Program() {
  program = this;
  directory::create({configpath(), "tomoko/"});
  Application::onMain({&Program::main, this});
  Application::Windows::onModalChange([](bool modal) { if(modal && audio) audio->clear(); });

  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new GameBoy::Interface);
  emulators.append(new GameBoyAdvance::Interface);
  for(auto& emulator : emulators) emulator->bind = this;

  new ConfigurationManager;
  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new Presentation;

  presentation->setVisible();

  video = Video::create(config->video.driver);
  video->set(Video::Handle, presentation->viewport.handle());
  video->set(Video::Synchronize, config->video.synchronize);
  if(!video->init()) {
    delete video;
    video = Video::create("None");
  }

  audio = Audio::create(config->audio.driver);
  audio->set(Audio::Device, config->audio.device);
  audio->set(Audio::Handle, presentation->viewport.handle());
  audio->set(Audio::Synchronize, config->audio.synchronize);
  audio->set(Audio::Frequency, 96000u);
  audio->set(Audio::Latency, 80u);
  if(!audio->init()) {
    delete audio;
    audio = Audio::create("None");
  }

  input = Input::create(config->input.driver);
  input->set(Input::Handle, presentation->viewport.handle());
  input->onChange({&InputManager::onChange, inputManager});
  if(!input->init()) {
    delete input;
    input = Input::create("None");
  }

  dsp.setPrecision(16);
  dsp.setBalance(0.0);
  dsp.setVolume(config->audio.mute ? 0.0 : 1.0);
  dsp.setFrequency(32040);
  dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(96000);

  presentation->drawSplashScreen();

  updateVideoFilter();
}

auto Program::main() -> void {
  updateStatusText();
  inputManager->poll();

  if(!emulator || !emulator->loaded() || pause) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
}

auto Program::quit() -> void {
  unloadMedia();
  config->quit();
  inputManager->quit();
  delete video;
  delete audio;
  delete input;
  Application::quit();
}
