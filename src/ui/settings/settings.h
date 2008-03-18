class SettingsWindow : public Window {
public:
  Listbox panel_list;

  void setup();
  void show();
  uintptr_t close(Event);
  uintptr_t list_change(Event);
} window_settings;
