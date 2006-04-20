long Window::WndProc(UINT msg, WPARAM wparam, LPARAM lparam) {
EventInfo info;
  info.window     = this;
  info.window_id  = id;
  info.control    = 0;
  info.control_id = 0;

static bool  rbn_down = false;
static int32 coord_x, coord_y;

  if(state.dragmove == true)
  switch(msg) {

  case WM_MOUSEMOVE: {
    if(rbn_down == true) {
    POINT p;
      GetCursorPos(&p);
    RECT rc;
      GetWindowRect(hwnd, &rc);
      SetWindowPos(hwnd, 0,
        rc.left - (coord_x - p.x), rc.top - (coord_y - p.y),
        0, 0, SWP_NOZORDER | SWP_NOSIZE);
      coord_x = p.x;
      coord_y = p.y;
    }
  } break;

  case WM_RBUTTONDOWN: {
  POINT p;
    GetCursorPos(&p);
    coord_x  = p.x;
    coord_y  = p.y;
    rbn_down = true;
    SetCapture(hwnd);
  } break;

  case WM_CAPTURECHANGED:
  case WM_RBUTTONUP: {
    rbn_down = false;
    ReleaseCapture();
  } break;

  }

bool result = false;
  switch(msg) {

  case WM_CLOSE: {
    info.event_id = EVENT_CLOSE;
    result = Event(info);
  } break;

  case WM_DESTROY: {
    info.event_id = EVENT_DESTROY;
    result = Event(info);
  } break;

  case WM_PAINT: {
    info.event_id = EVENT_DRAW;
    result = Event(info);
  } break;

  case WM_COMMAND: {
    for(int i = 0; i < menu_count; i++) {
      if(LOWORD(wparam) == menu_list[i]) {
        info.event_id   = EVENT_MENUCLICKED;
        info.control_id = menu_list[i];
        result = Event(info);
        break;
      }
    }

    for(int i = 0; i < control_count; i++) {
      if(LOWORD(wparam) == list[i]->id) {
        if((list[i]->type == Control::COMBOBOX && HIWORD(wparam) == CBN_SELCHANGE) ||
           (list[i]->type == Control::LISTBOX  && HIWORD(wparam) == LBN_SELCHANGE)) {
          info.event_id   = EVENT_CHANGED;
          info.control    = list[i];
          info.control_id = list[i]->id;
          result = Event(info);
          break;
        } else {
          info.event_id   = EVENT_CLICKED;
          info.control    = list[i];
          info.control_id = list[i]->id;
          result = Event(info);
          break;
        }
      }
    }
  } break;

  case WM_HSCROLL: {
    for(int i = 0; i < control_count; i++) {
      if((HWND)lparam == list[i]->hwnd) {
        info.event_id   = EVENT_CHANGED;
        info.control    = list[i];
        info.control_id = list[i]->id;
        result = Event(info);
        break;
      }
    }
  } break;

  case WM_NOTIFY: {
    for(int i = 0; i < control_count; i++) {
      if(LOWORD(wparam) == list[i]->id) {
        if(list[i]->type == Control::LISTVIEW) {
          if(((LPNMHDR)lparam)->code == LVN_ITEMCHANGED) {
            if(SendMessage(list[i]->hwnd, LVM_GETNEXTITEM, -1, LVNI_FOCUSED) != -1) {
              info.event_id   = EVENT_CHANGED;
              info.control    = list[i];
              info.control_id = list[i]->id;
              result = Event(info);
              break;
            }
          } else if(((LPNMHDR)lparam)->code == NM_DBLCLK) {
            info.event_id   = EVENT_DOUBLECLICKED;
            info.control    = list[i];
            info.control_id = list[i]->id;
            result = Event(info);
            break;
          }
        }
      }
    }
  } break;

  case WM_ENTERMENULOOP: {
    info.event_id = EVENT_MENUENTER;
    result = Event(info);
  } break;

  case WM_EXITMENULOOP: {
    info.event_id = EVENT_MENUEXIT;
    result = Event(info);
  } break;

  case WM_CTLCOLORBTN:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORSTATIC: {
    for(int i = 0; i < control_count; i++) {
      if((HWND)lparam != list[i]->hwnd)continue;
    HDC hdc = (HDC)wparam;
      if(list[i]->state.use_textcolor == true) {
        SetTextColor(hdc,
          RGB((list[i]->state.textcolor >> 16) & 255,
              (list[i]->state.textcolor >>  8) & 255,
              (list[i]->state.textcolor >>  0) & 255));
      }
      if(list[i]->state.use_backcolor == true) {
        SetBkColor(hdc,
          RGB((list[i]->state.backcolor >> 16) & 255,
              (list[i]->state.backcolor >>  8) & 255,
              (list[i]->state.backcolor >>  0) & 255));
      }
      if(list[i]->backbrush)return (long)list[i]->backbrush;
    }
  } break;

  }

//did event handler process message?
  return (result == true) ? (long)true : DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::Resize(uint width, uint height, bool center) {
  state.width  = width;
  state.height = height;

//actual windows are resized so that their client size is exactly as requested.
//windows inside of windows are resized as controls are, thusly the window style
//takes away from their total client area size.
  if(!parent) {
    SetWindowPos(hwnd_resize, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
  RECT rc;
    GetClientRect(hwnd_resize, &rc);
    width  += width  - (rc.right  - rc.left);
    height += height - (rc.bottom - rc.top);
  uint x = (GetSystemMetrics(SM_CXSCREEN) - width)  >> 1;
  uint y = (GetSystemMetrics(SM_CYSCREEN) - height) >> 1;
    SetWindowPos(hwnd, 0, x, y, width, height, SWP_NOZORDER | ((center == false) ? SWP_NOMOVE : 0));
  } else {
    SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
  }

  if(Visible())Focus();

  state.window_width  = width;
  state.window_height = height;
}

void Window::Resize() { Resize(state.width, state.height); }

void Window::Move(uint x, uint y) {
  state.x = x;
  state.y = y;

  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::MoveToTop() {
RECT rc, workarea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  GetWindowRect(hwnd, &rc);
uint x = rc.left;
uint y = workarea.top;
  Move(x, y);
}

void Window::MoveToBottom() {
RECT rc, workarea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  GetWindowRect(hwnd, &rc);
uint x = rc.left;
uint y = workarea.bottom - (rc.bottom - rc.top);
  Move(x, y);
}

void Window::MoveToLeft() {
RECT rc, workarea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  GetWindowRect(hwnd, &rc);
uint x = workarea.left;
uint y = rc.top;
  Move(x, y);
}

void Window::MoveToRight() {
RECT rc, workarea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  GetWindowRect(hwnd, &rc);
uint x = workarea.right - (rc.right - rc.left);
uint y = rc.top;
  Move(x, y);
}

void Window::Center() {
RECT workarea, rc;
//SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
  GetWindowRect(hwnd, &rc);
uint x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1;
uint y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1;
  Move(x, y);
}

void Window::SetStyle(const char *style) {
  state.ws = WS_SYSMENU;
  state.es = 0;

  state.dragmove = false;

  if(parent)state.ws |= WS_CHILD;

string part;
  ParseStyleParam(style, part);
  for(int i = 0; i < count(part); i++) {
    if(strmatch(part[i], "visible")) state.ws |= WS_VISIBLE;
    if(strmatch(part[i], "popup"))   state.ws |= WS_POPUP;
    if(strmatch(part[i], "border"))  state.ws |= WS_BORDER;
    if(strmatch(part[i], "frame"))   state.ws |= WS_DLGFRAME;
    if(strmatch(part[i], "titlebar"))state.ws |= WS_CAPTION;
    if(strmatch(part[i], "minimize"))state.ws |= WS_MINIMIZEBOX;
    if(strmatch(part[i], "maximize"))state.ws |= WS_MAXIMIZEBOX;

    if(strmatch(part[i], "topmost")) state.es |= WS_EX_TOPMOST;
    if(strmatch(part[i], "layered")) state.es |= WS_EX_LAYERED;
    if(strmatch(part[i], "sunken"))  state.es |= WS_EX_STATICEDGE;
    if(strmatch(part[i], "edge"))    state.es |= WS_EX_CLIENTEDGE;

    if(strmatch(part[i], "dragmove"))state.dragmove = true;
  }

  if(!hwnd)return;

//never show resize window
  SetWindowLong(hwnd_resize, GWL_STYLE,   state.ws & ~WS_VISIBLE);
  SetWindowLong(hwnd_resize, GWL_EXSTYLE, state.es);
  SetWindowPos (hwnd_resize, (state.es & WS_EX_TOPMOST) ? HWND_TOPMOST : HWND_NOTOPMOST,
    0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

  SetWindowLong(hwnd, GWL_STYLE,   state.ws);
  SetWindowLong(hwnd, GWL_EXSTYLE, state.es);
  SetWindowPos (hwnd, (state.es & WS_EX_TOPMOST) ? HWND_TOPMOST : HWND_NOTOPMOST,
    0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

  Resize();
}

void Window::SetAlphaLevel(uint level) {
  level &= 255;
  if(level == 255) {
  //no reason to use layered attribute for completely opaque window
    state.es &= ~WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, state.es);
    SetLayeredWindowAttributes(hwnd, 0, 0, 0);
  } else {
    state.es |=  WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, state.es);
    SetLayeredWindowAttributes(hwnd, 0, level, LWA_ALPHA);
  }
}

void Window::SetBackgroundColor(uint r, uint g, uint b) {
  backbrush = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
}

void Window::SetIcon(uint resource_id) {
  state.use_icon = true;
  state.icon_id  = resource_id;
}

void Window::Show(bool do_show) {
  if(do_show == true) {
    ShowWindow(hwnd, SW_NORMAL);
    state.ws |=  WS_VISIBLE;
  } else {
    ShowWindow(hwnd, SW_HIDE);
    state.ws &= ~WS_VISIBLE;
  }
}

bool Window::Visible() {
  return !!(GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE);
}

void Window::Show()  { Show(true); }
void Window::Hide()  { Show(false); }
void Window::Focus() { SetFocus(hwnd); }

bool Window::Create(Window *parent_window, const char *classname, const char *style, uint x, uint y, uint width, uint height, const char *title) {
  parent = parent_window;
  id     = WINDOWID_INDEX + WindowList.window_count;

  strcpy(state.classname, classname);
  strcpy(state.title, title);
  strcpy(state.style, style);

  state.x      = x;
  state.y      = y;
  state.width  = width;
  state.height = height;
  SetStyle(style);

WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = backbrush;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = (state.use_icon) ? LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(state.icon_id)) : LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = CoreWindowProc;
  wc.lpszClassName = strptr(state.classname);
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindowEx(state.es, strptr(state.classname), strptr(state.title), state.ws,
    state.x, state.y, state.width, state.height, parent ? parent->hwnd : 0, 0, wc.hInstance, 0);
  hwnd_resize = CreateWindowEx(state.es, "resize_class", "", state.ws & ~WS_VISIBLE,
    state.x, state.y, state.width, state.height, parent ? parent->hwnd : 0, 0, wc.hInstance, 0);
  if(!hwnd || !hwnd_resize)return false;

  Resize();

  WindowList.Add(this);
  return true;
}

bool Window::Add(Control &control) {
  if(control_count >= CONTROL_LIMIT)return false;
  list[control_count++] = &control;
  return true;
}

/*
 * menu functions
 */

void Window::CreateMenu() {
  menu = ::CreateMenu();
  submenu_pos = 0;
  for(int i = 0; i < 16; i++)submenu[i].handle = 0;
}

void Window::AddMenuGroup(const char *text) {
  submenu[submenu_pos].handle = CreatePopupMenu();
  strcpy(submenu[submenu_pos].text, text);
  submenu_pos++;
}

void Window::EndMenuGroup() {
  submenu_pos--;
  AppendMenu((submenu_pos == 0) ? menu : submenu[submenu_pos - 1].handle, MF_STRING | MF_POPUP,
    (uint)submenu[submenu_pos].handle, submenu[submenu_pos].text);
}

void Window::AddMenuItem(uint id, const char *text) {
  if(menu_count >= CONTROL_LIMIT)return;

  menu_list[menu_count++] = id;
  AppendMenu((submenu_pos == 0) ? menu : submenu[submenu_pos - 1].handle, MF_STRING,
    (uint)id, text);
}

void Window::AddMenuSeparator() {
  AppendMenu((submenu_pos == 0) ? menu : submenu[submenu_pos - 1].handle, MF_SEPARATOR, 0, "");
}

void Window::CheckMenuItem(uint id, bool do_check) {
  ::CheckMenuItem(menu, id, (do_check == true) ? MF_CHECKED : MF_UNCHECKED);
}

bool Window::MenuItemChecked(uint id) {
  return ::CheckMenuItem(menu, id, 0);
}

void Window::CheckMenuItem(uint id) { CheckMenuItem(id, true); }
void Window::UncheckMenuItem(uint id) { CheckMenuItem(id, false); }

void Window::ShowMenu(bool state) {
  SetMenu(hwnd_resize, (state == true) ? menu : 0);
  SetMenu(hwnd, (state == true) ? menu : 0);
  Resize();
}

bool Window::MenuVisible() { return GetMenu(hwnd) != 0; }
void Window::ShowMenu() { ShowMenu(true); }
void Window::HideMenu() { ShowMenu(false); }

void Window::SetEventCallback(EventCallback ecb) {
  event_callback = ecb;
}

bool Window::Event(EventInfo &info) {
  return (event_callback) ? event_callback(info) : false;
}

Window::Window() {
  hwnd           = 0;
  hwnd_resize    = 0;
  backbrush      = (HBRUSH)COLOR_WINDOW;
  control_count  = 0;
  event_callback = 0;

  state.use_icon = false;

  list = (Control**)malloc((CONTROL_LIMIT + 1) * sizeof(Control*));
  memset(list, 0, (CONTROL_LIMIT + 1) * sizeof(Control*));
  menu_list = (uint*)malloc((CONTROL_LIMIT + 1) * sizeof(uint));
  memset(menu_list, 0, (CONTROL_LIMIT + 1) * sizeof(uint));

  for(int i = 0; i < 16; i++) {
    submenu[i].text  = (char*)malloc(256);
    strcpy(submenu[i].text, "");
  }
}
