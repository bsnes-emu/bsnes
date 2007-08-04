void CheatEditorWindow::refresh() {
  list.reset();

  for(uint i = 0; i < cheat.count(); i++) {
  bool   enabled;
  uint32 addr;
  uint8  data;
  char   s_code[256], s_desc[256], s_result[1024];
    cheat.get(i, enabled, addr, data, s_code, s_desc);
    sprintf(s_result, "%s|%s|%s", enabled ? "Enabled" : "Disabled", s_code, s_desc);
    list.add_item(s_result);
  }

  list.autosize_columns();

//enable controls only if cartridge is loaded
bool loaded = cartridge.loaded();
  add_code.enable(loaded);
  toggle_code.enable(loaded);
  delete_code.enable(loaded);
}

bool CheatEditorWindow::message(uint id, uintptr_t param) {
ui::Control *control = (ui::Control*)param;

  if(id == ui::Message::Clicked && control == &add_code) {
  char s_code[256], s_desc[256];
    code.get_text(s_code, sizeof s_code);
    desc.get_text(s_desc, sizeof s_desc);
    cheat.add(false, s_code, s_desc); //param 0 = new codes disabled by default
    refresh();
    return true;
  }

  if((id == ui::Message::Clicked && control == &toggle_code) ||
     (id == ui::Message::DoubleClicked && control == &list)) {
  int index = list.get_selection();
    if(index >= 0 && index < cheat.count()) {
      cheat.enabled(index) ? cheat.disable(index) : cheat.enable(index);
    bool   enabled;
    uint32 addr;
    uint8  data;
    char   s_code[256], s_desc[256], s_result[1024];
      cheat.get(index, enabled, addr, data, s_code, s_desc);
      sprintf(s_result, "%s|%s|%s", enabled ? "Enabled" : "Disabled", s_code, s_desc);
      list.set_item(index, s_result);
    }
    return true;
  }

  if(id == ui::Message::Clicked && control == &delete_code) {
  int index = list.get_selection();
    if(index >= 0 && index < cheat.count()) {
      cheat.remove(index);
      refresh();
    }
    return true;
  }

  return true;
}

void CheatEditorWindow::setup() {
  create(0, 475, 355);

int x = 0, y = 0;
  list.create(*this, ui::Listbox::Header | ui::Listbox::VerticalScrollAlways, x, y, 475, 285, "Status|Code|Description");
  y += 290;

  add_code.create   (*this, 0, x,       y, 155, 30, "Add Code");
  toggle_code.create(*this, 0, x + 160, y, 155, 30, "Toggle Status");
  delete_code.create(*this, 0, x + 320, y, 155, 30, "Delete Code");
  y += 35;

  code.create(*this, 0, x,       y, 155, 30, "<code>");
  desc.create(*this, 0, x + 160, y, 315, 30, "<description>");

  refresh();
}
