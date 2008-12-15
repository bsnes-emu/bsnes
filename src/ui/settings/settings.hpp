class SettingsWindow : public Window {
public:
  Listbox panel_list;

  void setup();
  void show();
  uintptr_t close(event_t);
  uintptr_t list_change(event_t);
} window_settings;
