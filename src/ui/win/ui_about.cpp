const char AboutWindow::about_text[4096] = ""
  "          bsnes -- version " BSNES_VERSION "\r\n"
  "          Author: byuu\r\n"
  "          Project began: October 14th, 2004\r\n"
  "\r\n\r\n"
  "Contributors:\r\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\r\n"
  "   Overload, Richard Bannister, TRAC, zones";

bool AboutWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    Hide();
    return true;
  } break;

  case EVENT_DRAW: {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);

  HDC hdcsrc = CreateCompatibleDC(hdc);
  HICON hicon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(100));
    DrawIcon(hdc, 5, 10, hicon);
    DeleteObject(hicon);

    SelectObject(hdc, (HGDIOBJ)global::font_about);
    SetBkMode(hdc, TRANSPARENT);
  RECT rc;
    SetTextColor(hdc, RGB(0, 0, 0));
    SetRect(&rc, 5, 5, 315, 145);
    DrawText(hdc, about_text, strlen(about_text), &rc, DT_TOP | DT_LEFT);

    EndPaint(hwnd, &ps);
  } break;

  case EVENT_CLICKED: {
    if(info.control == &Ok) {
      Hide();
    }
  } break;

  }

  return false;
}

void AboutWindow::Setup() {
  Ok.Create(this, "visible", 220, 125, 100, 25, "Ok");
}
