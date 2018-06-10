#include "../bsnes.hpp"
#include "interface.cpp"
#include "game.cpp"
#include "paths.cpp"
#include "state.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(string_vector arguments) {
  program = this;
  Emulator::platform = this;

  new Presentation;
  presentation->setVisible();

  if(settings["Crashed"].boolean()) {
    MessageDialog(
      "Driver crash detected. Hardware drivers have been disabled.\n"
      "Please reconfigure drivers in the advanced settings panel."
    ).setParent(*presentation).information();
    settings["Video/Driver"].setValue("None");
    settings["Audio/Driver"].setValue("None");
    settings["Input/Driver"].setValue("None");
  }

  settings["Crashed"].setValue(true);
  settings.save();

  initializeVideoDriver();
  if(!video->ready()) {
    MessageDialog({
      "Error: failed to initialize ", settings["Video/Driver"].text(), " video driver."
    }).setParent(*presentation).error();
    settings["Video/Driver"].setValue("None");
    initializeVideoDriver();
  }
  initializeAudioDriver();
  if(!audio->ready()) {
    MessageDialog({
      "Error: failed to initialize ", settings["Audio/Driver"].text(), " audio driver."
    }).setParent(*presentation).error();
    settings["Audio/Driver"].setValue("None");
    initializeAudioDriver();
  }
  initializeInputDriver();
  if(!input->ready()) {
    MessageDialog({
      "Error: failed to initialize ", settings["Input/Driver"].text(), " input driver."
    }).setParent(*presentation).error();
    settings["Input/Driver"].setValue("None");
    initializeInputDriver();
  }

  settings["Crashed"].setValue(false);
  settings.save();

  new InputManager;
  new SettingsWindow;
  new CheatDatabase;
  new CheatWindow;
  new StateWindow;
  new ToolsWindow;
  new AboutWindow;

  arguments.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : arguments) {
    if(argument == "--fullscreen") {
      presentation->toggleFullscreenMode();
    } else if(file::exists(argument)) {
      gameQueue.append(argument);
    }
  }

  if(gameQueue) load();
  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  updateMessage();
  video->poll();
  inputManager->poll();
  inputManager->pollHotkeys();

  if(!emulator->loaded()
  || presentation->pauseEmulation.checked()
  || (!focused() && settingsWindow->input.pauseEmulation.checked())
  ) {
    audio->clear();
    usleep(20 * 1000);
    return;
  }

  emulator->run();
  if(settings["Emulator/AutoSaveMemory/Enable"].boolean()) {
    auto currentTime = chrono::timestamp();
    if(currentTime - autoSaveTime >= settings["Emulator/AutoSaveMemory/Interval"].natural()) {
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
  Application::quit();
}
