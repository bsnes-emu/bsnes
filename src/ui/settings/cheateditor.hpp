//main editor window for all codes
class CheatEditorWindow : public Window {
public:
  Listbox  list;
  Checkbox autosort;
  Button   add_code;
  Button   edit_code;
  Button   delete_code;

  void sync_ui();
  void setup();

  string read_code(unsigned index);
  void refresh();

  uintptr_t toggle_code_state(event_t);
  uintptr_t list_change(event_t);
  uintptr_t autosort_tick(event_t);
  uintptr_t add_tick(event_t);
  uintptr_t edit_tick(event_t);
  uintptr_t delete_tick(event_t);
} window_cheat_editor;

//individual cheat code editor
class CheatCodeEditorWindow : public Window {
public:
  Label    ldescription;
  Editbox  description;
  Label    lcodes;
  Editbox  codes;
  Checkbox enabled;
  Label    lvalid;
  Button   ok;
  Button   cancel;
  Button   clear;

  bool active_mode;
  unsigned active_code;

  void show(bool editmode, unsigned codenumber = 0);
  void setup();

  uintptr_t close(event_t);
  uintptr_t validate(event_t);
  uintptr_t ok_tick(event_t);
  uintptr_t cancel_tick(event_t);
  uintptr_t clear_tick(event_t);
} window_cheat_code_editor;
