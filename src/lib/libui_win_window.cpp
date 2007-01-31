namespace libui {

long __stdcall libui_wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam) {
Window *window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if(window) { return window->wndproc(hwnd, msg, wparam, lparam); }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

long Window::wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {

  case WM_CLOSE: {
    if(close() == false) { return TRUE; }
  } break;

  case WM_KEYDOWN: {
    keydown((int)wparam);
  } break;

  case WM_KEYUP: {
    keyup((int)wparam);
  } break;

  case WM_COMMAND: {
  uint i = LOWORD(wparam);
    if(!info.control[i])break;
  Control &control = *info.control[i];
    if(control.id != i)break; //this should never happen

    switch(control.type) {

    case Control::MenuCheckItem: { //need to simulate auto check event
    MenuCheckItem &item = static_cast<MenuCheckItem&>(control);
      item.check(!item.checked()); //toggle checked status
    } break;

    case Control::MenuRadioItem: { //need to simulate auto radio check event
      static_cast<MenuRadioItem&>(control).check();
    } break;

    case Control::Radiobox: {
      static_cast<Radiobox&>(control).check();
    } break;

    }

    clicked(&control);
  } break;

  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowHandle Window::handle() {
  return (WindowHandle)info.hwnd;
}

void Window::attach(Control &control, uint x, uint y, bool attach_to_window) {
  info.control[info.control_index] = &control;
  control.id    = info.control_index++;
  control.owner = this;
  SetWindowLong(control.hwnd, GWL_ID, control.id);
  SetWindowPos(control.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  if(attach_to_window == true) { SetParent(control.hwnd, info.hwnd); }

  if(control.type == Control::Container) {
    SetWindowLongPtr(control.hwnd, GWLP_USERDATA, (LONG_PTR)GetWindowLongPtr(info.hwnd, GWLP_USERDATA));
  }
}

void Window::focus() {
  show();
  SetFocus(info.hwnd);
}

void Window::move(Control &control, uint x, uint y) {
  SetWindowPos(control.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::create(const char *style, uint width, uint height, const char *caption) {
stringarray part;
  split(part, "|", style);
  for(int i = 0; i < count(part); i++) {
    if(part[i] == "center") { info.center = true; }
    if(part[i] == "menu") { info.has_menu = true; }
  }

char classname[4096];
  sprintf(classname, "libui_class_%d", window_count++);
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = libui_wndproc;
  wc.lpszClassName = classname;
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

  info.hwnd = CreateWindowEx(0, classname, caption ? caption : "",
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width, height, 0, 0, GetModuleHandle(0), 0);
  info.hwnd_resize = CreateWindowEx(0, "libui_class", "",
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, rc.left, rc.top, width, height, 0, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(info.hwnd, GWLP_USERDATA, (LONG_PTR)this);

  if(info.has_menu == true) {
    info.control[info.control_index] = &info.menu_owner;

    info.menu_owner.id     = info.control_index++;
    info.menu_owner.owner  = this;
    info.menu_owner.type   = Control::MenuGroup;
    info.menu_owner.master = true;
    info.menu_owner.group  = CreateMenu();

    info.menu_group[++info.menu_group_index] = &info.menu_owner;
  }

  resize(width, height);
}

void Window::move(uint x, uint y) {
RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  if(x < rc.left)x = rc.left;
  if(y < rc.top )y = rc.top;
  SetWindowPos(info.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::resize(uint width, uint height) {
  info.width  = width;
  info.height = height;

  SetWindowPos(info.hwnd_resize, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
RECT rc;
  GetClientRect(info.hwnd_resize, &rc);
  width  += width  - (rc.right  - rc.left);
  height += height - (rc.bottom - rc.top);
uint x = (GetSystemMetrics(SM_CXSCREEN) - width)  >> 1;
uint y = (GetSystemMetrics(SM_CYSCREEN) - height) >> 1;
  SetWindowPos(info.hwnd, 0, x, y, width, height, SWP_NOZORDER | (info.center == true ? 0 : SWP_NOMOVE));
}

void Window::show() {
  ShowWindow(info.hwnd, SW_NORMAL);
}

void Window::hide() {
  ShowWindow(info.hwnd, SW_HIDE);
}

//

void Window::set_background_color(uint8 r, uint8 g, uint8 b) {
HBRUSH old_brush = info.background;
  info.background = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
  SetClassLong(info.hwnd, GCL_HBRBACKGROUND, (LONG)info.background);
  InvalidateRect(info.hwnd, 0, TRUE);
  if(old_brush) { DeleteObject((HGDIOBJ)old_brush); }
}

//

Window::Window() {
  info.background       = 0;
  info.center           = false;
  info.control_index    = 1;
  info.has_menu         = false;
  info.menu_group_index = 1;
}

};
