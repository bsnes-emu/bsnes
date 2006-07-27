#include "ui_settings.cpp"
#include "ui_videosettings.cpp"
#include "ui_coloradjust.cpp"
#include "ui_rastersettings.cpp"
#include "ui_inputconfig.cpp"
#include "ui_cheateditor.cpp"

void init_settings() {
  wSettings.SetIcon(100);
  wSettings.Create(0, "bsnes_settings", "topmost|title|minimize",
    0, 0, 640, 365, "bsnes Configuration Settings");
  wSettings.SetAlphaLevel(config::misc.config_window_alpha_level);
  wSettings.MoveToBottom();
  wSettings.MoveToRight();

  wVideoSettings.Create(&wSettings, "bsnes_videosettings", "", 160, 5, 475, 355);
  wColorAdjust.Create(&wSettings, "bsnes_coloradjust", "", 160, 5, 475, 355);
  wRasterSettings.Create(&wSettings, "bsnes_rastersettings", "", 160, 5, 475, 355);
  wInputConfig.Create(&wSettings, "bsnes_inputconfig", "", 160, 5, 475, 355);
  wCheatEditor.Create(&wSettings, "bsnes_cheateditor", "", 160, 5, 475, 355);
}

void setup_settings() {
  wSettings.Setup();

  wVideoSettings.Setup();
  wColorAdjust.Setup();
  wRasterSettings.Setup();
  wInputConfig.Setup();
  wCheatEditor.Setup();
}

void Settings::set_active_panel(Window *panel) {
  if(active_panel) {
    active_panel->Hide();
    active_panel = 0;
  }
  if(panel) {
    active_panel = panel;
    active_panel->Show();
  }
}

void Settings::show_active_panel() {
  if(active_panel) {
    active_panel->Show();
  }
}

void Settings::hide_active_panel() {
  if(active_panel) {
    active_panel->Hide();
  }
}

Settings::Settings() {
  active_panel = 0;
}
