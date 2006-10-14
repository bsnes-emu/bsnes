class Settings {
public:
Window *active_panel;
  void set_active_panel(Window *panel);
  void show_active_panel();
  void hide_active_panel();

  Settings();
} settings;

#include "ui_settings.h"
#include "ui_videosettings.h"
#include "ui_coloradjust.h"
#include "ui_rastersettings.h"
#include "ui_emusettings.h"
#include "ui_inputconfig.h"
#include "ui_cheateditor.h"
