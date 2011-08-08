struct SettingsWindow : TopLevelWindow {
  enum : unsigned { PanelWidth = 120 };

  HorizontalLayout layout;
  ListView panel;

  void create();
  void change();
};

extern SettingsWindow settingsWindow;

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "advanced.hpp"
