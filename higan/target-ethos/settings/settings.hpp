struct SettingsLayout : HorizontalLayout {
  Widget spacer;
  VerticalLayout layout;

  void append(Sizable &widget, const Size &size, unsigned spacing = 0);
  SettingsLayout();
};

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "hotkey.hpp"
#include "timing.hpp"
#include "driver.hpp"

struct Settings : Window {
  HorizontalLayout layout;
  ListView panelList;

  void panelChanged();
  Settings();
};

extern Settings *settings;
