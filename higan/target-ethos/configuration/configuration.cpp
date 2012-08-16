#include "../ethos.hpp"
Configuration *config = nullptr;

Configuration::Configuration() {
  append(video.driver = ruby::video.default_driver(), "Video::Driver");
  append(video.synchronize = false, "Video::Synchronize");
  append(video.shader = "Blur", "Video::Shader");
  append(video.scaleMode = 0, "Video::ScaleMode");
  append(video.aspectCorrection = true, "Video::AspectCorrection");
  append(video.maskOverscan = false, "Video::MaskOverscan");
  append(video.maskOverscanHorizontal = 8, "Video::MaskOverscan::Horizontal");
  append(video.maskOverscanVertical = 8, "Video::MaskOverscan::Vertical");
  append(video.saturation = 100, "Video::Saturation");
  append(video.gamma = 150, "Video::Gamma");
  append(video.luminance = 100, "Video::Luminance");
  append(audio.driver = ruby::audio.default_driver(), "Audio::Driver");
  append(audio.synchronize = true, "Audio::Synchronize");
  append(audio.frequency = 48000, "Audio::Frequency");
  append(audio.latency = 60, "Audio::Latency");
  append(audio.resampler = 2, "Audio::Resampler");
  append(audio.volume = 100, "Audio::Volume");
  append(audio.mute = false, "Audio::Mute");
  append(input.driver = ruby::input.default_driver(), "Input::Driver");
  append(input.focusPause = false, "Input::Focus::Pause");
  append(input.focusAllow = false, "Input::Focus::AllowInput");
  append(timing.video = 60.0, "Timing::Video");
  append(timing.audio = 48000.0, "Timing::Audio");
  load();
}

void Configuration::load() {
  configuration::load(application->path("settings.cfg"));
  save();  //creates file if it does not exist
}

void Configuration::save() {
  configuration::save(application->path("settings.cfg"));
}
