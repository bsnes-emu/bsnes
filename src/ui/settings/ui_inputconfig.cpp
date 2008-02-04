/* InputConfigWindow */

void InputConfigWindow::setup() {
  create(0, 475, 355);

  capture_mode.create(0, 475, 15, "When emulation window does not have focus:");
  RadioboxGroup group;
  group.add(&capture_always);
  group.add(&capture_focus);
  group.add(&capture_pause);
  capture_always.create(group, 0, 155, 20, "Allow input");
  capture_focus.create(group, 0, 155, 20, "Ignore input");
  capture_pause.create(group, 0, 155, 20, "Pause emulation");

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 275, "Name\tValue");
  setkey.create(0, 235, 30, "Assign Key");
  setkey.disable();
  clrkey.create(0, 235, 30, "Unassign Key");
  clrkey.disable();

uint y = 0;
  attach(capture_mode,    0, y); y +=  15 + 5;
  attach(capture_always,  0, y);
  attach(capture_focus, 160, y);
  attach(capture_pause, 320, y); y +=  20 + 5;
  attach(list,            0, y); y += 275 + 5;
  attach(setkey,          0, y);
  attach(clrkey,        240, y); y +=  30 + 5;

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

  for(uint i = 0; i < list_size; i++) list.add_item("???\t???");
  refresh_list();
  window_input_capture.setup();
}

void InputConfigWindow::refresh_list() {
  for(uint i = 0; i < list_size; i++) {
    list.set_item(i, string() << list_index[i] << "\t" << input_find(get_value(i)));
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
  if(pos < 0) return true;
  window_input_capture.index = pos;
  window_input_capture.label.set_text(string() << "Press a key to assign to " << list_index[pos] << " ...");
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
  create(Window::AutoCenter, 350, 100, "bsnes Key Capture");
  label.create(0, 340, 90);
  attach(label, 5, 5);
  on_close = bind(&InputCaptureWindow::close, this);
}

InputCaptureWindow::InputCaptureWindow() {
  waiting = false;
  locked = false;
  index = 0;
}

/* Misc */

const int InputConfigWindow::list_size = 31;

const char InputConfigWindow::list_index[][64] = {
  "Joypad 1 Up",
  "Joypad 1 Down",
  "Joypad 1 Left",
  "Joypad 1 Right",
  "Joypad 1 A",
  "Joypad 1 B",
  "Joypad 1 X",
  "Joypad 1 Y",
  "Joypad 1 L",
  "Joypad 1 R",
  "Joypad 1 Select",
  "Joypad 1 Start",

  "Joypad 2 Up",
  "Joypad 2 Down",
  "Joypad 2 Left",
  "Joypad 2 Right",
  "Joypad 2 A",
  "Joypad 2 B",
  "Joypad 2 X",
  "Joypad 2 Y",
  "Joypad 2 L",
  "Joypad 2 R",
  "Joypad 2 Select",
  "Joypad 2 Start",

  "Load Cartridge",
  "Pause Emulation",
  "Reset System",
  "Power Cycle System",
  "Toggle Fullscreen",
  "Toggle Menubar",
  "Toggle Statusbar",
};

uint InputConfigWindow::get_value(uint index) {
  switch(index) {
    case  0: return input_find(config::input.joypad1.up);
    case  1: return input_find(config::input.joypad1.down);
    case  2: return input_find(config::input.joypad1.left);
    case  3: return input_find(config::input.joypad1.right);
    case  4: return input_find(config::input.joypad1.a);
    case  5: return input_find(config::input.joypad1.b);
    case  6: return input_find(config::input.joypad1.x);
    case  7: return input_find(config::input.joypad1.y);
    case  8: return input_find(config::input.joypad1.l);
    case  9: return input_find(config::input.joypad1.r);
    case 10: return input_find(config::input.joypad1.select);
    case 11: return input_find(config::input.joypad1.start);

    case 12: return input_find(config::input.joypad2.up);
    case 13: return input_find(config::input.joypad2.down);
    case 14: return input_find(config::input.joypad2.left);
    case 15: return input_find(config::input.joypad2.right);
    case 16: return input_find(config::input.joypad2.a);
    case 17: return input_find(config::input.joypad2.b);
    case 18: return input_find(config::input.joypad2.x);
    case 19: return input_find(config::input.joypad2.y);
    case 20: return input_find(config::input.joypad2.l);
    case 21: return input_find(config::input.joypad2.r);
    case 22: return input_find(config::input.joypad2.select);
    case 23: return input_find(config::input.joypad2.start);

    case 24: return input_find(config::input.gui.load);
    case 25: return input_find(config::input.gui.pause);
    case 26: return input_find(config::input.gui.reset);
    case 27: return input_find(config::input.gui.power);
    case 28: return input_find(config::input.gui.toggle_fullscreen);
    case 29: return input_find(config::input.gui.toggle_menubar);
    case 30: return input_find(config::input.gui.toggle_statusbar);
  }

  return keyboard::none;
}

void InputConfigWindow::set_value(uint index, uint16 value) {
  switch(index) {
    case  0: config::input.joypad1.up     = input_find(value); break;
    case  1: config::input.joypad1.down   = input_find(value); break;
    case  2: config::input.joypad1.left   = input_find(value); break;
    case  3: config::input.joypad1.right  = input_find(value); break;
    case  4: config::input.joypad1.a      = input_find(value); break;
    case  5: config::input.joypad1.b      = input_find(value); break;
    case  6: config::input.joypad1.x      = input_find(value); break;
    case  7: config::input.joypad1.y      = input_find(value); break;
    case  8: config::input.joypad1.l      = input_find(value); break;
    case  9: config::input.joypad1.r      = input_find(value); break;
    case 10: config::input.joypad1.select = input_find(value); break;
    case 11: config::input.joypad1.start  = input_find(value); break;

    case 12: config::input.joypad2.up     = input_find(value); break;
    case 13: config::input.joypad2.down   = input_find(value); break;
    case 14: config::input.joypad2.left   = input_find(value); break;
    case 15: config::input.joypad2.right  = input_find(value); break;
    case 16: config::input.joypad2.a      = input_find(value); break;
    case 17: config::input.joypad2.b      = input_find(value); break;
    case 18: config::input.joypad2.x      = input_find(value); break;
    case 19: config::input.joypad2.y      = input_find(value); break;
    case 20: config::input.joypad2.l      = input_find(value); break;
    case 21: config::input.joypad2.r      = input_find(value); break;
    case 22: config::input.joypad2.select = input_find(value); break;
    case 23: config::input.joypad2.start  = input_find(value); break;

    case 24: config::input.gui.load              = input_find(value); break;
    case 25: config::input.gui.pause             = input_find(value); break;
    case 26: config::input.gui.reset             = input_find(value); break;
    case 27: config::input.gui.power             = input_find(value); break;
    case 28: config::input.gui.toggle_fullscreen = input_find(value); break;
    case 29: config::input.gui.toggle_menubar    = input_find(value); break;
    case 30: config::input.gui.toggle_statusbar  = input_find(value); break;
  }

  input_manager.bind();
}
