uintptr_t AdvancedWindow::list_change(Event) {
  int pos = list.get_selection();
  edit_val.enable(pos >= 0);
  set_val.enable(pos >= 0);
  set_def.enable(pos >= 0);
  if(pos >= 0) {
    unsigned i = lookup[pos];
    string default_;
    config::config().list[i]->get_default(default_);
    desc.set_text(string() << "(default = " << default_ << ")\n" << config::config().list[i]->description);
    string value_;
    config::config().list[i]->get(value_);
    edit_val.set_text(value_);
  }
  return true;
}

uintptr_t AdvancedWindow::setval_tick(Event) {
  char t[4096];
  edit_val.get_text(t, sizeof t);
  update(list.get_selection(), t);
  return true;
}

uintptr_t AdvancedWindow::setdef_tick(Event) {
  update(list.get_selection(), 0);
  return true;
}

void AdvancedWindow::update(uint pos, const char *data) {
  unsigned i = lookup[pos];
  string default_;
  config::config().list[i]->get_default(default_);
  config::config().list[i]->set(data ? data : (const char*)default_);
  string value_;
  config::config().list[i]->get(value_);
  edit_val.set_text(value_);
  list.set_item(pos, string()
    << config::config().list[i]->name
    << (value_ == default_ ? "" : " (*)")
    << "\t"
    << (config::config().list[i]->type == setting::string_type ? "string" : "integral")
    << "\t"
    << value_
  );
  list.autosize_columns();
}

void AdvancedWindow::load() {
  lookup.reset();

  for(unsigned i = 0; i < config::config().list.size(); i++) {
    string name = config::config().list[i]->name;

    //blacklist (omit/hide options that can be configured through the standard UI)
    if(name == "file.autodetect_type") continue;
    if(strbegin(name, "path.")) continue;
    if(strbegin(name, "snes.controller_port_")) continue;
    if(strpos(name, "colorfilter.") >= 0) continue;
    if(name == "misc.status_enable") continue;
    if(name == "system.regulate_speed") continue;
    if(strbegin(name, "video.windowed.") && name != "video.windowed.synchronize") continue;
    if(strbegin(name, "video.fullscreen.") && name != "video.fullscreen.synchronize") continue;
    if(name == "audio.mute") continue;
    if(name == "input.capture_mode") continue;
    if(strbegin(name, "input.joypad")) continue;
    if(strbegin(name, "input.gui")) continue;

    string value_, default_;
    config::config().list[i]->get(value_);
    config::config().list[i]->get_default(default_);
    list.add_item(string()
      << name
      << (value_ == default_ ? "" : " (*)")
      << "\t"
      << (config::config().list[i]->type == setting::string_type ? "string" : "integral")
      << "\t"
      << value_
    );
    lookup.add(i);
  }
}

void AdvancedWindow::setup() {
  create(0, 475, 355);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 235, "Name\tType\tValue");
  desc.create(Editbox::Multiline | Editbox::VerticalScrollAlways | Editbox::Readonly, 475, 80,
    "Note: modification of certain variables will not take effect until\n"
    "bsnes is restarted. (*) = modified"
  );
  edit_val.create(0, 265, 30, "<current value>");
  set_val.create (0, 100, 30, "Set");
  set_def.create (0, 100, 30, "Default");

  unsigned y = 0;
  attach(list,       0, y); y += 235 + 5;
  attach(desc,       0, y); y +=  80 + 5;
  attach(edit_val,   0, y);
  attach(set_val,  270, y);
  attach(set_def,  375, y); y +=  30 + 5;

  load();
  list.autosize_columns();

  edit_val.disable();
  set_val.disable();
  set_def.disable();

  list.on_change  = bind(&AdvancedWindow::list_change, this);
  set_val.on_tick = bind(&AdvancedWindow::setval_tick, this);
  set_def.on_tick = bind(&AdvancedWindow::setdef_tick, this);
}
