//=================
//CheatEditorWindow
//=================

void CheatEditorWindow::setup() {
  create(0, 451, 370);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 451, 317,
    string() << translate["Status"] << "\t" << translate["Code"] << "\t" << translate["Description"]);
  autosort.create   (0, 451, 18, translate["Keep cheat code list sorted by description"]);
  add_code.create   (0, 147, 25, translate["Add Code"]);
  edit_code.create  (0, 147, 25, translate["Edit Code"]);
  delete_code.create(0, 147, 25, translate["Delete Code"]);

  unsigned y = 0;
  attach(list,          0, y); y += 317 + 5;
  attach(autosort,      0, y); y +=  18 + 5;
  attach(add_code,      0, y);
  attach(edit_code,   152, y);
  attach(delete_code, 304, y); y +=  25 + 5;

  autosort.check(config.misc.cheat_autosort);

  list.on_activate    = bind(&CheatEditorWindow::toggle_code_state, this);
  list.on_change      = bind(&CheatEditorWindow::list_change, this);
  autosort.on_tick    = bind(&CheatEditorWindow::autosort_tick, this);
  add_code.on_tick    = bind(&CheatEditorWindow::add_tick, this);
  edit_code.on_tick   = bind(&CheatEditorWindow::edit_tick, this);
  delete_code.on_tick = bind(&CheatEditorWindow::delete_tick, this);

  sync_ui();
  window_cheat_code_editor.setup();
}

void CheatEditorWindow::sync_ui() {
  add_code.enable(cartridge.loaded());
  edit_code.enable(cartridge.loaded() && list.get_selection() >= 0);
  delete_code.enable(cartridge.loaded() && list.get_selection() >= 0);
}

string CheatEditorWindow::read_code(unsigned index) {
  string s;

  Cheat::cheat_t item;
  cheat.get(index, item);
  s << (item.enabled ? translate["Enabled"] : translate["Disabled"]) << "\t";

  lstring line;
  split(line, "+", item.code);
  if(count(line) > 1) line[0] << "+...";
  s << line[0] << "\t";

  split(line, "\n", item.desc);
  if(count(line) > 1) line[0] << " ...";
  s << line[0];

  return s;
}

void CheatEditorWindow::refresh() {
  if(config.misc.cheat_autosort == true) cheat.sort();

  list.reset();
  for(unsigned i = 0; i < cheat.count(); i++) list.add_item(read_code(i));
  list.autosize_columns();

  sync_ui();
}

uintptr_t CheatEditorWindow::autosort_tick(event_t) {
  config.misc.cheat_autosort = autosort.checked();
  if(config.misc.cheat_autosort == true) refresh();
  return true;
}

uintptr_t CheatEditorWindow::toggle_code_state(event_t) {
  int index = list.get_selection();
  if(index >= 0 && index < cheat.count()) {
    cheat.enabled(index) ? cheat.disable(index) : cheat.enable(index);
    list.set_item(index, read_code(index));
  }
  return true;
}

//enables "Edit Code" / "Delete Code" buttons when a code is selected
uintptr_t CheatEditorWindow::list_change(event_t) {
  int index = list.get_selection();
  edit_code.enable(index >= 0);
  delete_code.enable(index >= 0);
  return true;
}

uintptr_t CheatEditorWindow::add_tick(event_t) {
  add_code.disable();
  edit_code.disable();
  delete_code.disable();
  window_cheat_code_editor.show(false);
  return true;
}

