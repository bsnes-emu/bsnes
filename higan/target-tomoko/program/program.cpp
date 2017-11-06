#include "../tomoko.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <ms/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <pce/interface/interface.hpp>
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

  Emulator::platform = this;
  emulators.append(new Famicom::Interface);
  emulators.append(new SuperFamicom::Interface);
  emulators.append(new MasterSystem::MasterSystemInterface);
  emulators.append(new MegaDrive::Interface);
  emulators.append(new PCEngine::PCEngineInterface);
  emulators.append(new PCEngine::SuperGrafxInterface);
  emulators.append(new GameBoy::GameBoyInterface);
  emulators.append(new GameBoy::GameBoyColorInterface);
  emulators.append(new GameBoyAdvance::Interface);
  emulators.append(new MasterSystem::GameGearInterface);
  emulators.append(new WonderSwan::WonderSwanInterface);
  emulators.append(new WonderSwan::WonderSwanColorInterface);

  new Presentation;
  presentation->setVisible();

  if(settings["Crashed"].boolean()) {
    MessageDialog().setText("Driver crash detected. Video/Audio/Input drivers have been disabled.").information();
    settings["Video/Driver"].setValue("None");
    settings["Audio/Driver"].setValue("None");
    settings["Input/Driver"].setValue("None");
  }

  settings["Crashed"].setValue(true);
  settings.save();

  initializeVideoDriver();
  initializeAudioDriver();
  initializeInputDriver();

  settings["Crashed"].setValue(false);
  settings.save();

  new InputManager;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new AboutWindow;

  updateVideoShader();
  updateAudioDriver();
  updateAudioEffects();

  args.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : args) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else if(directory::exists(argument.split("|", 1L).right())) {
      if(!argument.transform("\\", "/").endsWith("/")) argument.append("/");
      mediumQueue.append(argument);
    } else if(file::exists(argument)) {
      if(auto result = execute("icarus", "--import", argument)) {
        mediumQueue.append(result.output.strip());
      }
    }
  }
  loadMedium();

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  updateStatusText();
  inputManager->poll();
  inputManager->pollHotkeys();

  if(!emulator || !emulator->loaded() || pause || (!focused() && settings["Input/FocusLoss/Pause"].boolean())) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
  if(settings["Emulation/AutoSaveRAM"].boolean()) {
    time_t currentTime = time(nullptr);
    if(currentTime - autoSaveTime >= 5) {
      autoSaveTime = currentTime;
      emulator->save();
    }
  }
}

auto Program::quit() -> void {
  hasQuit = true;
  unloadMedium();
  settings.save();
  inputManager->quit();
  video.reset();
  audio.reset();
  input.reset();
  Application::quit();
}
