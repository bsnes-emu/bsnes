void CALLBACK InputCfgTimerProc(HWND hwnd, UINT msg, UINT event, DWORD time) {
  wInputCfg.timer_key_update();
}

void InputCfgWindow::timer_key_update() {
uint32 r, n;
  r = uiInput->poll();
  if(r == 0xffff)return;

  end_key_update();

//dev_to_update is set by ui_main.cpp
  switch(dev_to_update) {
  case SNES::DEV_JOYPAD1:
    switch(key_to_update) {
    case INPUTCFG_UP:     n = config::input.joypad1.up;     break;
    case INPUTCFG_DOWN:   n = config::input.joypad1.down;   break;
    case INPUTCFG_LEFT:   n = config::input.joypad1.left;   break;
    case INPUTCFG_RIGHT:  n = config::input.joypad1.right;  break;
    case INPUTCFG_A:      n = config::input.joypad1.a;      break;
    case INPUTCFG_B:      n = config::input.joypad1.b;      break;
    case INPUTCFG_X:      n = config::input.joypad1.x;      break;
    case INPUTCFG_Y:      n = config::input.joypad1.y;      break;
    case INPUTCFG_L:      n = config::input.joypad1.l;      break;
    case INPUTCFG_R:      n = config::input.joypad1.r;      break;
    case INPUTCFG_SELECT: n = config::input.joypad1.select; break;
    case INPUTCFG_START:  n = config::input.joypad1.start;  break;
    }
    break;
  case SNES::DEV_JOYPAD2:
    switch(key_to_update) {
    case INPUTCFG_UP:     n = config::input.joypad2.up;     break;
    case INPUTCFG_DOWN:   n = config::input.joypad2.down;   break;
    case INPUTCFG_LEFT:   n = config::input.joypad2.left;   break;
    case INPUTCFG_RIGHT:  n = config::input.joypad2.right;  break;
    case INPUTCFG_A:      n = config::input.joypad2.a;      break;
    case INPUTCFG_B:      n = config::input.joypad2.b;      break;
    case INPUTCFG_X:      n = config::input.joypad2.x;      break;
    case INPUTCFG_Y:      n = config::input.joypad2.y;      break;
    case INPUTCFG_L:      n = config::input.joypad2.l;      break;
    case INPUTCFG_R:      n = config::input.joypad2.r;      break;
    case INPUTCFG_SELECT: n = config::input.joypad2.select; break;
    case INPUTCFG_START:  n = config::input.joypad2.start;  break;
    }
    break;
  }

  if(r == 0xff01) {
  //escape key was pressed, clear key
    n = 0xffff;
  } else if((r & 0xff00) != 0xff00) {
  //joy update
    n = (r & 0xff00) | (n & 0x00ff);
  } else if((r & 0x00ff) != 0x00ff) {
  //key update
    n = (n & 0xff00) | (r & 0x00ff);
  }

  switch(dev_to_update) {
  case SNES::DEV_JOYPAD1:
    switch(key_to_update) {
    case INPUTCFG_UP:     config::input.joypad1.up     = n; break;
    case INPUTCFG_DOWN:   config::input.joypad1.down   = n; break;
    case INPUTCFG_LEFT:   config::input.joypad1.left   = n; break;
    case INPUTCFG_RIGHT:  config::input.joypad1.right  = n; break;
    case INPUTCFG_A:      config::input.joypad1.a      = n; break;
    case INPUTCFG_B:      config::input.joypad1.b      = n; break;
    case INPUTCFG_X:      config::input.joypad1.x      = n; break;
    case INPUTCFG_Y:      config::input.joypad1.y      = n; break;
    case INPUTCFG_L:      config::input.joypad1.l      = n; break;
    case INPUTCFG_R:      config::input.joypad1.r      = n; break;
    case INPUTCFG_SELECT: config::input.joypad1.select = n; break;
    case INPUTCFG_START:  config::input.joypad1.start  = n; break;
    }
    break;
  case SNES::DEV_JOYPAD2:
    switch(key_to_update) {
    case INPUTCFG_UP:     config::input.joypad2.up     = n; break;
    case INPUTCFG_DOWN:   config::input.joypad2.down   = n; break;
    case INPUTCFG_LEFT:   config::input.joypad2.left   = n; break;
    case INPUTCFG_RIGHT:  config::input.joypad2.right  = n; break;
    case INPUTCFG_A:      config::input.joypad2.a      = n; break;
    case INPUTCFG_B:      config::input.joypad2.b      = n; break;
    case INPUTCFG_X:      config::input.joypad2.x      = n; break;
    case INPUTCFG_Y:      config::input.joypad2.y      = n; break;
    case INPUTCFG_L:      config::input.joypad2.l      = n; break;
    case INPUTCFG_R:      config::input.joypad2.r      = n; break;
    case INPUTCFG_SELECT: config::input.joypad2.select = n; break;
    case INPUTCFG_START:  config::input.joypad2.start  = n; break;
    }
    break;
  }
}

