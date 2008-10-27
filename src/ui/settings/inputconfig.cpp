/* InputConfigWindow */

void InputConfigWindow::setup() {
  create(0, 475, 355);

  capture_mode.create(0, 475, 18, translate["{{input}}When emulation window does not have focus:"]);
  RadioboxGroup group;
  group.add(&capture_always);
  group.add(&capture_focus);
  group.add(&capture_pause);
  capture_always.create(group, 0, 155, 18, translate["{{input}}Allow input"]);
  capture_focus.create (group, 0, 155, 18, translate["{{input}}Ignore input"]);
  capture_pause.create (group, 0, 155, 18, translate["{{input}}Pause emulation"]);

  config_type.create(0, 235, 25);
  config_type.add_item(translate["{{input}}Controller Port 1"]);
  config_type.add_item(translate["{{input}}Controller Port 2"]);
  config_type.add_item(translate["{{input}}User Interface"]);
  config_type.set_selection(0);

  config_subtype.create(0, 235, 25);
  refresh_subtype();

  list.create(Listbox::Header | Listbox::VerticalScrollAlways, 475, 254,
    string() << translate["{{input}}Name"] << "\t" << translate["{{input}}Value"]);
  setkey.create(0, 235, 25, string() << translate["{{input}}Assign Key"] << " ...");
  setkey.disable();
  clrkey.create(0, 235, 25, translate["{{input}}Unassign Key"]);
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

InputGroup* InputConfigWindow::get_group() {
  unsigned port  = config_type.get_selection();
  unsigned index = config_subtype.get_selection();

  if(port == 0 || port == 1) {
    //SNES controller
    InputDevice *device = 0;
    for(unsigned i = 0; i < inputpool.list.size(); i++) {
      if(inputpool.list[i]->port == port) {
        if(index-- == 0) {
          device = inputpool.list[i];
          break;
        }
      }
    }
    return device;
  } else {
    //user interface
    return &inputuigeneral;
  }
}

bool InputConfigWindow::assign(uint16_t code) {
  InputGroup *group = get_group();
  if(!group) return false;

  int pos = list.get_selection();
  if(pos < 0 || pos >= group->list.size()) return false;

  //make sure boolean buttons map to boolean input; axes to mouse / joypad axes
  switch(group->list[pos]->type) {
    case InputObject::Button: {
      if(InputCode::is_button(code) == false) return false;
    } break;

    case InputObject::Axis: {
      if(InputCode::is_axis(code) == false) return false;
      int16_t state = input_manager.state(code);
      //add a bit of resistance to prevent infinitesimally small movements from triggering assignment
      if(InputCode::type(code) == InputCode::MouseAxis  && abs(state) < 8) return false;
      //joypad axis range = -32768 to +32767
      //some joypads have pressure-sensitive buttons that read +32767 when depressed ...
      //therefore, range test between 25% and 75% pressure before triggering assignment
      if(InputCode::type(code) == InputCode::JoypadAxis && (abs(state) < 8192 || abs(state) > 24576)) return false;
    } break;
  }

  group->list[pos]->setting = input_find(code);
  input_manager.bind();
  return true;
}

void InputConfigWindow::refresh_subtype() {
  config_subtype.reset();
  unsigned port = config_type.get_selection();

  if(port == 0 || port == 1) {
    //SNES controller
    for(unsigned device = 0; device < inputpool.list.size(); device++) {
      if(inputpool.list[device]->port == port) {
        config_subtype.add_item(translate[inputpool.list[device]->name]);
      }
    }
  } else {
    //user interface
    config_subtype.add_item(translate[inputuigeneral.name]);
  }

  config_subtype.set_selection(0);
}

void InputConfigWindow::refresh_list() {
  setkey.disable();
  clrkey.disable();
  list.reset();
  InputGroup *group = get_group();
  if(!group) return;

  for(unsigned i = 0; i < group->list.size(); i++) {
    list.add_item(string()
    << translate[group->list[i]->name]
    << "\t"
    << group->list[i]->setting);
  }

  list.autosize_columns();
}

uintptr_t InputConfigWindow::capture_change(event_t e) {
  if(e.widget == &capture_always) config::input.capture_mode = 0;
  if(e.widget == &capture_focus)  config::input.capture_mode = 1;
  if(e.widget == &capture_pause)  config::input.capture_mode = 2;
  return true;
}

uintptr_t InputConfigWindow::type_change(event_t) {
  refresh_subtype();
  refresh_list();
  return true;
}

uintptr_t InputConfigWindow::subtype_change(event_t) {
  refresh_list();
  return true;
}

uintptr_t InputConfigWindow::list_change(event_t) {
  int pos = list.get_selection();
  setkey.enable(pos >= 0);
  clrkey.enable(pos >= 0);
  return true;
}

uintptr_t InputConfigWindow::set_tick(event_t) {
  InputGroup *group = get_group();
  if(!group) return true;

  int pos = list.get_selection();
  if(pos < 0 || pos >= group->list.size()) return true;

  string message;
  if(group->list[pos]->type == InputObject::Button) {
    message = translate["Press a key or button to assign to $ ..."];
  } else {
    message = translate["Move mouse or analog joypad axis to assign to $ ..."];
  }

  replace(message, "$", translate[group->list[pos]->name]);
  window_input_capture.label.set_text(message);

  bool show_controller_graphic = false;
  InputDevice *device = dynamic_cast<InputDevice*>(group);
  if(device) {
    SNES::Input::DeviceID id = device->id;
    if(id == SNES::Input::DeviceIDJoypad1    || id == SNES::Input::DeviceIDJoypad2
    || id == SNES::Input::DeviceIDMultitap1A || id == SNES::Input::DeviceIDMultitap2A
    || id == SNES::Input::DeviceIDMultitap1B || id == SNES::Input::DeviceIDMultitap2B
    || id == SNES::Input::DeviceIDMultitap1C || id == SNES::Input::DeviceIDMultitap2C
    || id == SNES::Input::DeviceIDMultitap1D || id == SNES::Input::DeviceIDMultitap2D
    ) {
      show_controller_graphic = true;
    }
  }

  window_input_capture.index = pos;
  window_input_capture.canvas.show(show_controller_graphic);
  window_input_capture.show();
  return true;
}

uintptr_t InputConfigWindow::clr_tick(event_t) {
  if(list.get_selection() >= 0) {
    assign(keyboard::none);
    refresh_list();
  }
  return true;
}

/* InputCaptureWindow */

void InputCaptureWindow::assign(uint16_t code) {
  if(window_input_config.assign(code)) {
    waiting = false;
    hide();
    window_input_config.refresh_list();
    input_manager.flush();
  }
}

void InputCaptureWindow::show() {
  input_manager.refresh();
  waiting = true;
  //certain keys (eg spacebar) can activate the key assignment window,
  //which would then auto-bind those keys. detect these keys, and set
  //a lock so that these keys will not be registered unless they
  //are released and then re-pressed.
  locked = input_manager.state(keyboard::return_)
        || input_manager.state(keyboard::spacebar)
        || input_manager.state(mouse::button + 0);
  Window::focus();
}

uintptr_t InputCaptureWindow::close(event_t) {
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