uintptr_t CheatEditorWindow::edit_tick(event_t) {
  add_code.disable();
  edit_code.disable();
  delete_code.disable();
  window_cheat_code_editor.show(true, list.get_selection());
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

//=====================
//CheatCodeEditorWindow
//=====================

uintptr_t CheatCodeEditorWindow::close(event_t) {
  hide();
  window_cheat_editor.sync_ui();
  return false;
}

uintptr_t CheatCodeEditorWindow::validate(event_t) {
  char s_codes[1024];
  codes.get_text(s_codes, sizeof s_codes);

  string s_code = s_codes;
  strtr(s_code, " ,;&|\t\n", "+++++++");
  while(strpos(s_code, "++") >= 0) replace(s_code, "++", "+");
  trim(s_code, "+");

  Cheat::cheat_t item;
  bool valid = cheat.decode(s_code, item);

  lvalid.set_text(valid ? "" : translate["Cheat code is invalid."]);
  ok.enable(valid);
  return true;
}

uintptr_t CheatCodeEditorWindow::ok_tick(event_t) {
  char s_desc[1024], s_codes[1024];
  description.get_text(s_desc, sizeof s_desc);
  codes.get_text(s_codes, sizeof s_codes);

  string s_code = s_codes;
  strtr(s_code, " ,;&|\t\n", "+++++++");
  while(strpos(s_code, "++") >= 0) replace(s_code, "++", "+");
  trim(s_code, "+");

  if(active_mode == false) {
    //add a new code
    cheat.add(enabled.checked(), s_code, s_desc);
  } else {
    //modify an existing code
    cheat.edit(active_code, enabled.checked(), s_code, s_desc);
  }

  hide();
  window_cheat_editor.refresh();
  return true;
}

uintptr_t CheatCodeEditorWindow::cancel_tick(event_t) {
  hide();
  window_cheat_editor.sync_ui();
  return true;
}

uintptr_t CheatCodeEditorWindow::clear_tick(event_t) {
  description.set_text("");
  codes.set_text("");
  enabled.check(false);
  return true;
}

void CheatCodeEditorWindow::show(bool editmode, unsigned codenumber) {
  active_mode = editmode;
  active_code = codenumber;

  if(active_mode == false) {
    set_text(translate["Add new cheat code"]);
    description.set_text("");
    codes.set_text("");
    enabled.check(false);
  } else {
    Cheat::cheat_t item;
    cheat.get(active_code, item);

    set_text(translate["Modify existing cheat code"]);
    description.set_text(item.desc);
    string s = item.code;
    replace(s, "+", " + ");
    codes.set_text(s);
    enabled.check(item.enabled);
  }

  enabled.focus();
  validate(event_t(event_t::Change));
  Window::show();
}

void CheatCodeEditorWindow::setup() {
  create(Window::AutoCenter, 395, 235);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  ldescription.create(0, 385, 18, translate["Description:"]);
  description.create(Editbox::Multiline | Editbox::HorizontalScrollNever, 385, 65);
  lcodes.create(0, 385, 18, translate["Enter one or more cheat codes below:"]);
  codes.create(Editbox::Multiline | Editbox::HorizontalScrollNever, 385, 65);
  enabled.create(0, 190, 18, translate["Enable this cheat code"]);
  lvalid.create(0, 190, 18);
  ok.create(0, 125, 25, translate["Ok"]);
  cancel.create(0, 125, 25, translate["Cancel"]);
  clear.create(0, 125, 25, translate["Clear"]);

  unsigned y = 5;
  attach(ldescription, 5, y); y += 18;
  attach(description,  5, y); y += 65 + 5;
  attach(lcodes,       5, y); y += 18;
  attach(codes,        5, y); y += 65 + 5;
  attach(enabled,      5, y);
  attach(lvalid,     200, y); y += 18 + 5;
  attach(ok,           5, y);
  attach(cancel,     135, y);
  attach(clear,      265, y); y += 25 + 5;
  lvalid.disable();

  on_close = bind(&CheatCodeEditorWindow::close, this);
  codes.on_change = bind(&CheatCodeEditorWindow::validate, this);
  ok.on_tick = bind(&CheatCodeEditorWindow::ok_tick, this);
  cancel.on_tick = bind(&CheatCodeEditorWindow::cancel_tick, this);
  clear.on_tick = bind(&CheatCodeEditorWindow::clear_tick, this);
}
