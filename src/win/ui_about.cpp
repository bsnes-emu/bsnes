long AboutWindow::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case ABOUT_OK:
      w_about->hide();
      break;
    }
    break;
  case WM_CLOSE:
    w_about->hide();
    return TRUE;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

long __stdcall wndproc_aboutwindow(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return w_about->wndproc(hwnd, msg, wparam, lparam);
}

void AboutWindow::create() {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc_aboutwindow;
  wc.lpszClassName = "bsnes_aboutwindow";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindow("bsnes_aboutwindow", "About bsnes...",
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 275, 55, 0, 0, wc.hInstance, 0);
  resize(275, 55);
  center();

  CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE,
    5, 5, 200, 45, hwnd, (HMENU)ABOUT_STATIC, wc.hInstance, 0);   
  SetDlgItemText(hwnd, ABOUT_STATIC, ""
    " bsnes -- Version " BSNES_VERSION "\r\n"
    " Written by: byuu\r\n"
    " http://byuu.org/"
  );
  CreateWindow("BUTTON", "OK", WS_CHILD | WS_VISIBLE,
    210, 5, 60, 20, hwnd, (HMENU)ABOUT_OK, wc.hInstance, 0);

  SendDlgItemMessage(hwnd, ABOUT_STATIC, WM_SETFONT, (WPARAM)hFont, TRUE);
  SendDlgItemMessage(hwnd, ABOUT_OK,     WM_SETFONT, (WPARAM)hFont, TRUE);
}
