#include "../base.hpp"
SettingsWindow *settingsWindow = 0;

SettingsWindow::SettingsWindow() {
  setTitle("Configuration Settings");
  setGeometry({ 128, 128, 640, 360 });
}
