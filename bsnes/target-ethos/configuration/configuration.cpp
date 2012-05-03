#include "../ethos.hpp"
Configuration *config = nullptr;

Configuration::Configuration() {
  append(video.scaleMode = 0, "Video::ScaleMode");
  append(video.aspectCorrection = true, "Video::AspectCorrection");

  load();
}

void Configuration::load() {
  configuration::load(application->path("settings.cfg"));
  save();  //creates file if it does not exist
}

void Configuration::save() {
  configuration::save(application->path("settings.cfg"));
}
