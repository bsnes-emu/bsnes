long InputConfig::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
HDC hdc, hdcsrc;
PAINTSTRUCT ps;
HBITMAP hbm;
  switch(msg) {
  case WM_PAINT:
    hdc = BeginPaint(hwnd, &ps);
    hdcsrc = CreateCompatibleDC(hdc);
    hbm = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(100));
    SelectObject(hdcsrc, hbm);
    BitBlt(hdc, 5, 25, 400, 200, hdcsrc, 0, 0, SRCCOPY);
    DeleteDC(hdcsrc);
    DeleteObject(hbm);
    EndPaint(hwnd, &ps);
    break;
  case WM_KEYUP:
    update_polling((int)wparam);
    break;
  case WM_CLOSE:
    polling = false;
    hide();
    return 0;
  case WM_DESTROY:
    alert("Error: Input Configuration window destroyed");
    return 0;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

long __stdcall wndproc_inputconfig(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return w_inputconfig->wndproc(hwnd, msg, wparam, lparam);
}

void InputConfig::update_polling(int key) {
bool end_config = false;
  if(polling == false)return;

  switch(config_type) {
  case JOYPAD1:
    switch(config_pos) {
    case  0:config::input.joypad1.up     = key;break;
    case  1:config::input.joypad1.down   = key;break;
    case  2:config::input.joypad1.left   = key;break;
    case  3:config::input.joypad1.right  = key;break;
    case  4:config::input.joypad1.select = key;break;
    case  5:config::input.joypad1.start  = key;break;
    case  6:config::input.joypad1.x      = key;break;
    case  7:config::input.joypad1.y      = key;break;
    case  8:config::input.joypad1.a      = key;break;
    case  9:config::input.joypad1.b      = key;break;
    case 10:config::input.joypad1.l      = key;break;
    case 11:config::input.joypad1.r      = key;break;
    case 12:end_config = true;break;
    }
    config_pos++;
    break;
  case JOYPAD2:
    switch(config_pos) {
    case  0:config::input.joypad2.up     = key;break;
    case  1:config::input.joypad2.down   = key;break;
    case  2:config::input.joypad2.left   = key;break;
    case  3:config::input.joypad2.right  = key;break;
    case  4:config::input.joypad2.select = key;break;
    case  5:config::input.joypad2.start  = key;break;
    case  6:config::input.joypad2.x      = key;break;
    case  7:config::input.joypad2.y      = key;break;
    case  8:config::input.joypad2.a      = key;break;
    case  9:config::input.joypad2.b      = key;break;
    case 10:config::input.joypad2.l      = key;break;
    case 11:config::input.joypad2.r      = key;break;
    case 12:end_config = true;break;
    }
    config_pos++;
    break;
  }

  if(end_config == true) {
    polling = false;
    hide();
    return;
  }

  update_command();
}

void InputConfig::update_command() {
  switch(config_type) {
  case JOYPAD1:
  case JOYPAD2:
    switch(config_pos) {
    case  0:set_text("Press key for Up...");    break;
    case  1:set_text("Press key for Down...");  break;
    case  2:set_text("Press key for Left...");  break;
    case  3:set_text("Press key for Right..."); break;
    case  4:set_text("Press key for Select...");break;
    case  5:set_text("Press key for Start..."); break;
    case  6:set_text("Press key for X...");     break;
    case  7:set_text("Press key for Y...");     break;
    case  8:set_text("Press key for A...");     break;
    case  9:set_text("Press key for B...");     break;
    case 10:set_text("Press key for L...");     break;
    case 11:set_text("Press key for R...");     break;
    case 12:set_text("Configuration complete. Press any key to close window...");break;
    }
    break;
  }
}

void InputConfig::begin_config(int type) {
  if(visible == true) {
    center();
    SetFocus(hwnd);
    return;
  }

  config_type = type;
  config_pos  = 0;
  switch(config_type) {
  case JOYPAD1:
    SetWindowText(hwnd, "bsnes Input Configuration (Joypad1)");
    break;
  case JOYPAD2:
    SetWindowText(hwnd, "bsnes Input Configuration (Joypad2)");
    break;
  default:
    return;
  }

  polling = true;
  update_command();

  center();
  show();
}

void InputConfig::set_text(char *str) {
  SetDlgItemText(hwnd, ID_COMMAND, str);
}

void InputConfig::create() {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc_inputconfig;
  wc.lpszClassName = "bsnes_inputconfig";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindow("bsnes_inputconfig", "bsnes input configuration",
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 410, 230,
    0, 0, GetModuleHandle(0), 0);

  resize(410, 230);
  center();

  CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 5, 5, 350, 15, hwnd, (HMENU)ID_COMMAND, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwnd, ID_COMMAND, WM_SETFONT, (WPARAM)hFont, 0);
  SendDlgItemMessage(hwnd, 120,        WM_SETFONT, (WPARAM)hFont, 0);
}
