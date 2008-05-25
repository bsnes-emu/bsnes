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

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 284, string() << translate["Name"] << "\t" << translate["Value"]);
  setkey.create(0, 235, 25, translate["Assign Key"]);
  setkey.disable();
  clrkey.create(0, 235, 25, translate["Unassign Key"]);
  clrkey.disable();

  unsigned y = 0;
  attach(capture_mode,    0, y); y +=  18;
  attach(capture_always,  0, y);
  attach(capture_focus, 160, y);
  attach(capture_pause, 320, y); y +=  18 + 5;
  attach(list,            0, y); y += 284 + 5;
  attach(setkey,          0, y);
  attach(clrkey,        240, y); y +=  25 + 5;

  capture_always.on_tick = bind(&InputConfigWindow::capture_change, this);
  capture_focus.on_tick  = bind(&InputConfigWindow::capture_change, this);
  capture_pause.on_tick  = bind(&InputConfigWindow::capture_change, this);
  list.on_change         = bind(&InputConfigWindow::list_change, this);
  list.on_activate       = bind(&InputConfigWindow::set_tick, this);
  setkey.on_tick         = bind(&InputConfigWindow::set_tick, this);
  clrkey.on_tick         = bind(&InputConfigWindow::clr_tick, this);

  if(config::input.capture_mode == 1) capture_focus.check();
  else if(config::input.capture_mode == 2) capture_pause.check();
  else config::input.capture_mode = 0; //capture_always

  for(unsigned i = 0; i < inputcount; i++) list.add_item("???\t???");
  refresh_list();
  window_input_capture.setup();
}

void InputConfigWindow::refresh_list() {
  for(unsigned i = 0; i < inputcount; i++) {
    const char *name;
    acquire(i, name);
    list.set_item(i, string() << name << "\t" << input_find(get_value(i)));
  }
  list.autosize_columns();
}

uintptr_t InputConfigWindow::capture_change(Event e) {
  if(e.widget == &capture_always) config::input.capture_mode = 0;
  if(e.widget == &capture_focus)  config::input.capture_mode = 1;
  if(e.widget == &capture_pause)  config::input.capture_mode = 2;
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
  if(pos < 0 || pos >= inputcount) return true;
  window_input_capture.index = pos;
  string message = translate["Press a key to assign to $ ..."];
  const char *name;
  acquire(pos, name);
  replace(message, "$", name);
  window_input_capture.label.set_text(message);
  window_input_capture.canvas.show(pos < 24); //only show joypad graphic if setting joypad 1/2 button
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
  switch(index) {
    case  0: name = translate["Joypad 1 Up"];               return config::input.joypad1.up;
    case  1: name = translate["Joypad 1 Down"];             return config::input.joypad1.down;
    case  2: name = translate["Joypad 1 Left"];             return config::input.joypad1.left;
    case  3: name = translate["Joypad 1 Right"];            return config::input.joypad1.right;
    case  4: name = translate["Joypad 1 A"];                return config::input.joypad1.a;
    case  5: name = translate["Joypad 1 B"];                return config::input.joypad1.b;
    case  6: name = translate["Joypad 1 X"];                return config::input.joypad1.x;
    case  7: name = translate["Joypad 1 Y"];                return config::input.joypad1.y;
    case  8: name = translate["Joypad 1 L"];                return config::input.joypad1.l;
    case  9: name = translate["Joypad 1 R"];                return config::input.joypad1.r;
    case 10: name = translate["Joypad 1 Select"];           return config::input.joypad1.select;
    case 11: name = translate["Joypad 1 Start"];            return config::input.joypad1.start;

    case 12: name = translate["Joypad 2 Up"];               return config::input.joypad2.up;
    case 13: name = translate["Joypad 2 Down"];             return config::input.joypad2.down;
    case 14: name = translate["Joypad 2 Left"];             return config::input.joypad2.left;
    case 15: name = translate["Joypad 2 Right"];            return config::input.joypad2.right;
    case 16: name = translate["Joypad 2 A"];                return config::input.joypad2.a;
    case 17: name = translate["Joypad 2 B"];                return config::input.joypad2.b;
    case 18: name = translate["Joypad 2 X"];                return config::input.joypad2.x;
    case 19: name = translate["Joypad 2 Y"];                return config::input.joypad2.y;
    case 20: name = translate["Joypad 2 L"];                return config::input.joypad2.l;
    case 21: name = translate["Joypad 2 R"];                return config::input.joypad2.r;
    case 22: name = translate["Joypad 2 Select"];           return config::input.joypad2.select;
    case 23: name = translate["Joypad 2 Start"];            return config::input.joypad2.start;

    case 24: name = translate["Load Cartridge"];            return config::input.gui.load;
    case 25: name = translate["Pause Emulation"];           return config::input.gui.pause;
    case 26: name = translate["Reset System"];              return config::input.gui.reset;
    case 27: name = translate["Power Cycle System"];        return config::input.gui.power;
    case 28: name = translate["Exit Emulator"];             return config::input.gui.quit;
    case 29: name = translate["Emulation Speed Decrease"];  return config::input.gui.speed_decrease;
    case 30: name = translate["Emulation Speed Increase"];  return config::input.gui.speed_increase;
    case 31: name = translate["Frameskip Decrease"];        return config::input.gui.frameskip_decrease;
    case 32: name = translate["Frameskip Increase"];        return config::input.gui.frameskip_increase;
    case 33: name = translate["Toggle Fullscreen"];         return config::input.gui.toggle_fullscreen;
    case 34: name = translate["Toggle Menubar"];            return config::input.gui.toggle_menubar;
    case 35: name = translate["Toggle Statusbar"];          return config::input.gui.toggle_statusbar;
  }

  name = "";
  static string_setting notfound("", "", "");
  return notfound;
}

const int InputConfigWindow::inputcount = 36;

uint InputConfigWindow::get_value(uint index) {
  const char *name;
  return input_find(acquire(index, name));
}

void InputConfigWindow::set_value(uint index, uint16 value) {
  const char *name;
  acquire(index, name) = input_find(value);
  input_manager.bind();
}
