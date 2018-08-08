#include "../bsnes.hpp"
#include "platform.cpp"
#include "game.cpp"
#include "game-pak.cpp"
#include "game-rom.cpp"
#include "paths.cpp"
#include "states.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "utility.cpp"
#include "patch.cpp"
#include "hacks.cpp"
Program program;

auto Program::create(vector<string> arguments) -> void {
  Emulator::platform = this;

  presentation.create();
  presentation.setVisible();
  aboutWindow.create();

  settingsWindow.create();
  videoSettings.create();
  audioSettings.create();
  inputSettings.create();
  hotkeySettings.create();
  pathSettings.create();
  emulatorSettings.create();
  driverSettings.create();

  toolsWindow.create();
  cheatDatabase.create();
  cheatWindow.create();
  cheatEditor.create();
  stateWindow.create();
  stateManager.create();
  manifestViewer.create();

  if(settings.general.crashed) {
    MessageDialog(
      "Driver crash detected. Hardware drivers have been disabled.\n"
      "Please reconfigure drivers in the advanced settings panel."
    ).setParent(*presentation).information();
    settings.video.driver = "None";
    settings.audio.driver = "None";
    settings.input.driver = "None";
  }

  settings.general.crashed = true;
  settings.save();
  updateVideoDriver(presentation);
  updateAudioDriver(presentation);
  updateInputDriver(presentation);
  settings.general.crashed = false;
  settings.save();

  driverSettings.videoDriverChanged();
  driverSettings.audioDriverChanged();
  driverSettings.inputDriverChanged();

  arguments.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : arguments) {
    if(argument == "--fullscreen") {
      presentation.toggleFullscreenMode();
    } else if(inode::exists(argument)) {
      gameQueue.append(argument);
    }
  }

  if(gameQueue) load();
  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  if(Application::modal()) return;

  updateStatus();
  video.poll();
  inputManager.poll();
  inputManager.pollHotkeys();

  if(paused()) {
    audio.clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
  if(emulatorSettings.autoSaveMemory.checked()) {
    auto currentTime = chrono::timestamp();
    if(currentTime - autoSaveTime >= settings.emulator.autoSaveMemory.interval) {
      autoSaveTime = currentTime;
      emulator->save();
    }
  }
}

auto Program::quit() -> void {
  unload();
  settings.save();
  video.reset();
  audio.reset();
  input.reset();
  Application::kill();
}
