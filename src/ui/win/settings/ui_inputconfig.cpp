void CALLBACK wInputConfigInputTimerProc(HWND hwnd, UINT msg, UINT event, DWORD time) {
  if(!uiInput)return;

  uiInput->ui_poll_input(&wInputConfig, false);
}

bool InputConfigWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_DRAW: {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  HDC hdcsrc = CreateCompatibleDC(hdc);
  HBITMAP hbm = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(102));
    SelectObject(hdcsrc, hbm);
    BitBlt(hdc, 285, 139, 190, 100, hdcsrc, 0, 0, SRCCOPY);
    DeleteDC(hdcsrc);
    DeleteObject(hbm);
    EndPaint(hwnd, &ps);
  } break;

  case Input::EVENT_INPUTKEYDOWN: {
    if(button_update.active == true) {
      button_update.id = info.control_id;
      ButtonUpdateEnd();
    }
  } break;

  case EVENT_CHANGED: {
    if(info.control == &Resistance) {
      config::input.axis_resistance = Resistance.GetPos();
      ResistanceLabel.SetText("Joypad axis resistance: %d%%", uint(config::input.axis_resistance));
    } else if(info.control == &Selected) {
      switch(Selected.GetSelection()) {
      case 0: AllowBadInput.Check(config::input.joypad1.allow_invalid_input); break;
      case 1: AllowBadInput.Check(config::input.joypad2.allow_invalid_input); break;
      }
      UpdateButtonList();
    }
  } break;

  case EVENT_DOUBLECLICKED: {
    if(info.control == &ButtonList) {
    int sel = ButtonList.GetSelection();
      if(sel != -1)ButtonUpdateBegin(sel);
    }
  } break;

  case EVENT_CLICKED: {
    if(info.control == &ButtonUpdate) {
    int sel = ButtonList.GetSelection();
      if(sel != -1)ButtonUpdateBegin(sel);
    } else if(info.control == &ButtonClear) {
    int sel = ButtonList.GetSelection();
      if(sel != -1) {
        ButtonUpdateBegin(sel);
        button_update.id = uiInput->keymap.esc;
        ButtonUpdateEnd();
      }
    } else if(info.control == &AllowBadInput) {
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
  case SNES::JOYPAD_UP:     return "Up";
  case SNES::JOYPAD_DOWN:   return "Down";
  case SNES::JOYPAD_LEFT:   return "Left";
  case SNES::JOYPAD_RIGHT:  return "Right";
  case SNES::JOYPAD_A:      return "A";
  case SNES::JOYPAD_B:      return "B";
  case SNES::JOYPAD_X:      return "X";
  case SNES::JOYPAD_Y:      return "Y";
  case SNES::JOYPAD_L:      return "L";
  case SNES::JOYPAD_R:      return "R";
  case SNES::JOYPAD_SELECT: return "Select";
  case SNES::JOYPAD_START:  return "Start";
  }
  return "";
}

void InputConfigWindow::UpdateButtonList() {
  ButtonList.DeleteAllItems();
uint device = Selected.GetSelection();
char t[512], tmp[512];
string str, part;
#define add(__label, __bn) \
  strcpy(tmp, uiInput->keymaptostring(uiInput->get_keymap(device, SNES::JOYPAD_##__bn))); \
  split(part, " | ", tmp); \
  replace(part[0], "keynone", "<none>"); \
  if(count(part) < 2)strcpy(part[1], "<none>"); \
  sprintf(t, #__label "|%s|%s", strptr(part[0]), strptr(part[1])); \
  ButtonList.AddItem(t);

  add(Up, UP); add(Down, DOWN); add(Left, LEFT); add(Right, RIGHT);
  add(Y, Y); add(X, X); add(B, B); add(A, A);
  add(L, L); add(R, R); add(Select, SELECT); add(Start, START);
#undef add
}

void InputConfigWindow::ButtonUpdateBegin(uint button) {
  if(button_update.active == true)return;

//        0,  1,    2,      3,     4,  5,    6,    7,     8, 9, 10,     11
//convert Up, Down, Left,   Right, Y,  X,    B,    A,     L, R, Select, Start
//to      B,  Y,    Select, Start, Up, Down, Left, Right, A, X, L,      R
uint translate_table[12] = { 4, 5, 6, 7, 1, 9, 0, 8, 10, 11, 2, 3 };
  button_update.active     = true;
  button_update.controller = Selected.GetSelection();
  button_update.button     = translate_table[button];

  Message.SetText("Press key or button for 'Controller %d::%s', "
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
    old_id = uiInput->get_keymap(button_update.controller, button_update.button);

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

  uiInput->set_keymap(button_update.controller, button_update.button, id);

  Message.SetText("Button updated. Select another button to update.");
int sel = ButtonList.GetSelection();
  UpdateButtonList();
  ButtonList.SetSelection(sel);
}

void InputConfigWindow::Show() {
  Selected.SetSelection(0);
  AllowBadInput.Check(config::input.joypad1.allow_invalid_input);
  Message.SetText("Select button to update.");
  UpdateButtonList();
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
  ResistanceLabel.Create(this, "visible", x, y + 3, 150, 15);
  ResistanceLabel.SetText("Joypad axis resistance: %d%%", uint(config::input.axis_resistance));
  Resistance.Create(this, "visible", x + 150, y, 310, 25);
  Resistance.SetRange(10, 90);
  Resistance.SetPos(config::input.axis_resistance);
  y += 25;

  SelectLabel.Create(this, "visible", x, y + 3, 150, 15, "Select controller to configure:");
  Selected.Create(this, "visible", x + 150, y, 100, 200);
  Selected.AddItem("Controller 1");
  Selected.AddItem("Controller 2");
  y += 25;

  Separator.Create(this, "visible|sunken", x, y + 1, 460, 3);
  y += 9;

  Message.Create(this, "visible", x, y, 460, 15);
  y += 20;

  ButtonList.Create(this, "visible|edge", x, y, 265, 195);
  ButtonList.AddColumn(Listview::LEFT,  61, "Button");
  ButtonList.AddColumn(Listview::LEFT, 100, "Key Mapping");
  ButtonList.AddColumn(Listview::LEFT, 100, "Joypad Mapping");
  ButtonUpdate.Create(this, "visible", x + 270, y, 92, 25, "Update");
  ButtonClear.Create(this, "visible", x + 270 + 98, y, 92, 25, "Clear");
  y += 200;

  AllowBadInput.Create(this, "visible", x, y, 460, 15,
    "Allow up+down and left+right button combinations (not recommended -- can trigger bugs)");
  y += 15;
}

InputConfigWindow::InputConfigWindow() {
  button_update.active = false;
}
