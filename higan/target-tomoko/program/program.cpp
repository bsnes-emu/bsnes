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

Program::Program(lstring args) {
  program = this;
  directory::create({configpath(), "tomoko/"});
  Application::onMain({&Program::main, this});
  Application::Windows::onModalChange([](bool modal) { if(modal && audio) audio->clear(); });

  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new GameBoy::Interface);
  emulators.append(new GameBoyAdvance::Interface);
  for(auto& emulator : emulators) emulator->bind = this;

  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new Presentation;

  presentation->setVisible();

  video = Video::create(settings["Video/Driver"].text());
  video->set(Video::Handle, presentation->viewport.handle());
  video->set(Video::Synchronize, settings["Video/Synchronize"].boolean());
  if(!video->init()) {
    delete video;
    video = Video::create("None");
  }

  audio = Audio::create(settings["Audio/Driver"].text());
  audio->set(Audio::Device, settings["Audio/Device"].text());
  audio->set(Audio::Handle, presentation->viewport.handle());
  audio->set(Audio::Synchronize, settings["Audio/Synchronize"].boolean());
  audio->set(Audio::Frequency, 96000u);
  audio->set(Audio::Latency, 80u);
  if(!audio->init()) {
    delete audio;
    audio = Audio::create("None");
  }

  input = Input::create(settings["Input/Driver"].text());
  input->set(Input::Handle, presentation->viewport.handle());
  input->onChange({&InputManager::onChange, inputManager});
  if(!input->init()) {
    delete input;
    input = Input::create("None");
  }

  dsp.setPrecision(16);
  dsp.setBalance(0.0);
  dsp.setFrequency(32040);
  dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(96000);

  presentation->drawSplashScreen();

  updateVideoFilter();
  updateAudioVolume();

  args.takeFirst();  //ignore program location in argument parsing
  for(auto& argument : args) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else {
      auto location = argument;
      if(file::exists(location)) location = dirname(location);
      if(directory::exists(location)) loadMedia(location);
    }
  }
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
  settings.quit();
  inputManager->quit();
  delete video;
  delete audio;
  delete input;
  Application::quit();
}
