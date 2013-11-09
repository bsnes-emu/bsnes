namespace phoenix {

static LRESULT CALLBACK Console_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Console& console = *(Console*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(msg == WM_CHAR) {
    if(console.p.keyPress(wparam)) return 0;
  }
  return console.p.windowProc(hwnd, msg, wparam, lparam);
}

void pConsole::print(string text) {
}

void pConsole::reset() {
}

void pConsole::constructor() {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | ES_READONLY | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&console);
  setDefaultFont();

  windowProc = (LRESULT CALLBACK (*)(HWND, UINT, LPARAM, WPARAM))GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Console_windowProc);
  synchronize();
}

void pConsole::destructor() {
  DestroyWindow(hwnd);
}

void pConsole::orphan() {
  destructor();
  constructor();
}

bool pConsole::keyPress(unsigned scancode) {
  return false;
}

}
