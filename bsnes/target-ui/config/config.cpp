#include "../base.hpp"
Config *config = nullptr;

Config::Config() {
  append(video.driver = "", "Video::Driver");
  append(video.filter = "None", "Video::Filter");
  append(video.shader = "Blur", "Video::Shader");
  append(video.synchronize = true, "Video::Synchronize");
  append(video.correctAspectRatio = true, "Video::CorrectAspectRatio");

  append(video.maskOverscan = false, "Video::MaskOverscan");
  append(video.maskOverscanHorizontal = 8, "Video::MaskOverscanHorizontal");
  append(video.maskOverscanVertical = 8, "Video::MaskOverscanVertical");

  append(video.brightness = 100, "Video::Brightness");
  append(video.contrast = 100, "Video::Contrast");
  append(video.gamma = 50, "Video::Gamma");

  append(video.fullScreenMode = 0, "Video::FullScreenMode");

  append(video.startFullScreen = false, "Video::StartFullScreen");
  append(video.compositionMode = 0, "Video::CompositionMode");

  append(audio.driver = "", "Audio::Driver");
  append(audio.synchronize = true, "Audio::Synchronize");
  append(audio.mute = false, "Audio::Mute");
  append(audio.volume = 100, "Audio::Volume");
  append(audio.latency = 60, "Audio::Latency");
  append(audio.resampler = "sinc", "Audio::Resampler");

  append(audio.frequency = 48000, "Audio::Frequency::Native");
  append(audio.frequencyNES = 1789772, "Audio::Frequency::NES");
  append(audio.frequencySNES = 32000, "Audio::Frequency::SNES");
  append(audio.frequencyGB = 4194304, "Audio::Frequency::GB");
  append(audio.frequencyGBA = 32768, "Audio::Frequency::GBA");

  append(input.driver = "", "Input::Driver");
  append(input.focusPolicy = 1, "Input::FocusPolicy");

  append(nes.controllerPort1Device = 1, "Famicom::Controller::Port1");
  append(nes.controllerPort2Device = 0, "Famicom::Controller::Port2");

  append(snes.controllerPort1Device = 1, "SuperFamciom::Controller::Port1");
  append(snes.controllerPort2Device = 0, "SuperFamicom::Controller::Port2");

  load(application->path("settings.cfg"));
  save(application->path("settings.cfg"));
}

Config::~Config() {
  save(application->path("settings.cfg"));
}
