class CheatEditorWindow : public Window {
public:
  Listbox list;
  Button  add_code;
  Button  toggle_code;
  Button  delete_code;
  Editbox code;
  Editbox desc;

  void setup();
  void refresh();

  uintptr_t toggle_event(event_t);
  uintptr_t add_tick(event_t);
  uintptr_t delete_tick(event_t);
} window_cheat_editor;
