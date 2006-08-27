const char AboutWindow::about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\r\n"
  "Author: byuu\r\n"
  "Project began: October 14th, 2004\r\n"
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
break;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);

//HDC hdcsrc = CreateCompatibleDC(hdc);
//HBITMAP hbm = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(101));
//  SelectObject(hdcsrc, hbm);
//  BitBlt(hdc, 0, 0, 400, 240, hdcsrc, 0, 0, SRCCOPY);
//  DeleteDC(hdcsrc);
//  DeleteObject(hbm);

    SelectObject(hdc, (HGDIOBJ)global::font_about);
    SetBkMode(hdc, TRANSPARENT);
  RECT rc;
    SetTextColor(hdc, RGB(0, 0, 0));
    SetRect(&rc, 27, 28, 375, 215);
    DrawText(hdc, about_text, strlen(about_text), &rc, DT_TOP | DT_LEFT);
    SetRect(&rc, 29, 28, 375, 215);
    DrawText(hdc, about_text, strlen(about_text), &rc, DT_TOP | DT_LEFT);
    SetRect(&rc, 28, 27, 375, 215);
    DrawText(hdc, about_text, strlen(about_text), &rc, DT_TOP | DT_LEFT);
    SetRect(&rc, 28, 29, 375, 215);
    DrawText(hdc, about_text, strlen(about_text), &rc, DT_TOP | DT_LEFT);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetRect(&rc, 28, 28, 375, 215);
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
  AboutText.Create(this, "visible|edge|multiline|readonly", 5, 5, 315, 125, about_text);
  AboutText.SetBackgroundColor(32, 32, 32);
  AboutText.SetTextColor(255, 255, 255);
  AboutText.SetFont(global::font_about);
  Ok.Create(this, "visible", 240, 135, 80, 25, "Ok");
}
