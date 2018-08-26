#include "../higan.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <ms/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <pce/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <ws/interface/interface.hpp>
#include "platform.cpp"
#include "game.cpp"
#include "state.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(vector<string> arguments) {
  program = this;

  Emulator::platform = this;

#ifdef CORE_FC
  emulators.append(new Famicom::Interface);
#endif
#ifdef CORE_SFC
  emulators.append(new SuperFamicom::Interface);
#endif
#ifdef CORE_MS
  emulators.append(new MasterSystem::MasterSystemInterface);
#endif
#ifdef CORE_MD
  emulators.append(new MegaDrive::Interface);
#endif
#ifdef CORE_PCE
  emulators.append(new PCEngine::PCEngineInterface);
#endif
#ifdef CORE_PCE
  emulators.append(new PCEngine::SuperGrafxInterface);
#endif
#ifdef CORE_GB
  emulators.append(new GameBoy::GameBoyInterface);
#endif
#ifdef CORE_GB
  emulators.append(new GameBoy::GameBoyColorInterface);
#endif
#ifdef CORE_GBA
  emulators.append(new GameBoyAdvance::Interface);
#endif
#ifdef CORE_MS
  emulators.append(new MasterSystem::GameGearInterface);
#endif
#ifdef CORE_WS
  emulators.append(new WonderSwan::WonderSwanInterface);
#endif
#ifdef CORE_WS
  emulators.append(new WonderSwan::WonderSwanColorInterface);
#endif
#ifdef CORE_WS
  emulators.append(new WonderSwan::PocketChallengeV2Interface);
#endif

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
  new SystemProperties;
  new SettingsManager;
  new CheatDatabase;
  new ToolsManager;
  new AboutWindow;

  updateVideoShader();
  updateAudioDriver();
  updateAudioEffects();

  arguments.takeFirst();  //ignore program location in argument parsing
  for(auto& argument : arguments) {
    if(argument == "--fullscreen") {
      presentation->toggleFullScreen();
    } else if(directory::exists(argument.split("|", 1L).right())) {
      gameQueue.append(argument);
    } else if(file::exists(argument)) {
      if(auto result = execute("icarus", "--import", argument)) {
        gameQueue.append(result.output.strip());
      }
    }
  }
  if(gameQueue) load();

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  updateStatusText();
  video->poll();
  inputManager->poll();
  inputManager->pollHotkeys();

  if(!emulator
  || !emulator->loaded()
  || pause
  || (!focused() && settingsManager->input.pauseEmulation.checked())
  ) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
  if(settings["Emulation/AutoSaveMemory/Enable"].boolean()) {
    time_t currentTime = time(nullptr);
    if(currentTime - autoSaveTime >= settings["Emulation/AutoSaveMemory/Interval"].natural()) {
      autoSaveTime = currentTime;
      emulator->save();
    }
  }
}

auto Program::quit() -> void {
  hasQuit = true;
  unload();
  settings.save();
  inputManager->quit();
  video.reset();
  audio.reset();
  input.reset();
  Application::kill();
}
