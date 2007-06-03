class CheatEditorWindow : public ui::Window { public:
ui::Listbox list;
ui::Button  add_code;
ui::Button  toggle_code;
ui::Button  delete_code;
ui::Editbox code;
ui::Editbox desc;
  int  message(uint id, void *param);
  void setup();
} window_cheat_editor;