void InputCfgWindow::begin_key_update(uint32 key) {
  if(update_active == true)return;

  update_active = true;
  key_to_update = key;

char t[512];
  strcpy(t, "Press button or key for '");
  switch(key) {
  case INPUTCFG_UP:     strcat(t, "Up");     break;
  case INPUTCFG_DOWN:   strcat(t, "Down");   break;
  case INPUTCFG_LEFT:   strcat(t, "Left");   break;
  case INPUTCFG_RIGHT:  strcat(t, "Right");  break;
  case INPUTCFG_A:      strcat(t, "A");      break;
  case INPUTCFG_B:      strcat(t, "B");      break;
  case INPUTCFG_X:      strcat(t, "X");      break;
  case INPUTCFG_Y:      strcat(t, "Y");      break;
  case INPUTCFG_L:      strcat(t, "L");      break;
  case INPUTCFG_R:      strcat(t, "R");      break;
  case INPUTCFG_SELECT: strcat(t, "Select"); break;
  case INPUTCFG_START:  strcat(t, "Start");  break;
  }
  strcat(t, "', or press Esc to clear.");
  set_text(INPUTCFG_TEXT, t);

  SetTimer(hwnd, 1000, 50, InputCfgTimerProc);
}

void InputCfgWindow::end_key_update() {
  update_active = false;
  KillTimer(hwnd, 1000);
  set_text(INPUTCFG_TEXT, "Key updated. Select button to update.");
}

long InputCfgWindow::wndproc(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case INPUTCFG_UP:
    case INPUTCFG_DOWN:
    case INPUTCFG_LEFT:
    case INPUTCFG_RIGHT:
    case INPUTCFG_A:
    case INPUTCFG_B:
    case INPUTCFG_X:
    case INPUTCFG_Y:
    case INPUTCFG_L:
    case INPUTCFG_R:
    case INPUTCFG_SELECT:
    case INPUTCFG_START:
      begin_key_update(LOWORD(wparam));
      break;
    case INPUTCFG_ALLOW_INVALID_INPUT:
      switch(dev_to_update) {
      case SNES::DEV_JOYPAD1:
        config::input.joypad1.allow_invalid_input.toggle();
        check(INPUTCFG_ALLOW_INVALID_INPUT, (bool)config::input.joypad1.allow_invalid_input);
        break;
      case SNES::DEV_JOYPAD2:
        config::input.joypad2.allow_invalid_input.toggle();
        check(INPUTCFG_ALLOW_INVALID_INPUT, (bool)config::input.joypad2.allow_invalid_input);
        break;
      }
      break;
    }
    break;
  case WM_CLOSE:
    hide();
    return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void InputCfgWindow::show() {
  if(visible()) {
    SetFocus(hwnd);
    return;
  }

  switch(dev_to_update) {
  case SNES::DEV_JOYPAD1:
    SetWindowText(hwnd, "bsnes Input Configuration (Joypad 1)");
    check(INPUTCFG_ALLOW_INVALID_INPUT, config::input.joypad1.allow_invalid_input);
    break;
  case SNES::DEV_JOYPAD2:
    SetWindowText(hwnd, "bsnes Input Configuration (Joypad 2)");
    check(INPUTCFG_ALLOW_INVALID_INPUT, config::input.joypad2.allow_invalid_input);
    break;
  default:
    return;
  }

  update_active = false;
  set_text(INPUTCFG_TEXT, "Select button to update.");
  Window::show();
}

void InputCfgWindow::hide() {
  Window::hide();
  end_key_update();
}

void InputCfgWindow::setup() {
  control("STATIC", "",       WS_VISIBLE,   5,   5, 335,  20, INPUTCFG_TEXT);

  control("BUTTON", "L",      WS_VISIBLE, 105,  30,  40,  25, INPUTCFG_L);
  control("BUTTON", "R",      WS_VISIBLE, 210,  30,  40,  25, INPUTCFG_R);

  control("BUTTON", "Up",     WS_VISIBLE,  45,  45,  40,  25, INPUTCFG_UP);
  control("BUTTON", "Left",   WS_VISIBLE,   5,  70,  40,  25, INPUTCFG_LEFT);
  control("BUTTON", "Right",  WS_VISIBLE,  85,  70,  40,  25, INPUTCFG_RIGHT);
  control("BUTTON", "Down",   WS_VISIBLE,  45,  95,  40,  25, INPUTCFG_DOWN);

  control("BUTTON", "Select", WS_VISIBLE, 135,  85,  40,  25, INPUTCFG_SELECT);
  control("BUTTON", "Start",  WS_VISIBLE, 180,  85,  40,  25, INPUTCFG_START);

  control("BUTTON", "X",      WS_VISIBLE, 270,  45,  40,  25, INPUTCFG_X);
  control("BUTTON", "Y",      WS_VISIBLE, 230,  70,  40,  25, INPUTCFG_Y);
  control("BUTTON", "A",      WS_VISIBLE, 310,  70,  40,  25, INPUTCFG_A);
  control("BUTTON", "B",      WS_VISIBLE, 270,  95,  40,  25, INPUTCFG_B);

  control("BUTTON", "Allow invalid input (not recommended)",
    WS_VISIBLE | BS_CHECKBOX, 5, 125, 335, 20, INPUTCFG_ALLOW_INVALID_INPUT);
}
