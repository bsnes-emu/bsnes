#include "bsnes.hpp"
#include <sfc/interface/interface.hpp>
Video video;
Audio audio;
Input input;
unique_pointer<Emulator::Interface> emulator;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  if(name.beginsWith("database/")) {
    location = {Path::userData(), "icarus/", name};
    if(inode::exists(location)) return location;
  }

  directory::create({Path::userData(), "bsnes/"});
  return {Path::userData(), "bsnes/", name};
}

auto hiro::initialize() -> void {
  Application::setName("bsnes");
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  settings.location = locate("settings.bml");

  for(auto argument : arguments) {
    if(argument == "--fullscreen") {
      presentation.startFullScreen = true;
    } else if(argument.beginsWith("--locale=")) {
      Application::locale().scan(locate("locales/"));
      Application::locale().select(argument.trimLeft("--locale=", 1L));
    } else if(argument.beginsWith("--settings=")) {
      settings.location = argument.trimLeft("--settings=", 1L);
    } else if(inode::exists(argument)) {
      //game without option
      program.gameQueue.append({";", argument});
    } else if(argument.find(";")) {
      //game with option
      auto game = argument.split(";", 1L);
      if(inode::exists(game.last())) program.gameQueue.append(argument);
    }
  }

  settings.load();
  Application::setScreenSaver(settings.general.screenSaver);
  Application::setToolTips(settings.general.toolTips);
  emulator = new SuperFamicom::Interface;
  program.create();
  Application::run();
}
