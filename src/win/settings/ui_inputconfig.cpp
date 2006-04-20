void CALLBACK wInputConfigInputTimerProc(HWND hwnd, UINT msg, UINT event, DWORD time) {
  if(!uiInput)return;

  uiInput->ui_poll_input(&wInputConfig, false);
}

bool InputConfigWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case Input::EVENT_INPUTKEYDOWN: {
    if(button_update.active == true) {
      button_update.id = info.control_id;
      ButtonUpdateEnd();
    }
  } break;

  case EVENT_CHANGED: {
    if(info.control == &Resistance) {
      config::input.axis_resistance = Resistance.GetPos();
      ResistanceLabel.SetText("Resistance: %d%%", uint(config::input.axis_resistance));
    } else if(info.control == &Selected) {
      switch(Selected.GetSelection()) {
      case 0: AllowBadInput.Check(config::input.joypad1.allow_invalid_input); break;
      case 1: AllowBadInput.Check(config::input.joypad2.allow_invalid_input); break;
      }
    }
  } break;

  case EVENT_CLICKED: {
    if(info.control == &ButtonUp)    ButtonUpdate(BUTTON_UP);
    if(info.control == &ButtonDown)  ButtonUpdate(BUTTON_DOWN);
    if(info.control == &ButtonLeft)  ButtonUpdate(BUTTON_LEFT);
    if(info.control == &ButtonRight) ButtonUpdate(BUTTON_RIGHT);
    if(info.control == &ButtonA)     ButtonUpdate(BUTTON_A);
    if(info.control == &ButtonB)     ButtonUpdate(BUTTON_B);
    if(info.control == &ButtonX)     ButtonUpdate(BUTTON_X);
    if(info.control == &ButtonY)     ButtonUpdate(BUTTON_Y);
    if(info.control == &ButtonL)     ButtonUpdate(BUTTON_L);
    if(info.control == &ButtonR)     ButtonUpdate(BUTTON_R);
    if(info.control == &ButtonSelect)ButtonUpdate(BUTTON_SELECT);
    if(info.control == &ButtonStart) ButtonUpdate(BUTTON_START);

    if(info.control == &AllowBadInput) {
      switch(Selected.GetSelection()) {
      case 0:
        config::input.joypad1.allow_invalid_input.toggle();
        AllowBadInput.Check(config::input.joypad1.allow_invalid_input);
        break;
      case 1:
        config::input.joypad2.allow_invalid_input.toggle();
        AllowBadInput.Check(config::input.joypad2.allow_invalid_input);
        break;
      }
    }
  } break;

  }

  return false;
}

const char *InputConfigWindow::GetCaption(uint button) {
  switch(button) {
  case BUTTON_UP:     return "Up";
  case BUTTON_DOWN:   return "Down";
  case BUTTON_LEFT:   return "Left";
  case BUTTON_RIGHT:  return "Right";
  case BUTTON_A:      return "A";
  case BUTTON_B:      return "B";
  case BUTTON_X:      return "X";
  case BUTTON_Y:      return "Y";
  case BUTTON_L:      return "L";
  case BUTTON_R:      return "R";
  case BUTTON_SELECT: return "Select";
  case BUTTON_START:  return "Start";
  }

  return "";
}

void InputConfigWindow::ButtonUpdate(uint button) {
  if(button_update.active == true)return;

  button_update.active     = true;
  button_update.controller = Selected.GetSelection();
  button_update.button     = button;

  Message.SetText("Press button or key for 'Controller %d::%s', "
    "or press escape to clear button assignment.",
    button_update.controller + 1, GetCaption(button_update.button));
  SetFocus(Message.hwnd);
  SetTimer(hwnd, 0, 50, wInputConfigInputTimerProc);
}

