uintptr_t AdvancedWindow::list_change(Event) {
int pos = list.get_selection();
  set_val.enable(pos >= 0);
  set_def.enable(pos >= 0);
  if(pos >= 0 && pos < config::config().list_count) {
    desc.set_text(string() << "(default = " << config::config().list[pos]->def << ")\n" << config::config().list[pos]->desc);
  string val;
    config::config().list[pos]->get(val);
    edit_val.set_text(val);
  }
}

uintptr_t AdvancedWindow::setval_tick(Event) {
char t[4096];
  edit_val.get_text(t, sizeof t);
  update(list.get_selection(), t);
}

uintptr_t AdvancedWindow::setdef_tick(Event) {
  update(list.get_selection(), 0);
  return true;
}

void AdvancedWindow::read_config(uint pos, string &data) {
  data = "?\t?\t?";
  if(pos >= config::config().list_count) return;

string name, val;
  name = config::config().list[pos]->name;
  config::config().list[pos]->get(val);
  if(val != config::config().list[pos]->def) { strcat(name, " (*)"); }
  data = string()
    << name << "\t"
    << (config::config().list[pos]->type == Setting::String ? "String" : "Integer") << "\t"
    << val;
}

void AdvancedWindow::update(uint pos, const char *data) {
  if(pos >= config::config().list_count) return;

  config::config().list[pos]->set(data ? data : config::config().list[pos]->def);
string val;
  config::config().list[pos]->get(val);
  edit_val.set_text(val);
  read_config(pos, val);
  list.set_item(pos, val);
  list.autosize_columns();
}

void AdvancedWindow::setup() {
  create(0, 475, 355);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 235, "Name\tType\tValue");
  desc.create(Editbox::Multiline | Editbox::VerticalScrollAlways | Editbox::Readonly, 475, 80,
    "<description>\n"
    "Warning: modifification of certain variables will not take effect until\n"
    "bsnes is restarted, and corresponding UI elements will not be updated\n"
    "to reflect changes here. (*) = modified"
  );
  edit_val.create(0, 265, 30, "<current value>");
  set_val.create (0, 100, 30, "Set");
  set_def.create (0, 100, 30, "Default");

uint y = 0;
  attach(list,       0, y); y += 235 + 5;
  attach(desc,       0, y); y += 80 + 5;
  attach(edit_val,   0, y);
  attach(set_val,  270, y);
  attach(set_def,  375, y); y += 30 + 5;

  for(int i = 0; i < config::config().list_count; i++) {
  string val;
    read_config(i, val);
    list.add_item(val);
  }

  list.autosize_columns();

  set_val.disable();
  set_def.disable();

  list.on_change  = bind(&AdvancedWindow::list_change, this);
  set_val.on_tick = bind(&AdvancedWindow::setval_tick, this);
  set_def.on_tick = bind(&AdvancedWindow::setdef_tick, this);
}
