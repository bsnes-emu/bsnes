#include "input.hpp"
#include "advanced.hpp"

struct SettingsWindow : Window {
  HorizontalLayout layout;
  ListView panelList;

  void setPanel(unsigned);

  SettingsWindow();
  ~SettingsWindow();
};

extern SettingsWindow *settingsWindow;