void InputConfigWindow::ButtonUpdateEnd() {
  if(button_update.active == false)return;

  button_update.active = false;
  KillTimer(hwnd, 0);

uint old_id, id = button_update.id;
  if(id == uiInput->keymap.esc) {
  //escape key was pressed, clear key + joy mapping
    id = Input::JOYKEY_NONE;
  } else {
    switch(button_update.controller) {
    case 0:
      switch(button_update.button) {
      case BUTTON_UP:     old_id = config::input.joypad1.up;     break;
      case BUTTON_DOWN:   old_id = config::input.joypad1.down;   break;
      case BUTTON_LEFT:   old_id = config::input.joypad1.left;   break;
      case BUTTON_RIGHT:  old_id = config::input.joypad1.right;  break;
      case BUTTON_A:      old_id = config::input.joypad1.a;      break;
      case BUTTON_B:      old_id = config::input.joypad1.b;      break;
      case BUTTON_X:      old_id = config::input.joypad1.x;      break;
      case BUTTON_Y:      old_id = config::input.joypad1.y;      break;
      case BUTTON_L:      old_id = config::input.joypad1.l;      break;
      case BUTTON_R:      old_id = config::input.joypad1.r;      break;
      case BUTTON_SELECT: old_id = config::input.joypad1.select; break;
      case BUTTON_START:  old_id = config::input.joypad1.start;  break;
      }
      break;
    case 1:
      switch(button_update.button) {
      case BUTTON_UP:     old_id = config::input.joypad2.up;     break;
      case BUTTON_DOWN:   old_id = config::input.joypad2.down;   break;
      case BUTTON_LEFT:   old_id = config::input.joypad2.left;   break;
      case BUTTON_RIGHT:  old_id = config::input.joypad2.right;  break;
      case BUTTON_A:      old_id = config::input.joypad2.a;      break;
      case BUTTON_B:      old_id = config::input.joypad2.b;      break;
      case BUTTON_X:      old_id = config::input.joypad2.x;      break;
      case BUTTON_Y:      old_id = config::input.joypad2.y;      break;
      case BUTTON_L:      old_id = config::input.joypad2.l;      break;
      case BUTTON_R:      old_id = config::input.joypad2.r;      break;
      case BUTTON_SELECT: old_id = config::input.joypad2.select; break;
      case BUTTON_START:  old_id = config::input.joypad2.start;  break;
      }
      break;
    }

    if(id & Input::JOYFLAG) {
    //joypad key was pressed

    //format for EVENT_INPUTKEY(DOWN|UP) is (joypad<<8)|button,
    //format for config file is (joypad<<16)|(button<<8)|key.
    //the idea is to allow config file to have both key and joypad
    //mapping for a single SNES button, so id must be converted to
    //config file format.
      id <<= 8;

      id  &= Input::JOYMASK;
      id  |= old_id & Input::KEYMASK;
    } else {
    //keyboard key was pressed
      id  &= Input::KEYMASK;
      id  |= old_id & Input::JOYMASK;
    }

    id &= Input::JOYKEYMASK;
  }

  switch(button_update.controller) {
  case 0:
    switch(button_update.button) {
    case BUTTON_UP:     config::input.joypad1.up     = id; break;
    case BUTTON_DOWN:   config::input.joypad1.down   = id; break;
    case BUTTON_LEFT:   config::input.joypad1.left   = id; break;
    case BUTTON_RIGHT:  config::input.joypad1.right  = id; break;
    case BUTTON_A:      config::input.joypad1.a      = id; break;
    case BUTTON_B:      config::input.joypad1.b      = id; break;
    case BUTTON_X:      config::input.joypad1.x      = id; break;
    case BUTTON_Y:      config::input.joypad1.y      = id; break;
    case BUTTON_L:      config::input.joypad1.l      = id; break;
    case BUTTON_R:      config::input.joypad1.r      = id; break;
    case BUTTON_SELECT: config::input.joypad1.select = id; break;
    case BUTTON_START:  config::input.joypad1.start  = id; break;
    }
    break;
  case 1:
    switch(button_update.button) {
    case BUTTON_UP:     config::input.joypad2.up     = id; break;
    case BUTTON_DOWN:   config::input.joypad2.down   = id; break;
    case BUTTON_LEFT:   config::input.joypad2.left   = id; break;
    case BUTTON_RIGHT:  config::input.joypad2.right  = id; break;
    case BUTTON_A:      config::input.joypad2.a      = id; break;
    case BUTTON_B:      config::input.joypad2.b      = id; break;
    case BUTTON_X:      config::input.joypad2.x      = id; break;
    case BUTTON_Y:      config::input.joypad2.y      = id; break;
    case BUTTON_L:      config::input.joypad2.l      = id; break;
    case BUTTON_R:      config::input.joypad2.r      = id; break;
    case BUTTON_SELECT: config::input.joypad2.select = id; break;
    case BUTTON_START:  config::input.joypad2.start  = id; break;
    }
    break;
  }

  Message.SetText("Button updated. Select another button to update.");
}

