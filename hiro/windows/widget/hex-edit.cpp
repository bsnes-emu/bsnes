#if defined(Hiro_HexEdit)

namespace hiro {

static auto CALLBACK HexEdit_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto hexEdit = dynamic_cast<mHexEdit*>(object);
  if(!hexEdit) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto self = hexEdit->self();
  if(!self) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_KEYDOWN:
    if(self->keyPress(wparam)) return 0;
    break;

  case WM_MOUSEWHEEL: {
    signed offset = -((int16_t)HIWORD(wparam) / WHEEL_DELTA);
    self->scrollTo(self->scrollPosition() + offset);
    return true;
  }

  case WM_SIZE: {
    RECT rc;
    GetClientRect(self->hwnd, &rc);
    SetWindowPos(self->scrollBar, HWND_TOP, rc.right - 18, 0, 18, rc.bottom, SWP_SHOWWINDOW);
    break;
  }

  case WM_VSCROLL: {
    SCROLLINFO info{sizeof(SCROLLINFO)};
    info.fMask = SIF_ALL;
    GetScrollInfo((HWND)lparam, SB_CTL, &info);
    switch(LOWORD(wparam)) {
    case SB_LEFT: info.nPos = info.nMin; break;
    case SB_RIGHT: info.nPos = info.nMax; break;
    case SB_LINELEFT: info.nPos--; break;
    case SB_LINERIGHT: info.nPos++; break;
    case SB_PAGELEFT: info.nPos -= info.nMax >> 3; break;
    case SB_PAGERIGHT: info.nPos += info.nMax >> 3; break;
    case SB_THUMBTRACK: info.nPos = info.nTrackPos; break;
    }

    info.fMask = SIF_POS;
    SetScrollInfo((HWND)lparam, SB_CTL, &info, TRUE);
    GetScrollInfo((HWND)lparam, SB_CTL, &info);  //get clamped position

    self->scrollTo(info.nPos);
    return true;
  }
  }

  return self->windowProc(hwnd, msg, wparam, lparam);
}

auto pHexEdit::construct() -> void {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);

  scrollBar = CreateWindowEx(
    0, L"SCROLLBAR", L"",
    WS_VISIBLE | WS_CHILD | SBS_VERT,
    0, 0, 0, 0, hwnd, nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(scrollBar, GWLP_USERDATA, (LONG_PTR)&reference);

  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)HexEdit_windowProc);

  pWidget::_setState();
  setBackgroundColor(state().backgroundColor);
  setLength(state().length);
  setOffset(state().offset);
  update();
  PostMessage(hwnd, EM_SETSEL, 10, 10);
}

auto pHexEdit::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pHexEdit::setBackgroundColor(Color color) -> void {
  if(backgroundBrush) DeleteObject(backgroundBrush);
  backgroundBrush = CreateSolidBrush(color ? CreateRGB(color) : GetSysColor(COLOR_WINDOW));
}

auto pHexEdit::setColumns(unsigned columns) -> void {
  update();
}

auto pHexEdit::setForegroundColor(Color color) -> void {
}

auto pHexEdit::setLength(unsigned length) -> void {
  SetScrollRange(scrollBar, SB_CTL, 0, rowsScrollable(), true);
  EnableWindow(scrollBar, rowsScrollable() > 0);
  update();
}

auto pHexEdit::setOffset(unsigned offset) -> void {
  SetScrollPos(scrollBar, SB_CTL, offset / state().columns, true);
  update();
}

auto pHexEdit::setRows(unsigned rows) -> void {
  update();
}

auto pHexEdit::update() -> void {
  if(!state().onRead) {
    SetWindowText(hwnd, L"");
    return;
  }

  unsigned cursorPosition = Edit_GetSel(hwnd);

  string output;
  unsigned offset = state().offset;
  for(auto row : range(state().rows)) {
    output.append(hex(offset, 8L));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(auto column : range(state().columns)) {
      if(offset < state().length) {
        uint8_t data = self().doRead(offset++);
        hexdata.append(hex(data, 2L));
        hexdata.append(" ");
        ansidata.append(data >= 0x20 && data <= 0x7e ? (char)data : '.');
      } else {
        hexdata.append("   ");
        ansidata.append(" ");
      }
    }

    output.append(hexdata);
    output.append(ansidata);
    if(offset >= state().length) break;
    if(row != state().rows - 1) output.append("\r\n");
  }

  SetWindowText(hwnd, utf16_t(output));
  Edit_SetSel(hwnd, LOWORD(cursorPosition), HIWORD(cursorPosition));
}

bool pHexEdit::keyPress(unsigned scancode) {
  if(!state().onRead) return false;

  signed position = LOWORD(Edit_GetSel(hwnd));
  signed lineWidth = 10 + (state().columns * 3) + 1 + state().columns + 2;
  signed cursorY = position / lineWidth;
  signed cursorX = position % lineWidth;

  if(scancode == VK_HOME) {
    signed offset = cursorY * lineWidth + 10;
    Edit_SetSel(hwnd, offset, offset);
    return true;
  }

  if(scancode == VK_END) {
    signed offset = cursorY * lineWidth + 57;
    Edit_SetSel(hwnd, offset, offset);
    return true;
  }

  if(scancode == VK_UP) {
    if(cursorY > 0) return false;
    scrollTo(scrollPosition() - 1);
    return true;
  }

  if(scancode == VK_DOWN) {
    if(cursorY >= rows() - 1) return true;
    if(cursorY < state().rows - 1) return false;
    scrollTo(scrollPosition() + 1);
    return true;
  }

  if(scancode == VK_PRIOR) {
    scrollTo(scrollPosition() - state().rows);
    return true;
  }

  if(scancode == VK_NEXT) {
    scrollTo(scrollPosition() + state().rows);
    return true;
  }

  //convert scancode to hex nibble
       if(scancode >= '0' && scancode <= '9') scancode = scancode - '0';
  else if(scancode >= 'A' && scancode <= 'F') scancode = scancode - 'A' + 10;
  else if(scancode >= 'a' && scancode <= 'f') scancode = scancode - 'a' + 10;
  else return false;

  if(cursorX >= 10) {
    //not on an offset
    cursorX -= 10;
    if((cursorX % 3) != 2) {
      //not on a space
      bool cursorNibble = (cursorX % 3) == 1;  //0 = high, 1 = low
      cursorX /= 3;
      if(cursorX < state().columns) {
        //not in ANSI region
        unsigned offset = state().offset + (cursorY * state().columns + cursorX);

        if(offset >= state().length) return false;  //do not edit past end of data
        uint8_t data = self().doRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        self().doWrite(offset, data);

        //auto-advance cursor to next nibble or byte
        position++;
        if(cursorNibble && cursorX != state().columns - 1) position++;
        Edit_SetSel(hwnd, position, position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return true;
}

signed pHexEdit::rows() {
  return (max(1u, state().length) + state().columns - 1) / state().columns;
}

signed pHexEdit::rowsScrollable() {
  return max(0u, rows() - state().rows);
}

signed pHexEdit::scrollPosition() {
  return state().offset / state().columns;
}

void pHexEdit::scrollTo(signed position) {
  if(position > rowsScrollable()) position = rowsScrollable();
  if(position < 0) position = 0;
  if(position == scrollPosition()) return;
  self().setOffset(position * state().columns);
}

}

#endif
