class SettingsWindow : public ui::Window { public:
ui::Listbox panel_list;
ui::Panel panel;
  int  message(uint id, void *param);
  void show();
  void setup();
} window_settings;
