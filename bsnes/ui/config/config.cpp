#include "../base.hpp"
Config *config = 0;

Config::Config() {
  attach(video.driver = "", "Video::Driver");
  attach(video.shader = "", "Video::Shader");
  attach(video.synchronize = true, "Video::Synchronize");
  attach(video.smooth = false, "Video::Smooth");

  attach(audio.driver = "", "Audio::Driver");
  attach(audio.synchronize = true, "Audio::Synchronize");
  attach(audio.mute = false, "Audio::Mute");

  attach(input.driver = "", "Input::Driver");
  attach(input.focusPolicy = 1, "Input::FocusPolicy");

  attach(path.last = application->realpath, "Path::Recent");

  attach(nes.controllerPort1Device = 1, "NES::Controller::Port1");
  attach(nes.controllerPort2Device = 0, "NES::Controller::Port2");

  attach(snes.controllerPort1Device = 1, "SNES::Controller::Port1");
  attach(snes.controllerPort2Device = 0, "SNES::Controller::Port2");

  load(string{ application->userpath, "settings.cfg" });
  save(string{ application->userpath, "settings.cfg" });
}

Config::~Config() {
  save(string{ application->userpath, "settings.cfg" });
}
