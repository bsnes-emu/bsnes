class AdvancedWindow : public Window {
public:
  Listbox list;
  Editbox edit;
  Button  set;

  array<unsigned> lookup;
  void update(unsigned item);
  void load();
  void setup();

  uintptr_t list_activate(event_t);
  uintptr_t list_change(event_t);
  uintptr_t set_tick(event_t);
} window_advanced;
