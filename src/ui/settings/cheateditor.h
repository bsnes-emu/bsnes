class CheatEditorWindow : public Window {
public:
  Listbox  list;
  Checkbox autosort;
  Button   add_code;
  Button   toggle_code;
  Button   delete_code;
  Editbox  code;
  Editbox  desc;

  void setup();
  void refresh();
  bool is_code_valid();

  uintptr_t toggle_code_state(event_t);
  uintptr_t list_change(event_t);
  uintptr_t code_input(event_t);
  uintptr_t autosort_tick(event_t);
  uintptr_t add_tick(event_t);
  uintptr_t delete_tick(event_t);
} window_cheat_editor;
