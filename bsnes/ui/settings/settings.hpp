struct SettingsLayout : HorizontalLayout {
  Widget spacer;
  VerticalLayout layout;

  void append(Sizable &widget, const Size &size, unsigned spacing = 0);
  SettingsLayout();
};

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "advanced.hpp"

struct SettingsWindow : Window {
  HorizontalLayout layout;
  ListView panelList;

  void panelChanged();

  SettingsWindow();
  ~SettingsWindow();
};

extern SettingsWindow *settingsWindow;
