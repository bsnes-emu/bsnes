uintptr_t AdvancedWindow::list_activate(event_t) {
  int item = list.get_selection();
  if(item >= 0) {
    //if item is integral_setting::boolean, toggle its state
    if(config.list[lookup[item]].type == configuration::boolean_t) {
      *(bool*)config.list[lookup[item]].data = *(bool*)config.list[lookup[item]].data ? false : true;
      update(item);
    }
  }
  return true;
}

uintptr_t AdvancedWindow::list_change(event_t) {
  int item = list.get_selection();
  edit.enable(item >= 0);
  set.enable (item >= 0);
  edit.set_text(item >= 0 ? config.list[lookup[item]].get() : translate["<current value>"]);
  return true;
}

uintptr_t AdvancedWindow::set_tick(event_t) {
  int item = list.get_selection();
  if(item >= 0) {
    char value[4096];
    edit.get_text(value, sizeof value);
    config.list[lookup[item]].set(value);
    update(item);
  }
  return true;
}

void AdvancedWindow::update(unsigned item) {
  edit.set_text(config.list[lookup[item]].get());
  list.set_item(item, string()
    << config.list[lookup[item]].name << "\t"
    << config.list[lookup[item]].get() << "\t"
    << config.list[lookup[item]].desc
  );
  list.autosize_columns();
}

void AdvancedWindow::load() {
  lookup.reset();

  for(unsigned i = 0; i < config.list.size(); i++) {
    string name = config.list[i].name;

    //blacklist (omit/hide options that can be configured through the standard UI)
    if(name == "snes.expansion_port") continue;
    if(name == "snes.region") continue;
    if(strbegin(name, "system.")) continue;
    if(strbegin(name, "path.")) continue;
    if(strbegin(name, "snes.controller_port")) continue;
    if(name == "system.emulation_speed") continue;
    if(strbegin(name, "video.windowed.")) continue;
    if(strbegin(name, "video.fullscreen.")) continue;
    if(name == "video.synchronize") continue;
    if(name == "video.contrast") continue;
    if(name == "video.brightness") continue;
    if(name == "video.gamma") continue;
    if(name == "video.gamma_ramp") continue;
    if(name == "video.ntsc_filter_merge_fields") continue;
    if(strbegin(name, "audio.")) continue;
    if(name == "input.capture_mode") continue;
    if(strbegin(name, "input.joypad")) continue;
    if(strbegin(name, "input.multitap")) continue;
    if(strbegin(name, "input.mouse")) continue;
    if(strbegin(name, "input.superscope")) continue;
    if(strbegin(name, "input.justifier")) continue;
    if(strbegin(name, "input.gui")) continue;
    if(strbegin(name, "input.debugger")) continue;

    list.add_item(string()
      << name << "\t"
      << config.list[i].get() << "\t"
      << config.list[i].desc
    );
    lookup.add(i);
  }
}

void AdvancedWindow::setup() {
  create(0, 451, 370);

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 451, 340,
    string() << translate["Name"] << "\t" << translate["Value"] << "\t" << translate["Description"]);
  edit.create(0, 346, 25, translate["<current value>"]);
  set.create (0, 100, 25, translate["{{advanced}}Set"]);

  unsigned y = 0;
  attach(list,   0, y); y += 340 + 5;
  attach(edit,   0, y);
  attach(set,  351, y);

  load();
  list.autosize_columns();

  edit.disable();
  set.disable();

  list.on_activate = bind(&AdvancedWindow::list_activate, this);
  list.on_change   = bind(&AdvancedWindow::list_change,   this);
  set.on_tick      = bind(&AdvancedWindow::set_tick,      this);
}
