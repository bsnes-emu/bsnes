class AdvancedWindow : public Window {
public:
  Listbox list;
  Editbox desc;
  Editbox edit_val;
  Button  set_val;
  Button  set_def;

  void read_config(uint pos, string &data);
  void update(uint pos, const char *data);
  void setup();
  uintptr_t list_change(Event);
  uintptr_t setval_tick(Event);
  uintptr_t setdef_tick(Event);
} window_advanced;
