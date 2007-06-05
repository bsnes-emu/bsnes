bool AdvancedWindow::message(uint id, uintptr_t param) {
ui::Control *control = (ui::Control*)param;
  if(id == ui::Message::Changed && control == &list) {
  int pos = list.get_selection();
    set_val.enable(pos >= 0);
    set_def.enable(pos >= 0);
    if(pos >= 0 && pos < config_file.list_count) {
      desc.set_text("(default = %s)\n%s", config_file.list[pos]->def, config_file.list[pos]->desc);
    string val;
      config_file.list[pos]->get(val);
      edit_val.set_text("%s", strptr(val));
    }
  } else if(id == ui::Message::Clicked && control == &set_val) {
  char t[4096];
    edit_val.get_text(t, sizeof(t));
    update(list.get_selection(), t);
  } else if(id == ui::Message::Clicked && control == &set_def) {
    update(list.get_selection(), 0);
  }

  return true;
}

void AdvancedWindow::read_config(uint pos, string &data) {
  strcpy(data, "?|?|?");
  if(pos >= config_file.list_count)return;
string name, val;
  name = config_file.list[pos]->name;
  config_file.list[pos]->get(val);
  if(val != config_file.list[pos]->def) { strcat(name, " (*)"); }
  sprintf(data, "%s|%s|%s",
    strptr(name),
    config_file.list[pos]->type == Setting::String ? "String" : "Integer",
    strptr(val)
  );
}

void AdvancedWindow::update(uint pos, const char *data) {
  if(pos >= config_file.list_count)return;
  config_file.list[pos]->set(data ? data : config_file.list[pos]->def);
string val;
  config_file.list[pos]->get(val);
  edit_val.set_text("%s", strptr(val));
  read_config(pos, val);
  list.set_item(pos, strptr(val));
  list.autosize_columns();
}

void AdvancedWindow::setup() {
  create(0, 475, 355);

int x = 0, y = 0;
  list.create(*this, ui::Listbox::Header | ui::Listbox::VerticalScrollAlways, x, y, 475, 235, "Name|Type|Value");
  y += 240;
  desc.create(*this, ui::Editbox::Multiline | ui::Editbox::Readonly, x, y, 475, 80,
    "<description>\n"
    "Warning: modifification of certain variables will not take effect until\n"
    "bsnes is restarted, and corresponding UI elements will not be updated\n"
    "to reflect changes here. (*) = modified"
  );
  y += 85;
  edit_val.create(*this, 0, x, y, 265, 30, "<current value>");
  set_val.create(*this, 0, x + 270, y, 100, 30, "Set");
  set_def.create(*this, 0, x + 375, y, 100, 30, "Default");

  for(int i = 0; i < config_file.list_count; i++) {
  string val;
    read_config(i, val);
    list.add_item(strptr(val));
  }

  list.autosize_columns();

  set_val.disable();
  set_def.disable();
}