void InputConfigWindow::Show() {
  Selected.SetSelection(0);
  AllowBadInput.Check(config::input.joypad1.allow_invalid_input);
  Message.SetText("Select button to update.");
  Window::Show();
}

void InputConfigWindow::Hide() {
  if(button_update.active == true) {
    button_update.active = false;
    KillTimer(hwnd, 0);
  }
  Window::Hide();
}

void InputConfigWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Input Configuration");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  ResistanceDesc.Create(this, "visible|sunken|vscroll|multiline|readonly", x, y, 460, 45,
    "Joypad axis resistance: (75% = default)\r\n"
    "Controls how much force needs to be applied to an analog "
    "joypad axis in any given direction to trigger a button press. (continued)\r\n\r\n"
    "Example: With a resistance of 10% (minimum), one would barely need to tap "
    "left to trigger a left button press. However, with a resistance of 90% "
    "(maximum), one would need to push the axis as far to the left as possible to "
    "trigger a left button press.\r\n\r\n"
    "Generally, a lower resistance will work better when trying to input combos (eg "
    "in fighting games), whereas a higher resistance will prevent accidental button "
    "presses.\r\n\r\n"
    "This setting only applies when the controller is in analog input mode.\r\n\r\n"
    "Because it is possible to use multiple real controllers for a single emulated "
    "SNES controller, there is only one global axis resistance setting. This setting "
    "will thusly affect all joypads connected to this computer."
  );
  y += 45;
  ResistanceLabel.Create(this, "visible", x, y + 3, 100, 15);
  ResistanceLabel.SetText("Resistance: %d%%", uint(config::input.axis_resistance));
  Resistance.Create(this, "visible", x + 100, y, 360, 25);
  Resistance.SetRange(10, 90);
  Resistance.SetPos(config::input.axis_resistance);
  y += 25;

  Separator.Create(this, "visible|sunken", x, y + 5, 460, 3);
  y += 15;

  SelectLabel.Create(this, "visible", x, y + 3, 150, 15, "Select controller to configure:");
  Selected.Create(this, "visible", x + 150, y, 100, 200);
  Selected.AddItem("Controller 1");
  Selected.AddItem("Controller 2");
  y += 25;

  Message.Create(this, "visible", x, y, 460, 15);
  y += 20;

  ButtonL.Create     (this, "visible", x + 100, y,      40, 25, "L");
  ButtonR.Create     (this, "visible", x + 205, y,      40, 25, "R");

  ButtonUp.Create    (this, "visible", x +  40, y + 15, 40, 25, "Up");
  ButtonLeft.Create  (this, "visible", x,       y + 40, 40, 25, "Left");
  ButtonRight.Create (this, "visible", x +  80, y + 40, 40, 25, "Right");
  ButtonDown.Create  (this, "visible", x +  40, y + 65, 40, 25, "Down");

  ButtonSelect.Create(this, "visible", x + 130, y + 55, 40, 25, "Select");
  ButtonStart.Create (this, "visible", x + 180, y + 55, 40, 25, "Start");

  ButtonX.Create     (this, "visible", x + 270, y + 15, 40, 25, "X");
  ButtonY.Create     (this, "visible", x + 230, y + 40, 40, 25, "Y");
  ButtonA.Create     (this, "visible", x + 310, y + 40, 40, 25, "A");
  ButtonB.Create     (this, "visible", x + 270, y + 65, 40, 25, "B");

  AllowBadInput.Create(this, "visible", x, y + 95, 460, 15,
    "Allow up+down and left+right button combinations (not recommended -- can cause bugs)");
}

InputConfigWindow::InputConfigWindow() {
  button_update.active = false;
}
