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
  uintptr_t list_change(Event);
  uintptr_t setval_tick(Event);
  uintptr_t setdef_tick(Event);
} window_advanced;
