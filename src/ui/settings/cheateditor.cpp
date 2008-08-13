void CheatEditorWindow::setup() {
  create(0, 475, 355);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 295,
    string() << translate["Status"] << "\t" << translate["Code"] << "\t" << translate["Description"]);
  add_code.create   (0, 155, 25, translate["Add Code"]);
  toggle_code.create(0, 155, 25, translate["Toggle Status"]);
  delete_code.create(0, 155, 25, translate["Delete Code"]);
  code.create(0, 155, 25, translate["<code>"]);
  desc.create(0, 315, 25, translate["<description>"]);

  unsigned y = 0;
  attach(list,          0, y); y += 295 + 5;
  attach(add_code,      0, y);
  attach(toggle_code, 160, y);
  attach(delete_code, 320, y); y += 25 + 5;
  attach(code,          0, y);
  attach(desc,        160, y); y += 25 + 5;

  list.on_activate = bind(&CheatEditorWindow::toggle_event, this);
  add_code.on_tick = bind(&CheatEditorWindow::add_tick, this);
  toggle_code.on_tick = bind(&CheatEditorWindow::toggle_event, this);
  delete_code.on_tick = bind(&CheatEditorWindow::delete_tick, this);

  refresh();
}

void CheatEditorWindow::refresh() {
  list.reset();

  for(unsigned i = 0; i < cheat.count(); i++) {
    bool enabled;
    uint32 addr;
    uint8 data;
    char s_code[256], s_desc[256];
    cheat.get(i, enabled, addr, data, s_code, s_desc);
    list.add_item(string()
      << (enabled ? translate["Enabled"] : translate["Disabled"]) << "\t"
      << s_code << "\t"
      << s_desc);
  }

  list.autosize_columns();

  //enable controls only if cartridge is loaded
  bool loaded = cartridge.loaded();
  code.enable(loaded);
  desc.enable(loaded);
  add_code.enable(loaded);
  toggle_code.enable(loaded);
  delete_code.enable(loaded);
}

uintptr_t CheatEditorWindow::toggle_event(event_t) {
  int index = list.get_selection();
  if(index >= 0 && index < cheat.count()) {
    cheat.enabled(index) ? cheat.disable(index) : cheat.enable(index);
    bool enabled;
    uint32 addr;
    uint8 data;
    char s_code[256], s_desc[256];
    cheat.get(index, enabled, addr, data, s_code, s_desc);
    list.set_item(index, string()
      << (enabled ? translate["Enabled"] : translate["Disabled"]) << "\t"
      << s_code << "\t"
      << s_desc);
  }
  return true;
}

uintptr_t CheatEditorWindow::add_tick(event_t) {
  char s_code[256], s_desc[256];
  code.get_text(s_code, sizeof s_code);
  desc.get_text(s_desc, sizeof s_desc);
  cheat.add(false, s_code, s_desc); //param 0 = false, meaning: new codes disabled by default
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
