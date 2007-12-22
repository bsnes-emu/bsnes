/* InputConfigWindow */

void InputConfigWindow::setup() {
  create(0, 475, 355);

  lportA.create(0, 235, 20, "Controller Port A:");
  lportB.create(0, 235, 20, "Controller Port B:");

  portA.create(0, 235, 30);
  portA.add_item("None");
  portA.add_item("Joypad 1");
  portA.add_item("Joypad 2");
  portA.set_selection(1);
  portA.disable();

  portB.create(0, 235, 30);
  portB.add_item("None");
  portB.add_item("Joypad 1");
  portB.add_item("Joypad 2");
  portB.set_selection(2);
  portB.disable();

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 265, "Name\tValue");
  setkey.create(0, 235, 30, "Assign Key");
  setkey.disable();
  clrkey.create(0, 235, 30, "Unassign Key");
  clrkey.disable();

uint y = 0;
  attach(lportA,   0, y);
  attach(lportB, 240, y); y += 20;
  attach(portA,    0, y);
  attach(portB,  240, y); y += 30 + 5;
  attach(list,     0, y); y += 265 + 5;
  attach(setkey,   0, y);
  attach(clrkey, 240, y); y += 30 + 5;

  list.on_change   = bind(&InputConfigWindow::list_change, this);
  list.on_activate = bind(&InputConfigWindow::set_tick, this);
  setkey.on_tick   = bind(&InputConfigWindow::set_tick, this);
  clrkey.on_tick   = bind(&InputConfigWindow::clr_tick, this);

  for(uint i = 0; i < 24; i++) list.add_item("???\t???");
  refresh_list();
  window_input_capture.setup();
}

void InputConfigWindow::refresh_list() {
  for(uint i = 0; i < 24; i++) {
    list.set_item(i, string() << list_index[i] << "\t" << keymap::find(get_value(i)));
  }
  list.autosize_columns();
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
  set_value(pos, keymap::none);
  refresh_list();
  return true;
}

/* InputCaptureWindow */

void InputCaptureWindow::setup() {
  create(Window::AutoCenter, 350, 100, "bsnes Key Capture");
  label.create(0, 340, 90);
  attach(label, 5, 5);
  on_close = bind(&InputCaptureWindow::close, this);
}

void InputCaptureWindow::show() {
  uiInput->clear_input();

//enter and spacebar can be used to activate key capture,
//set lock on these keys if they are held down to prevent
//them from being instantly assigned to selected entry ...
  uiInput->poll();
  key_lock = (uiInput->key_down(keymap::enter) || uiInput->key_down(keymap::space));
  Window::focus();

  while(_term_ == false && visible() == true) {
    run();
  uint16 key = input_manager.scan();
    if(key == keymap::none) { key_lock = false; continue; }
    if(key_lock && (key == keymap::enter || key == keymap::space)) { continue; }
    hide();
    window_input_config.set_value(index, key);
    window_input_config.refresh_list();
    break;
  }

  uiInput->clear_input();
}

uintptr_t InputCaptureWindow::close(Event) {
  hide();
  return false;
}

InputCaptureWindow::InputCaptureWindow() {
  index = 0;
  key_lock = false;
}

/* Misc */

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
};

uint InputConfigWindow::get_value(uint index) {
  switch(index) {
    case  0: return keymap::find(config::input.joypad1.up);
    case  1: return keymap::find(config::input.joypad1.down);
    case  2: return keymap::find(config::input.joypad1.left);
    case  3: return keymap::find(config::input.joypad1.right);
    case  4: return keymap::find(config::input.joypad1.a);
    case  5: return keymap::find(config::input.joypad1.b);
    case  6: return keymap::find(config::input.joypad1.x);
    case  7: return keymap::find(config::input.joypad1.y);
    case  8: return keymap::find(config::input.joypad1.l);
    case  9: return keymap::find(config::input.joypad1.r);
    case 10: return keymap::find(config::input.joypad1.select);
    case 11: return keymap::find(config::input.joypad1.start);

    case 12: return keymap::find(config::input.joypad2.up);
    case 13: return keymap::find(config::input.joypad2.down);
    case 14: return keymap::find(config::input.joypad2.left);
    case 15: return keymap::find(config::input.joypad2.right);
    case 16: return keymap::find(config::input.joypad2.a);
    case 17: return keymap::find(config::input.joypad2.b);
    case 18: return keymap::find(config::input.joypad2.x);
    case 19: return keymap::find(config::input.joypad2.y);
    case 20: return keymap::find(config::input.joypad2.l);
    case 21: return keymap::find(config::input.joypad2.r);
    case 22: return keymap::find(config::input.joypad2.select);
    case 23: return keymap::find(config::input.joypad2.start);
  }

  return keymap::none;
}

void InputConfigWindow::set_value(uint index, uint16 value) {
  switch(index) {
    case  0: config::input.joypad1.up     = keymap::find(value); break;
    case  1: config::input.joypad1.down   = keymap::find(value); break;
    case  2: config::input.joypad1.left   = keymap::find(value); break;
    case  3: config::input.joypad1.right  = keymap::find(value); break;
    case  4: config::input.joypad1.a      = keymap::find(value); break;
    case  5: config::input.joypad1.b      = keymap::find(value); break;
    case  6: config::input.joypad1.x      = keymap::find(value); break;
    case  7: config::input.joypad1.y      = keymap::find(value); break;
    case  8: config::input.joypad1.l      = keymap::find(value); break;
    case  9: config::input.joypad1.r      = keymap::find(value); break;
    case 10: config::input.joypad1.select = keymap::find(value); break;
    case 11: config::input.joypad1.start  = keymap::find(value); break;

    case 12: config::input.joypad2.up     = keymap::find(value); break;
    case 13: config::input.joypad2.down   = keymap::find(value); break;
    case 14: config::input.joypad2.left   = keymap::find(value); break;
    case 15: config::input.joypad2.right  = keymap::find(value); break;
    case 16: config::input.joypad2.a      = keymap::find(value); break;
    case 17: config::input.joypad2.b      = keymap::find(value); break;
    case 18: config::input.joypad2.x      = keymap::find(value); break;
    case 19: config::input.joypad2.y      = keymap::find(value); break;
    case 20: config::input.joypad2.l      = keymap::find(value); break;
    case 21: config::input.joypad2.r      = keymap::find(value); break;
    case 22: config::input.joypad2.select = keymap::find(value); break;
    case 23: config::input.joypad2.start  = keymap::find(value); break;
  }

  input_manager.bind();
}
