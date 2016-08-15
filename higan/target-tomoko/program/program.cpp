#include "../tomoko.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <ws/interface/interface.hpp>
#include "interface.cpp"
#include "medium.cpp"
#include "state.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(string_vector args) {
  program = this;
  Application::onMain({&Program::main, this});

  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new MegaDrive::Interface);
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

  presentation->clearViewport();

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

  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new AboutWindow;

  presentation->setFocused();

  updateVideoShader();
  updateAudioDriver();
  updateAudioEffects();

  args.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : args) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else if(directory::exists(argument)) {
      mediumQueue.append(argument);
    } else if(file::exists(argument)) {
      if(auto result = execute("icarus", "--import", argument)) {
        mediumQueue.append(result.output.strip());
      }
    }
  }
  loadMedium();
}

auto Program::main() -> void {
  updateStatusText();
  inputManager->poll();
  inputManager->pollHotkeys();

  if(!emulator || !emulator->loaded() || pause || (!presentation->focused() && settings["Input/FocusLoss/Pause"].boolean())) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
}

auto Program::quit() -> void {
  hasQuit = true;
  unloadMedium();
  settings.quit();
  inputManager->quit();
  Application::quit();
}
