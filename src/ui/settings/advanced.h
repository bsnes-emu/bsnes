class AdvancedWindow : public Window {
public:
  Listbox list;
  Editbox desc;
  Editbox edit_val;
  Button  set_val;
  Button  set_def;

  array<unsigned> lookup;
  void update(uint pos, const char *data);
  void load();
  void setup();
  uintptr_t list_change(event_t);
  uintptr_t setval_tick(event_t);
  uintptr_t setdef_tick(event_t);
} window_advanced;
