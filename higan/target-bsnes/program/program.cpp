#include "../bsnes.hpp"
#include "data.cpp"
#include "interface.cpp"
#include "utility.cpp"
unique_pointer<Program> program;

Program::Program(string_vector args) {
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

auto Program::load(string location) -> void {
  if(!file::exists(location)) return;
  unload();

  context.gameROM = location;
  context.gameRAM = {Location::dir(location), Location::prefix(location), ".srm"};

  auto type = Location::suffix(location).trimLeft(".", 1L);
  for(auto& media : emulator->media) {
    if(media.type != type) continue;

    Emulator::audio.reset(2, audio->frequency());
    if(emulator->load(media.id)) {
      emulator->power();
      presentation->setTitle(emulator->title());
      presentation->reset.setEnabled(true);
      presentation->saveState.setEnabled(true);
      presentation->loadState.setEnabled(true);
    }

    break;
  }
}

auto Program::save() -> void {
  if(!emulator->loaded()) return;
  emulator->save();
}

auto Program::unload() -> void {
  if(!emulator->loaded()) return;
  emulator->unload();
  context = {};
  presentation->setTitle({"bsnes v", Emulator::Version});
  presentation->reset.setEnabled(false);
  presentation->saveState.setEnabled(false);
  presentation->loadState.setEnabled(false);
  presentation->clearViewport();
}
