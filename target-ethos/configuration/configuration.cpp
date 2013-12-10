#include "../ethos.hpp"
ConfigurationSettings* config = nullptr;

ConfigurationSettings::ConfigurationSettings() {
  video.append(video.driver = ruby::video.safestDriver(), "Driver");
  video.append(video.synchronize = false, "Synchronize");
  video.append(video.shader = "Blur", "Shader");
  video.append(video.scaleMode = 0, "ScaleMode");
  video.append(video.aspectCorrection = true, "AspectCorrection");
  video.append(video.colorEmulation = true, "ColorEmulation");
  video.maskOverscan.assign(video.maskOverscan.enable = false);
  video.maskOverscan.append(video.maskOverscan.horizontal = 8, "Horizontal");
  video.maskOverscan.append(video.maskOverscan.vertical = 8, "Vertical");
  video.append(video.maskOverscan, "MaskOverscan");
  video.append(video.saturation = 100, "Saturation");
  video.append(video.gamma = 100, "Gamma");
  video.append(video.luminance = 100, "Luminance");
  video.append(video.startFullScreen = false, "StartFullScreen");
  append(video, "Video");

  audio.append(audio.driver = ruby::audio.safestDriver(), "Driver");
  audio.append(audio.synchronize = true, "Synchronize");
  audio.append(audio.frequency = 48000, "Frequency");
  audio.append(audio.latency = 60, "Latency");
  audio.append(audio.resampler = 2, "Resampler");
  audio.append(audio.volume = 100, "Volume");
  audio.append(audio.mute = false, "Mute");
  append(audio, "Audio");

  input.append(input.driver = ruby::input.safestDriver(), "Driver");
  input.focus.append(input.focus.pause = false, "Pause");
  input.focus.append(input.focus.allow = false, "AllowInput");
  input.append(input.focus, "Focus");
  append(input, "Input");

  timing.append(timing.video = 60.0, "Video");
  timing.append(timing.audio = 48000.0, "Audio");
  append(timing, "Timing");

  server.append(server.hostname = "", "Hostname");
  server.append(server.username = "", "Username");
  server.append(server.password = "", "Password");
  append(server, "Server");

  library.append(library.selection = -1, "Selection");
  library.append(library.mediaMode = 0, "MediaMode");
  library.append(library.showOnStartup = true, "ShowOnStartup");
  append(library, "Library");

  load();
}

void ConfigurationSettings::load() {
  Configuration::Document::load(program->path("settings.bml"));
  save();  //creates file if it does not exist
}

void ConfigurationSettings::save() {
  Configuration::Document::save(program->path("settings.bml"));
}
