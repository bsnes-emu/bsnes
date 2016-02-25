#include "loki.hpp"
unique_pointer<Video> video;
unique_pointer<Audio> audio;
unique_pointer<Input> input;
Emulator::Interface* emulator = nullptr;

auto locate(string name) -> string {
  string location = {programpath(), name};
  if(inode::exists(location)) return location;

  location = {configpath(), "loki/", name};
  if(inode::exists(location)) return location;

  directory::create({localpath(), "loki/"});
  return {localpath(), "loki/", name};
}

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  Application::setName("loki");
  new Program(args);
  Application::run();
}
