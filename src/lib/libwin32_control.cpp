/*
 * called after custom control type successfully creates
 * control, used to initialize common control defaults
 */
void Control::PostCreate() {
  SetFont(global::font_default);
  parent->Add(*this);
}

uint Control::GetID()   { return id; }
uint Control::GetType() { return type; }

void Control::SetFont(Font font) {
  SendMessage(hwnd, WM_SETFONT, (WPARAM)font, 0);
}

void Control::Show(bool do_show) {
  if(do_show == true) {
    ShowWindow(hwnd, SW_NORMAL);
    state.ws |=  WS_VISIBLE;
  } else {
    ShowWindow(hwnd, SW_HIDE);
    state.ws &= ~WS_VISIBLE;
  }
}

bool Control::Visible() {
  return !!(GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE);
}

void Control::Show()  { Show(true); }
void Control::Hide()  { Show(false); }
void Control::Focus() { SetFocus(hwnd); }

void Control::Enable(bool do_enable) {
  if(do_enable == true) {
    EnableWindow(hwnd, TRUE);
    state.ws &= ~WS_DISABLED;
  } else {
    EnableWindow(hwnd, FALSE);
    state.ws |=  WS_DISABLED;
  }
}

bool Control::Enabled() {
  return !(GetWindowLong(hwnd, GWL_STYLE) & WS_DISABLED);
}

void Control::Enable() { Enable(true); }
void Control::Disable() { Enable(false); }

void Control::SetBackgroundColor(uint r, uint g, uint b) {
  state.use_backcolor = true;
  state.backcolor = (r << 16) | (g << 8) | b;
  if(backbrush)DeleteObject((HGDIOBJ)backbrush);
  backbrush = CreateSolidBrush(RGB(r, g, b));
  if(hwnd)InvalidateRect(hwnd, 0, TRUE);
}

void Control::SetTextColor(uint r, uint g, uint b) {
  state.use_textcolor = true;
  state.textcolor = (r << 16) | (g << 8) | b;
  if(hwnd)InvalidateRect(hwnd, 0, TRUE);
}

const char *Control::GetText() {
static char t[256 + 1];
  GetWindowText(hwnd, t, 256);
  return t;
}

void Control::GetText(char *text, uint length) {
  GetWindowText(hwnd, text, length ? length : 255);
}

void Control::SetText(const char *text, ...) {
char str[4096];
va_list args;
  va_start(args, text);
  vsprintf(str, text, args);
  va_end(args);
  SetWindowText(hwnd, str);
}

Control::Control() {
  hwnd      = 0;
  backbrush = 0;
  state.use_backcolor = false;
  state.use_textcolor = false;
  state.backcolor     = 0;
  state.textcolor     = 0;
}

Control::~Control() {
  if(backbrush) {
    DeleteObject((HGDIOBJ)backbrush);
    backbrush = 0;
  }
}
