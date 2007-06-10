namespace libui {

long __stdcall libui_wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam) {
Window *window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if(window) { return window->wndproc(hwnd, msg, wparam, lparam); }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

long Window::wndproc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {

  case WM_CLOSE: {
    if(message(Message::Close) == false) { return TRUE; }
  } break;

  case WM_ENTERMENULOOP: {
    message(Message::Block);
  }

  case WM_PAINT: {
  uint16 i = LOWORD(wparam);
    if(!i || !info.control[i])break;
  Control &control = *info.control[i];
    if(control.id != i)break; //this should never happen

    switch(control.type) {

    case ControlType::Canvas: {
      ((Canvas&)control).redraw();
    } break;

    }
  } break;

  case WM_KEYDOWN: {
    message(Message::KeyDown, libui::translate_key(wparam));
  } break;

  case WM_KEYUP: {
    message(Message::KeyUp,   libui::translate_key(wparam));
  } break;

  case WM_COMMAND: {
  uint16 i = LOWORD(wparam);
    if(!i || !info.control[i])break;
  Control &control = *info.control[i];
    if(control.id != i)break; //this should never happen

    switch(control.type) {

  //emit Message::Clicked message indirectly, through control.check()
    case ControlType::MenuCheckItem: {
      ((MenuCheckItem&)control).check(!((MenuCheckItem&)control).checked()); //toggle checked status
    } break;

    case ControlType::MenuRadioItem: {
      ((MenuRadioItem&)control).check();
    } break;

    case ControlType::Checkbox: {
      ((Checkbox&)control).check(!((Checkbox&)control).checked()); //toggle checked status
    } break;

    case ControlType::Radiobox: {
      ((Radiobox&)control).check();
    } break;

  //emit Message::Clicked message directly
    case ControlType::MenuItem:
    case ControlType::Button: {
      message(Message::Clicked, (uintptr_t)&control);
    } break;

    }
  } break;

  case WM_HSCROLL:
  case WM_VSCROLL: {
  uint16 i = GetDlgCtrlID((HWND)lparam);
    if(!i || !info.control[i])break;
  Control &control = *info.control[i];
    if(control.id != i)break;

    switch(control.type) {

    case ControlType::Slider: {
      message(Message::Changed, (uintptr_t)&control);
    } break;

    }
  } break;

  case WM_NOTIFY: {
  uint16 i = (uint16)wparam;
    if(!i || !info.control[i])break;
  Control &control = *info.control[i];
    if(control.id != i)break;

    switch(control.type) {

    case ControlType::Listbox: {
    Listbox &listbox = ((Listbox&)control);
      if(((LPNMHDR)lparam)->code == LVN_ITEMCHANGED) {
        if(((LPNMLISTVIEW)lparam)->uChanged & LVIF_STATE) {
          if(ListView_GetItemState(listbox.hwnd, ((LPNMLISTVIEW)lparam)->iItem, LVIS_FOCUSED)) {
            if(ListView_GetItemState(listbox.hwnd, ((LPNMLISTVIEW)lparam)->iItem, LVIS_SELECTED)) {
              message(Message::Changed, (uintptr_t)&control);
            }
          }
        }
      } else if(((LPNMHDR)lparam)->code == LVN_ITEMACTIVATE) {
        message(Message::DoubleClicked, (uintptr_t)&control);
      }
    } break;

    }
  } break;

  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::attach(Control &control) {
  info.control[info.control_index] = &control;
  control.id    = info.control_index++;
  control.owner = this;

//menu items will not have HWND, and do not require GWL_ID to be set
  if(control.hwnd) { SetWindowLong(control.hwnd, GWL_ID, control.id); }
}

void Window::focus() {
  if(!visible()) { show(); }
  SetFocus(info.hwnd);
}

bool Window::focused() {
  return (GetForegroundWindow() == info.hwnd);
}

void Window::move(Control &control, uint x, uint y) {
  SetWindowPos(control.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::create(uint style, uint width, uint height, const char *caption) {
  info.center = bool(style & Center);

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
    WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
    rc.left, rc.top, width, height, 0, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(info.hwnd, GWLP_USERDATA, (LONG_PTR)this);

  resize(width, height);
}

void Window::move(uint x, uint y) {
RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  if(x < rc.left) { x = rc.left; }
  if(y < rc.top ) { y = rc.top;  }
  SetWindowPos(info.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::resize(uint width, uint height) {
  if(info.fullscreen == true) {
    info.width  = GetSystemMetrics(SM_CXSCREEN);
    info.height = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(info.hwnd, 0, 0, 0, info.width, info.height, SWP_NOZORDER);
    return;
  }

  info.width  = width;
  info.height = height;

//set requested window size to hidden window, calculate the difference between requested and actual client
//size area, and then adjust width so that new width, height values will set requested client area size.
//AdjustWindowRect() does not properly calculate the height of multi-line menus, and thusly is not used.
  SetWindowPos(info.hwnd_resize, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
RECT rc;
  GetClientRect(info.hwnd_resize, &rc);
  width  += width  - (rc.right  - rc.left);
  height += height - (rc.bottom - rc.top);

int x = (GetSystemMetrics(SM_CXSCREEN) - int(width))  / 2;
int y = (GetSystemMetrics(SM_CYSCREEN) - int(height)) / 2;

//do not allow window to be placed offscreen, force to top-left if window is larger than screen size
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  if(x < rc.left) { x = rc.left; }
  if(y < rc.top)  { y = rc.top;  }

  SetWindowPos(info.hwnd, 0, x, y, width, height, SWP_NOZORDER | (info.center == true ? 0 : SWP_NOMOVE));
}

void Window::show() {
  ShowWindow(info.hwnd, SW_NORMAL);
  SetFocus(info.hwnd);
}

void Window::hide() {
  ShowWindow(info.hwnd, SW_HIDE);
}

void Window::show(bool state) {
  (state == true) ? show() : hide();
}

bool Window::visible() {
  return GetWindowLong(info.hwnd, GWL_STYLE) & WS_VISIBLE;
}

void Window::fullscreen() {
  if(info.fullscreen)return;
  info.fullscreen = true;
  SetWindowLong(info.hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
  resize(get_screen_width(), get_screen_height());
}

void Window::unfullscreen() {
  if(!info.fullscreen)return;
  info.fullscreen = false;
  SetWindowLong(info.hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE);
  resize(info.width, info.height);
}

void Window::fullscreen(bool state) {
  (state == true) ? fullscreen() : unfullscreen();
}

bool Window::is_fullscreen() {
  return info.fullscreen;
}

//

void Window::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string temp;
  vsprintf(temp, str, args);
  va_end(args);
  SetWindowText(info.hwnd, strptr(temp));
}

void Window::set_background_color(uint8 r, uint8 g, uint8 b) {
HBRUSH old_brush = info.background;
  info.background = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
  SetClassLong(info.hwnd, GCL_HBRBACKGROUND, (LONG)info.background);
  InvalidateRect(info.hwnd, 0, TRUE);
  if(old_brush) { DeleteObject((HGDIOBJ)old_brush); }
}

//

Window::Window() {
  info.fullscreen    = false;
  info.background    = 0;
  info.center        = false;
  info.control_index = 1;
}

//platform-dependent:

HWND Window::handle() {
  return info.hwnd;
}

};
