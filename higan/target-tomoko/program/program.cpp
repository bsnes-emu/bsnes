#include "../tomoko.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <ws/interface/interface.hpp>
#include "interface.cpp"
#include "medium.cpp"
#include "state.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(lstring args) {
  program = this;
  Application::onMain({&Program::main, this});

  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new GameBoy::Interface);
  emulators.append(new GameBoyAdvance::Interface);
  emulators.append(new WonderSwan::Interface);
  for(auto& emulator : emulators) emulator->bind = this;

  new Presentation;
  presentation->setVisible();

  video = Video::create(settings["Video/Driver"].text());
  video->set(Video::Handle, presentation->viewport.handle());
  video->set(Video::Synchronize, settings["Video/Synchronize"].boolean());
  if(!video->init()) video = Video::create("None");

  audio = Audio::create(settings["Audio/Driver"].text());
  audio->set(Audio::Device, settings["Audio/Device"].text());
  audio->set(Audio::Handle, presentation->viewport.handle());
  audio->set(Audio::Synchronize, settings["Audio/Synchronize"].boolean());
  audio->set(Audio::Latency, 80u);
  if(!audio->init()) audio = Audio::create("None");

  input = Input::create(settings["Input/Driver"].text());
  input->set(Input::Handle, presentation->viewport.handle());
  input->onChange({&InputManager::onChange, &inputManager()});
  if(!input->init()) input = Input::create("None");

  presentation->drawSplashScreen();

  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;

  updateVideoShader();
  updateAudioDriver();
  updateAudioEffects();

  args.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : args) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else {
      load(argument);
    }
  }
}

auto Program::load(string location) -> void {
  if(directory::exists(location)) {
    loadMedium(location);
  } else if(file::exists(location)) {
    //special handling to allow importing the Game Boy Advance BIOS
    if(file::size(location) == 16384 && file::sha256(location).beginsWith("fd2547724b505f48")) {
      auto target = locate("Game Boy Advance.sys/");
      if(file::copy(location, {target, "bios.rom"})) {
        MessageDialog().setTitle(Emulator::Name).setText("Game Boy Advance BIOS imported successfully!").information();
      }
      return;
    }

    //ask icarus to import the game; and play it upon success
    if(auto result = execute("icarus", "--import", location)) {
      loadMedium(result.output.strip());
    }
  }
}

auto Program::main() -> void {
  updateStatusText();
  inputManager->poll();

  if(!emulator || !emulator->loaded() || pause || (!presentation->focused() && settings["Input/FocusLoss/Pause"].boolean())) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
}

auto Program::quit() -> void {
  unloadMedium();
  settings.quit();
  inputManager->quit();
  Application::quit();
}
