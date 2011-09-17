#include "input.hpp"

struct SettingsWindow : Window {
  HorizontalLayout layout;
  ListView panelList;

  SettingsWindow();
  ~SettingsWindow();
};

extern SettingsWindow *settingsWindow;
