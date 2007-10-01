class AdvancedWindow : public ui::Window { public:
ui::Listbox list;
ui::Editbox desc;
ui::Editbox edit_val;
ui::Button  set_val;
ui::Button  set_def;
  bool message(uint id, uintptr_t param);
  void read_config(uint pos, string &data);
  void update(uint pos, const char *data);
  void setup();
} window_advanced;
