#include "../base.hpp"
Config *config = 0;

Config::Config() {
  attach(video.driver = "", "Video::Driver");
  attach(video.filter = "None", "Video::Filter");
  attach(video.shader = "None", "Video::Shader");
  attach(video.synchronize = true, "Video::Synchronize");
  attach(video.enableOverscan = false, "Video::EnableOverscan");
  attach(video.correctAspectRatio = true, "Video::CorrectAspectRatio");

  attach(video.brightness = 100, "Video::Brightness");
  attach(video.contrast = 100, "Video::Contrast");
  attach(video.gamma = 100, "Video::Gamma");
  attach(video.gammaRamp = true, "Video::GammaRamp");
  attach(video.fullScreenMode = 0, "Video::FullScreenMode");

  attach(audio.driver = "", "Audio::Driver");
  attach(audio.synchronize = true, "Audio::Synchronize");
  attach(audio.mute = false, "Audio::Mute");
  attach(audio.volume = 100, "Audio::Volume");

  attach(audio.frequencyNES = 1789772, "Audio::Frequency::NES");
  attach(audio.frequencySNES = 32000, "Audio::Frequency::SNES");
  attach(audio.frequencyGameBoy = 4194304, "Audio::Frequency::GameBoy");

  attach(input.driver = "", "Input::Driver");
  attach(input.focusPolicy = 1, "Input::FocusPolicy");

  attach(path.bios.satellaview = "", "Path::BIOS::Satellaview");
  attach(path.bios.sufamiTurbo = "", "Path::BIOS::SufamiTurbo");
  attach(path.bios.superGameBoy = "", "Path::BIOS::SuperGameBoy");

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
