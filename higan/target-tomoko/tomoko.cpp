#include "tomoko.hpp"
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
unique_pointer<Emulator::Interface> emulator;

auto locate(string name) -> string {
  string location = {programpath(), name};
  if(file_system_object::exists(location)) return location;

  location = {configpath(), "higan/", name};
  if(file_system_object::exists(location)) return location;

  directory::create({localpath(), "higan/"});
  return {localpath(), "higan/", name};
}

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("higan");
  new Program(args);
  Application::run();
}
