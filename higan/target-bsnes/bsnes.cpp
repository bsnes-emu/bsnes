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
auto nall::main(vector<string> arguments) -> void {
  string locale;  // = "日本語";
  for(auto argument : arguments) {
    if(argument.beginsWith("--locale=")) {
      locale = argument.trimLeft("--locale=", 1L);
    }
  }
  Application::setScreenSaver(!settings["UserInterface/SuppressScreenSaver"].boolean());
  Application::locale().scan(locate("locales/"));
  Application::locale().select(locale);
  emulator = new SuperFamicom::Interface;
  program.create(arguments);
  Application::run();
}
