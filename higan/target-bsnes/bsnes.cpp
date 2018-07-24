#include "bsnes.hpp"
#include <sfc/interface/interface.hpp>
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
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

#include <nall/main.hpp>
auto nall::main(string_vector arguments) -> void {
  string locale;  // = "日本語";
  for(auto argument : arguments) {
    if(argument.beginsWith("--locale=")) {
      locale = argument.trimLeft("--locale=", 1L);
    }
  }
  Application::setName("bsnes");
  Application::setScreenSaver(false);
  Application::locale().scan(locate("locales/"));
  Application::locale().select(locale);
  emulator = new SuperFamicom::Interface;
  new Program(arguments);
  Application::run();
}
