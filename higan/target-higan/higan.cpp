#include "higan.hpp"
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
Emulator::Interface* emulator = nullptr;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  directory::create({Path::userData(), "higan/"});
  return {Path::userData(), "higan/", name};
}

auto hiro::initialize() -> void {
  Application::setName("higan");
  Application::setScreenSaver(false);
}

#include <nall/main.hpp>
auto nall::main(vector<string> arguments) -> void {
  new Program(arguments);
  Application::run();
}
