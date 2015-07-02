#include "../tomoko.hpp"
ConfigurationManager* config = nullptr;

ConfigurationManager::ConfigurationManager() {
  config = this;

  userInterface.append(userInterface.showStatusBar, "ShowStatusBar");
  append(userInterface, "UserInterface");

  library.append(library.location, "Location");
  append(library, "Library");

  video.append(video.driver, "Driver");
  video.append(video.synchronize, "Synchronize");
  video.append(video.scale, "Scale");
  video.append(video.aspectCorrection, "AspectCorrection");
  video.append(video.filter, "Filter");
  video.append(video.colorEmulation, "ColorEmulation");
  video.append(video.saturation, "Saturation");
  video.append(video.gamma, "Gamma");
  video.append(video.luminance, "Luminance");
  video.overscan.append(video.overscan.mask, "Mask");
  video.overscan.append(video.overscan.horizontal, "Horizontal");
  video.overscan.append(video.overscan.vertical, "Vertical");
  video.append(video.overscan, "Overscan");
  append(video, "Video");

  audio.append(audio.driver, "Driver");
  audio.append(audio.device, "Device");
  audio.append(audio.synchronize, "Synchronize");
  audio.append(audio.mute, "Mute");
  audio.append(audio.volume, "Volume");
  audio.append(audio.frequency, "Frequency");
  audio.append(audio.latency, "Latency");
  audio.append(audio.resampler, "Resampler");
  append(audio, "Audio");

  input.append(input.driver, "Driver");
  append(input, "Input");

  timing.append(timing.video, "Video");
  timing.append(timing.audio, "Audio");
  append(timing, "Timing");

  load(locate({configpath(), "tomoko/"}, "settings.bml"));
  if(!library.location) library.location = {userpath(), "Emulation/"};
  if(!video.driver) video.driver = ruby::Video::safestDriver();
  if(!audio.driver) audio.driver = ruby::Audio::safestDriver();
  if(!input.driver) input.driver = ruby::Input::safestDriver();
  save(locate({configpath(), "tomoko/"}, "settings.bml"));
}

auto ConfigurationManager::quit() -> void {
  save(locate({configpath(), "tomoko/"}, "settings.bml"));
}
