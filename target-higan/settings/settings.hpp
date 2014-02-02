struct SettingsLayout : VerticalLayout {
  SettingsLayout();
};

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "hotkey.hpp"
#include "timing.hpp"
#include "server.hpp"
#include "advanced.hpp"

struct Settings : Window {
  VerticalLayout layout;
  TabFrame panels;

  Settings();
};

extern Settings* settings;
