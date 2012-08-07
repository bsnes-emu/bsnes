#include "../base.hpp"
Settings *settings = nullptr;

void Settings::load() {
  config.append(folderpath, "folderpath");
  config.append(synchronizeAudio = true, "synchronizeAudio");
  config.append(muteAudio = false, "muteAudio");

  config.load({ application->userpath, "settings.cfg" });
  config.save({ application->userpath, "settings.cfg" });
}

void Settings::save() {
  config.save({ application->userpath, "settings.cfg" });
}
