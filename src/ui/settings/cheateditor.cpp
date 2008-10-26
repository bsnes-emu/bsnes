void CheatEditorWindow::setup() {
  create(0, 475, 355);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 272,
    string() << translate["Status"] << "\t" << translate["Code"] << "\t" << translate["Description"]);
  autosort.create   (0, 475, 18, translate["Keep cheat code list sorted by description"]);
  add_code.create   (0, 155, 25, translate["Add Code"]);
  toggle_code.create(0, 155, 25, translate["Toggle Status"]);
  delete_code.create(0, 155, 25, translate["Delete Code"]);
  code.create(0, 155, 25, translate["<code>"]);
  desc.create(0, 315, 25, translate["<description>"]);

  unsigned y = 0;
  attach(list,          0, y); y += 272 + 5;
  attach(autosort,      0, y); y +=  18 + 5;
  attach(add_code,      0, y);
  attach(toggle_code, 160, y);
  attach(delete_code, 320, y); y +=  25 + 5;
  attach(code,          0, y);
  attach(desc,        160, y); y +=  25 + 5;

  autosort.check(config::misc.cheat_autosort);

  list.on_activate    = bind(&CheatEditorWindow::toggle_code_state, this);
  list.on_change      = bind(&CheatEditorWindow::list_change, this);
  autosort.on_tick    = bind(&CheatEditorWindow::autosort_tick, this);
  code.on_change      = bind(&CheatEditorWindow::code_input, this);
  add_code.on_tick    = bind(&CheatEditorWindow::add_tick, this);
  toggle_code.on_tick = bind(&CheatEditorWindow::toggle_code_state, this);
  delete_code.on_tick = bind(&CheatEditorWindow::delete_tick, this);

  refresh();
}

void CheatEditorWindow::refresh() {
  list.reset();
  if(config::misc.cheat_autosort == true) cheat.sort();

  for(unsigned i = 0; i < cheat.count(); i++) {
    Cheat::cheat_t cheatcode;
    cheat.get(i, cheatcode);
    list.add_item(string()
      << (cheatcode.enabled ? translate["Enabled"] : translate["Disabled"]) << "\t"
      << cheatcode.code << "\t"
      << cheatcode.desc);
  }

  list.autosize_columns();

  //enable controls only if cartridge is loaded
  bool loaded = cartridge.loaded();
  if(loaded == false) {
    code.set_text(translate["<code>"]);
    desc.set_text(translate["<description>"]);
  }
  code.enable(loaded);
  desc.enable(loaded);

  add_code.enable(loaded && is_code_valid());
  toggle_code.disable();  //no list item will be selected;
  delete_code.disable();  //so there's nothing to toggle / delete.
}

bool CheatEditorWindow::is_code_valid() {
  //input
  char s_code[16];
  code.get_text(s_code, sizeof s_code);
  //output
  unsigned addr;
  uint8_t data;
  Cheat::type_t type;
  return cheat.decode(s_code, addr, data, type);
}

//enable "Add Code" button only when cheat code is valid
uintptr_t CheatEditorWindow::code_input(event_t) {
  add_code.enable(is_code_valid());
  return true;
}

uintptr_t CheatEditorWindow::autosort_tick(event_t) {
  config::misc.cheat_autosort = autosort.checked();
  if(config::misc.cheat_autosort == true) refresh();
  return true;
}

uintptr_t CheatEditorWindow::toggle_code_state(event_t) {
  int index = list.get_selection();
  if(index >= 0 && index < cheat.count()) {
    cheat.enabled(index) ? cheat.disable(index) : cheat.enable(index);
    Cheat::cheat_t cheatcode;
    cheat.get(index, cheatcode);
    list.set_item(index, string()
      << (cheatcode.enabled ? translate["Enabled"] : translate["Disabled"]) << "\t"
      << cheatcode.code << "\t"
      << cheatcode.desc);
  }
  return true;
}

//enables "Toggle Code" / "Delete Code" buttons when a code is selected
uintptr_t CheatEditorWindow::list_change(event_t) {
  int index = list.get_selection();
  toggle_code.enable(index >= 0);
  delete_code.enable(index >= 0);
  return true;
}

uintptr_t CheatEditorWindow::add_tick(event_t) {
  char s_code[1024], s_desc[1024];
  code.get_text(s_code, sizeof s_code);
  desc.get_text(s_desc, sizeof s_desc);
  cheat.add(false, s_code, s_desc);  //param 0 = false, meaning new codes are disabled by default
  refresh();
  return true;
}

uintptr_t CheatEditorWindow::delete_tick(event_t) {
  int index = list.get_selection();
  if(index >= 0 && index < cheat.count()) {
    cheat.remove(index);
    refresh();
  }
  return true;
}
