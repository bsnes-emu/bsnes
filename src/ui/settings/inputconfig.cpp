/* InputConfigWindow */

void InputConfigWindow::setup() {
  create(0, 475, 355);

  capture_mode.create(0, 475, 18, translate["When emulation window does not have focus:"]);
  RadioboxGroup group;
  group.add(&capture_always);
  group.add(&capture_focus);
  group.add(&capture_pause);
  capture_always.create(group, 0, 155, 18, translate["Allow input"]);
  capture_focus.create (group, 0, 155, 18, translate["Ignore input"]);
  capture_pause.create (group, 0, 155, 18, translate["Pause emulation"]);

  config_type.create(0, 235, 25);
  config_type.add_item(translate["Controller Port 1"]);
  config_type.add_item(translate["Controller Port 2"]);
  config_type.add_item(translate["User Interface"]);
  config_type.set_selection(0);

  config_subtype.create(0, 235, 25);
  refresh_subtype();

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 254, string() << translate["Name"] << "\t" << translate["Value"]);
  setkey.create(0, 235, 25, translate["Assign Key"]);
  setkey.disable();
  clrkey.create(0, 235, 25, translate["Unassign Key"]);
  clrkey.disable();

  unsigned y = 0;
  attach(capture_mode,           0, y); y +=  18;
  attach(capture_always,         0, y);
  attach(capture_focus,        160, y);
  attach(capture_pause,        320, y); y +=  18 + 5;
  attach(config_type,            0, y);
  attach(config_subtype,       240, y); y +=  25 + 5;
  attach(list,                   0, y); y += 254 + 5;
  attach(setkey,                 0, y);
  attach(clrkey,               240, y); y +=  25 + 5;

  capture_always.on_tick   = bind(&InputConfigWindow::capture_change, this);
  capture_focus.on_tick    = bind(&InputConfigWindow::capture_change, this);
  capture_pause.on_tick    = bind(&InputConfigWindow::capture_change, this);
  config_type.on_change    = bind(&InputConfigWindow::type_change, this);
  config_subtype.on_change = bind(&InputConfigWindow::subtype_change, this);
  list.on_change           = bind(&InputConfigWindow::list_change, this);
  list.on_activate         = bind(&InputConfigWindow::set_tick, this);
  setkey.on_tick           = bind(&InputConfigWindow::set_tick, this);
  clrkey.on_tick           = bind(&InputConfigWindow::clr_tick, this);

  if(config::input.capture_mode == 1) capture_focus.check();
  else if(config::input.capture_mode == 2) capture_pause.check();
  else config::input.capture_mode = 0; //capture_always

  refresh_list();
  window_input_capture.setup();
}

InputConfigWindow::InputType InputConfigWindow::get_input_type(unsigned &length) {
  unsigned type = config_type.get_selection();
  unsigned subtype = config_subtype.get_selection();

  switch(type) {
    case 0: {
      switch(subtype) {
        case 0: length = 12; return Port1_Joypad;
        case 1: length = 12; return Port1_Multitap1;
        case 2: length = 12; return Port1_Multitap2;
        case 3: length = 12; return Port1_Multitap3;
        case 4: length = 12; return Port1_Multitap4;
      }
    } break;

    case 1: {
      switch(subtype) {
        case 0: length = 12; return Port2_Joypad;
        case 1: length = 12; return Port2_Multitap1;
        case 2: length = 12; return Port2_Multitap2;
        case 3: length = 12; return Port2_Multitap3;
        case 4: length = 12; return Port2_Multitap4;
      }
    } break;

    case 2: {
      switch(subtype) {
        case 0: length = 12; return UI_General;
      }
    } break;
  }

  return TypeUnknown;
}

void InputConfigWindow::refresh_subtype() {
  config_subtype.reset();

  switch(config_type.get_selection()) {
    case 0:
    case 1: {
      config_subtype.add_item(translate["Joypad"]);
      config_subtype.add_item(translate["Multitap Port 1"]);
      config_subtype.add_item(translate["Multitap Port 2"]);
      config_subtype.add_item(translate["Multitap Port 3"]);
      config_subtype.add_item(translate["Multitap Port 4"]);
    } break;

    case 2: {
      config_subtype.add_item(translate["General"]);
    } break;
  }

  config_subtype.set_selection(0);
}

void InputConfigWindow::refresh_list() {
  list.reset();
  unsigned length;
  get_input_type(length);
  for(unsigned i = 0; i < length; i++) {
    const char *name;
    acquire(i, name);
    list.add_item(string() << name << "\t" << input_find(get_value(i)));
  }
  list.autosize_columns();
}

uintptr_t InputConfigWindow::capture_change(Event e) {
  if(e.widget == &capture_always) config::input.capture_mode = 0;
  if(e.widget == &capture_focus)  config::input.capture_mode = 1;
  if(e.widget == &capture_pause)  config::input.capture_mode = 2;
  return true;
}

uintptr_t InputConfigWindow::type_change(Event) {
  refresh_subtype();
  refresh_list();
  return true;
}

