#include "../loki.hpp"
Settings* settings = nullptr;

Settings::Settings() {
  settings = this;

  video.append(video.driver = ruby::video.optimalDriver(), "Driver");
  video.append(video.synchronize = false, "Synchronize");
  append(video, "Video");

  audio.append(audio.driver = ruby::audio.optimalDriver(), "Driver");
  audio.append(audio.synchronize = true, "Synchronize");
  audio.append(audio.mute = false, "Mute");
  append(audio, "Audio");

  input.append(input.driver = ruby::input.optimalDriver(), "Driver");
  append(input, "Input");

  terminal.append(terminal.backgroundColor = 0x481818, "BackgroundColor");
  terminal.append(terminal.foregroundColor = 0xffffff, "ForegroundColor");
  append(terminal, "Terminal");

  geometry.append(geometry.presentation = "", "Presentation");
  geometry.append(geometry.terminal = "", "Terminal");
  append(geometry, "Geometry");

  load();
}

void Settings::load() {
  Configuration::Document::load(program->path("settings.bml"));
  Configuration::Document::save(program->path("settings.bml"));
}

void Settings::unload() {
  //remember window geometry for next run
  geometry.presentation = presentation->geometry().text();
  geometry.terminal = ::terminal->geometry().text();

  Configuration::Document::save(program->path("settings.bml"));
}

void Settings::command(string s, lstring args) {
  unsigned argc = args.size();
  s.ltrim<1>("settings.");

  if(s == "video.driver" && argc == 1) { video.driver = args[0]; return; }
  if(s == "video.synchronize" && argc == 1) { video.synchronize = args[0] != "false"; ruby::video.set(ruby::Video::Synchronize, video.synchronize); return; }
  if(s == "audio.driver" && argc == 1) { audio.driver = args[0]; return; }
  if(s == "audio.synchronize" && argc == 1) { audio.synchronize = args[0] != "false"; ruby::audio.set(ruby::Audio::Synchronize, audio.synchronize); return; }
  if(s == "audio.mute" && argc == 1) { audio.mute = args[0] != "false"; return; }
  if(s == "input.driver" && argc == 1) { input.driver = args[0]; return; }
  if(s == "terminal.background-color" && argc == 1) { terminal.backgroundColor = hex(args[0]); ::terminal->setColors(); return; }
  if(s == "terminal.foreground-color" && argc == 1) { terminal.foregroundColor = hex(args[0]); ::terminal->setColors(); return; }

  echo("Error: unrecognized setting: ", s, "\n");
}
