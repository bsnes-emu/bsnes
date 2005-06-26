void Window::resize(int width, int height) {
int style;
  window.width  = width;
  window.height = height;

  style = GetWindowLong(hwnd, GWL_STYLE);
  if(style & WS_CAPTION) {
    width  += GetSystemMetrics(SM_CXFIXEDFRAME) << 1;
    height += GetSystemMetrics(SM_CYFIXEDFRAME) << 1;
    height += GetSystemMetrics(SM_CYCAPTION);
  }

  if(width == GetSystemMetrics(SM_CXSCREEN) && height == GetSystemMetrics(SM_CYSCREEN)) {
    fullscreen = true;
  } else {
    fullscreen = false;
  }

  if(fullscreen == false) {
    if(GetMenu(hwnd) != 0) {
      height += GetSystemMetrics(SM_CYMENU);
    }
    SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
  } else {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, 0);
  }
}

void Window::center() {
RECT rc;
POINT p;
  if(fullscreen == true) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, window.width, window.height, 0);
    return;
  }
  GetWindowRect(hwnd, &rc);
  p.x = p.y = 0;
  ClientToScreen(hwnd, &p);
  OffsetRect(&rc, p.x, p.y);
int sw = GetSystemMetrics(SM_CXSCREEN),
    sh = GetSystemMetrics(SM_CYSCREEN);
int x = (sw - (rc.right - rc.left)) >> 1,
    y = (sh - (rc.bottom - rc.top)) >> 1;
  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::show_menu() {
RECT rc;
int width, height;
  if(GetMenu(hwnd) != NULL)return;

  SetMenu(hwnd, hmenu);
  if(cursor_visible == false) {
    ShowCursor(TRUE);
    cursor_visible = true;
  }
  if(fullscreen == true) {
    return;
  }

  resize(window.width, window.height);
}

void Window::hide_menu() {
RECT rc;
int width, height;
  if(GetMenu(hwnd) == NULL)return;

  SetMenu(hwnd, NULL);
  if(fullscreen == true) {
    if(cursor_visible == true) {
      ShowCursor(FALSE);
      cursor_visible = false;
    }
    return;
  }

  resize(window.width, window.height);
}

void Window::show() {
int style;
  style = GetWindowLong(hwnd, GWL_STYLE);
  if(style & WS_VISIBLE)return;
  ShowWindow(hwnd, SW_NORMAL);
}

void Window::hide() {
int style;
  style = GetWindowLong(hwnd, GWL_STYLE);
  if(!(style & WS_VISIBLE))return;
  ShowWindow(hwnd, SW_HIDE);
}

void Window::to_left(HWND _hwnd) {
RECT rc;
int offset = 0;
  if(_hwnd) {
    GetWindowRect(_hwnd, &rc);
    offset = rc.right;
  } else {
    offset = wa_offset.left;
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
  rc.left -= wa_offset.right;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::to_top() {
RECT rc;
  GetWindowRect(hwnd, &rc);
  rc.top = wa_offset.top;
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
  rc.top -= wa_offset.bottom;
  SetWindowPos(hwnd, 0, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

Window::Window() {
  fullscreen     = false;
  visible        = false;
  cursor_visible = true;
  hmenu          = NULL;

  SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
int sw = GetSystemMetrics(SM_CXSCREEN),
    sh = GetSystemMetrics(SM_CYSCREEN);

  wa_offset.left   = workarea.left;
  wa_offset.top    = workarea.top;
  wa_offset.right  = sw - workarea.right;
  wa_offset.bottom = sh - workarea.bottom;
}
