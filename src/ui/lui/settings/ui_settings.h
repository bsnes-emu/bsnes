class SettingsWindow : public ui::Window { public:
ui::Listbox panel_list;
ui::Panel panel;
  bool message(uint id, uintptr_t param = 0);
  void show();
  void setup();
} window_settings;
