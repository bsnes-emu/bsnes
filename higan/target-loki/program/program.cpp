#include "../loki.hpp"
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
}

auto Program::load(string location) -> void {
}

auto Program::main() -> void {
}

auto Program::quit() -> void {
  video.reset();
  audio.reset();
  input.reset();
  Application::quit();
}