uintptr_t InputConfigWindow::subtype_change(Event) {
  refresh_list();
  return true;
}

uintptr_t InputConfigWindow::list_change(Event) {
  int pos = list.get_selection();
  setkey.enable(pos >= 0);
  clrkey.enable(pos >= 0);
  return true;
}

uintptr_t InputConfigWindow::set_tick(Event) {
  int pos = list.get_selection();
  if(pos < 0) return true;
  window_input_capture.index = pos;
  string message = translate["Press a key to assign to $ ..."];
  const char *name;
  acquire(pos, name);
  replace(message, "$", name);
  window_input_capture.label.set_text(message);
  window_input_capture.canvas.show(config_type.get_selection() < 2); //only show joypad graphic if setting joypad button
  window_input_capture.show();
  return true;
}

uintptr_t InputConfigWindow::clr_tick(Event) {
  int pos = list.get_selection();
  if(pos < 0) return true;
  set_value(pos, keyboard::none);
  refresh_list();
  return true;
}

/* InputCaptureWindow */

void InputCaptureWindow::assign(uint16_t key) {
  waiting = false;
  hide();
  window_input_config.set_value(index, key);
  window_input_config.refresh_list();
  input.clear();
}

void InputCaptureWindow::show() {
  input.poll();
  waiting = true;
  locked = input.key_down(keyboard::return_) || input.key_down(keyboard::spacebar);
  Window::focus();
}

uintptr_t InputCaptureWindow::close(Event) {
  hide();
  return false;
}

void InputCaptureWindow::setup() {
  create(Window::AutoCenter, 382, 206, translate["bsnes Key Capture"]);
  label.create(0, 340, 18);
  canvas.create(0, 372, 178);
  memcpy(canvas.buffer(), resource::controller, 372 * 178 * 4);
  attach(label, 5, 5);
  attach(canvas, 5, 23);
  on_close = bind(&InputCaptureWindow::close, this);
}

InputCaptureWindow::InputCaptureWindow() {
  waiting = false;
  locked = false;
  index = 0;
}

/* Misc */

string_setting& InputConfigWindow::acquire(unsigned index, const char *&name) {
  #define map(n, lname) \
    case n: { \
      switch(index) { \
        case  0: name = translate["Up"];     return config::input.lname.up;     \
        case  1: name = translate["Down"];   return config::input.lname.down;   \
        case  2: name = translate["Left"];   return config::input.lname.left;   \
        case  3: name = translate["Right"];  return config::input.lname.right;  \
        case  4: name = translate["A"];      return config::input.lname.a;      \
        case  5: name = translate["B"];      return config::input.lname.b;      \
        case  6: name = translate["X"];      return config::input.lname.x;      \
        case  7: name = translate["Y"];      return config::input.lname.y;      \
        case  8: name = translate["L"];      return config::input.lname.l;      \
        case  9: name = translate["R"];      return config::input.lname.r;      \
        case 10: name = translate["Select"]; return config::input.lname.select; \
        case 11: name = translate["Start"];  return config::input.lname.start;  \
      } \
    } break;

  unsigned length;
  switch(get_input_type(length)) { default:
    map(Port1_Joypad,    joypad1)
    map(Port1_Multitap1, multitap1a)
    map(Port1_Multitap2, multitap1b)
    map(Port1_Multitap3, multitap1c)
    map(Port1_Multitap4, multitap1d)

    map(Port2_Joypad,    joypad2)
    map(Port2_Multitap1, multitap2a)
    map(Port2_Multitap2, multitap2b)
    map(Port2_Multitap3, multitap2c)
    map(Port2_Multitap4, multitap2d)

    case UI_General: {
      switch(index) {
        case  0: name = translate["Load Cartridge"];            return config::input.gui.load;
        case  1: name = translate["Pause Emulation"];           return config::input.gui.pause;
        case  2: name = translate["Reset System"];              return config::input.gui.reset;
        case  3: name = translate["Power Cycle System"];        return config::input.gui.power;
        case  4: name = translate["Exit Emulator"];             return config::input.gui.quit;
        case  5: name = translate["Emulation Speed Decrease"];  return config::input.gui.speed_decrease;
        case  6: name = translate["Emulation Speed Increase"];  return config::input.gui.speed_increase;
        case  7: name = translate["Frameskip Decrease"];        return config::input.gui.frameskip_decrease;
        case  8: name = translate["Frameskip Increase"];        return config::input.gui.frameskip_increase;
        case  9: name = translate["Toggle Fullscreen"];         return config::input.gui.toggle_fullscreen;
        case 10: name = translate["Toggle Menubar"];            return config::input.gui.toggle_menubar;
        case 11: name = translate["Toggle Statusbar"];          return config::input.gui.toggle_statusbar;
      }
    } break;
  }

  #undef map

  name = "";
  static string_setting notfound("", "", "");
  return notfound;
}

uint InputConfigWindow::get_value(uint index) {
  const char *name;
  return input_find(acquire(index, name));
}

void InputConfigWindow::set_value(uint index, uint16 value) {
  const char *name;
  acquire(index, name) = input_find(value);
  input_manager.bind();
}
