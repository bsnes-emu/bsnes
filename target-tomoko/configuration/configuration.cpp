#include "../tomoko.hpp"
ConfigurationManager* configurationManager = nullptr;
auto config() -> ConfigurationManager& { return *configurationManager; }

ConfigurationManager::ConfigurationManager() {
  configurationManager = this;

  video.append(video.driver, "Driver");
  video.append(video.synchronize, "Synchronize");
  video.append(video.scale, "Scale");
  video.append(video.aspectCorrection, "AspectCorrection");
  video.append(video.filter, "Filter");
  video.append(video.colorEmulation, "ColorEmulation");
  append(video, "Video");

  audio.append(audio.driver, "Driver");
  audio.append(audio.synchronize, "Synchronize");
  audio.append(audio.mute, "Mute");
  append(audio, "Audio");

  input.append(input.driver, "Driver");
  append(input, "Input");

  load({configpath(), "tomoko/settings.bml"});
  if(!video.driver) video.driver = ruby::video.safestDriver();
  if(!audio.driver) audio.driver = ruby::audio.safestDriver();
  if(!input.driver) input.driver = ruby::input.safestDriver();
  save({configpath(), "tomoko/settings.bml"});
}

auto ConfigurationManager::quit() -> void {
  save({configpath(), "tomoko/settings.bml"});
}
