#include "../bsnes.hpp"
#include "interface.cpp"
#include "game.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(string_vector arguments) {
  program = this;
  Emulator::platform = this;

  new Presentation;
  presentation->setVisible();

  if(settings["Crashed"].boolean()) {
    MessageDialog().setText("Driver crash detected. Hardware drivers have been disabled.").information();
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
  new SettingsWindow;
  new AboutWindow;

  arguments.takeLeft();  //ignore program location in argument parsing
  for(auto& argument : arguments) {
    if(file::exists(argument)) {
      load(argument);
    }
  }

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  inputManager->poll();

  if(emulator->loaded() && !presentation->pauseEmulation.checked()) {
    emulator->run();
  } else {
    usleep(20 * 1000);
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
