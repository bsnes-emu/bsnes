#include "../loki.hpp"
#include "interface.cpp"
#include "media.cpp"
unique_pointer<Program> program;

Program::Program(lstring args) {
  program = this;
  Application::onMain({&Program::main, this});

  emulator = new SuperFamicom::Interface;
  emulator->bind = this;

  new Terminal;
  new Presentation;
  terminal->setFocused();

  video = Video::create();
  video->set(Video::Handle, presentation->viewport.handle());
  video->set(Video::Synchronize, false);
  if(!video->init()) video = Video::create("None");

  audio = Audio::create();
  audio->set(Audio::Handle, presentation->viewport.handle());
  audio->set(Audio::Synchronize, true);
  audio->set(Audio::Frequency, 96000u);
  audio->set(Audio::Latency, 80u);
  if(!audio->init()) audio = Audio::create("None");

  input = Input::create();
  input->set(Input::Handle, presentation->viewport.handle());
  if(!input->init()) input = Input::create("None");

  presentation->drawSplashScreen();

  string location = args(1, "");
  if(!directory::exists(location)) location = {  //quick testing hack
    userpath(), "Emulation/Super Famicom/",
    "Legend of Zelda - A Link to the Past, The (USA) (1.0).sfc/"
  };

  if(directory::exists(location)) loadMedia(location);
}

auto Program::main() -> void {
  if(!emulator->loaded()) {
    usleep(20 * 1000);
    return;
  }

  devices = input->poll();
  emulator->run();
}

auto Program::quit() -> void {
  unloadMedia();
  video.reset();
  audio.reset();
  input.reset();
  Application::quit();
}
