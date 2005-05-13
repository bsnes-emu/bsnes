void Window::resize(int width, int height) {
RECT rc;
int x, y, wx, wy;
  hide();
  SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER);
  GetClientRect(hwnd, &rc);
  x = width  + width  - (rc.right  - rc.left);
  y = height + height - (rc.bottom - rc.top);
  wx = (GetSystemMetrics(SM_CXSCREEN) - x) / 2;
  wy = (GetSystemMetrics(SM_CYSCREEN) - y) / 2;
  SetWindowPos(hwnd, 0, wx, wy, x, y, SWP_NOZORDER);
}

void Window::to_left(HWND _hwnd) {
RECT rc;
int offset = 0;
  if(_hwnd) {
    GetWindowRect(_hwnd, &rc);
    offset = rc.right;
  }
  GetWindowRect(hwnd, &rc);
  rc.left = offset;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_center() {
RECT rc;
  GetWindowRect(hwnd, &rc);
  rc.left = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_right() {
RECT rc;
  GetWindowRect(hwnd, &rc);
  rc.left = GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left);
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_top() {
RECT rc;
  GetWindowRect(hwnd, &rc);
  rc.top = 0;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_middle() {
RECT rc;
  GetWindowRect(hwnd, &rc);
  rc.top = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_bottom(HWND _hwnd) {
RECT rc;
int offset = 0;
  if(_hwnd) {
    GetWindowRect(_hwnd, &rc);
    offset = rc.bottom - rc.top;
  }
  GetWindowRect(hwnd, &rc);
  rc.top = GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top) - offset;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::hide() {
  if(visible == true) {
    ShowWindow(hwnd, SW_HIDE);
    visible = false;
  }
}

void Window::show() {
  if(visible == false) {
    ShowWindow(hwnd, SW_NORMAL);
    visible = true;
  }
}

void Window::show_menu() {
  SetMenu(hwnd, hmenu);
  menu_visible = true;
}

void Window::hide_menu() {
  SetMenu(hwnd, 0);
  menu_visible = false;
}

Window::Window() {
  visible = false;
  menu_visible = false;
}
