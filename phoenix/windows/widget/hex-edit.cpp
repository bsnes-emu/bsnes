namespace phoenix {

static LRESULT CALLBACK HexEdit_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  HexEdit& hexEdit = *(HexEdit*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  switch(msg) {
  case WM_KEYDOWN:
    if(hexEdit.p.keyPress(wparam)) return 0;
    break;

  case WM_MOUSEWHEEL: {
    signed offset = -((int16_t)HIWORD(wparam) / WHEEL_DELTA);
    hexEdit.p.scrollTo(hexEdit.p.scrollPosition() + offset);
    return true;
  }

  case WM_SIZE: {
    RECT rc;
    GetClientRect(hexEdit.p.hwnd, &rc);
    SetWindowPos(hexEdit.p.scrollBar, HWND_TOP, rc.right - 18, 0, 18, rc.bottom, SWP_SHOWWINDOW);
    break;
  }

  case WM_VSCROLL: {
    SCROLLINFO info;
    memset(&info, 0, sizeof(SCROLLINFO));
    info.cbSize = sizeof(SCROLLINFO);
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

    hexEdit.p.scrollTo(info.nPos);
    return TRUE;
  }
  }

  return hexEdit.p.windowProc(hwnd, msg, wparam, lparam);
}

void pHexEdit::setBackgroundColor(Color color) {
}

void pHexEdit::setColumns(unsigned columns) {
  update();
}

void pHexEdit::setForegroundColor(Color color) {
}

void pHexEdit::setLength(unsigned length) {
  SetScrollRange(scrollBar, SB_CTL, 0, rowsScrollable(), TRUE);
  EnableWindow(scrollBar, rowsScrollable() > 0);
  update();
}

void pHexEdit::setOffset(unsigned offset) {
  SetScrollPos(scrollBar, SB_CTL, offset / hexEdit.state.columns, TRUE);
  update();
}

void pHexEdit::setRows(unsigned rows) {
  update();
}

void pHexEdit::update() {
  if(!hexEdit.onRead) {
    SetWindowText(hwnd, L"");
    return;
  }

  unsigned cursorPosition = Edit_GetSel(hwnd);

  string output;
  unsigned offset = hexEdit.state.offset;
  for(unsigned row = 0; row < hexEdit.state.rows; row++) {
    output.append(hex<8>(offset));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(unsigned column = 0; column < hexEdit.state.columns; column++) {
      if(offset < hexEdit.state.length) {
        uint8_t data = hexEdit.onRead(offset++);
        hexdata.append(hex<2>(data));
        hexdata.append(" ");
        ansidata.append(data >= 0x20 && data <= 0x7e ? (char)data : '.');
      } else {
        hexdata.append("   ");
        ansidata.append(" ");
      }
    }

    output.append(hexdata);
    output.append(ansidata);
    if(offset >= hexEdit.state.length) break;
    if(row != hexEdit.state.rows - 1) output.append("\r\n");
  }

  SetWindowText(hwnd, utf16_t(output));
  Edit_SetSel(hwnd, LOWORD(cursorPosition), HIWORD(cursorPosition));
}

void pHexEdit::constructor() {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&hexEdit);

  scrollBar = CreateWindowEx(
    0, L"SCROLLBAR", L"",
    WS_VISIBLE | WS_CHILD | SBS_VERT,
    0, 0, 0, 0, hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(scrollBar, GWLP_USERDATA, (LONG_PTR)&hexEdit);

  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)HexEdit_windowProc);

  setDefaultFont();
  setLength(hexEdit.state.length);
  setOffset(hexEdit.state.offset);
  update();
  PostMessage(hwnd, EM_SETSEL, 10, 10);

  synchronize();
}

void pHexEdit::destructor() {
  DestroyWindow(hwnd);
}

void pHexEdit::orphan() {
  destructor();
  constructor();
}

bool pHexEdit::keyPress(unsigned scancode) {
  if(!hexEdit.onRead) return false;

  signed position = LOWORD(Edit_GetSel(hwnd));
  signed lineWidth = 10 + (hexEdit.state.columns * 3) + 1 + hexEdit.state.columns + 2;
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
    if(cursorY < hexEdit.state.rows - 1) return false;
    scrollTo(scrollPosition() + 1);
    return true;
  }

  if(scancode == VK_PRIOR) {
    scrollTo(scrollPosition() - hexEdit.state.rows);
    return true;
  }

  if(scancode == VK_NEXT) {
    scrollTo(scrollPosition() + hexEdit.state.rows);
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
      if(cursorX < hexEdit.state.columns) {
        //not in ANSI region
        unsigned offset = hexEdit.state.offset + (cursorY * hexEdit.state.columns + cursorX);

        if(offset >= hexEdit.state.length) return false;  //do not edit past end of data
        uint8_t data = hexEdit.onRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        if(hexEdit.onWrite) hexEdit.onWrite(offset, data);

        //auto-advance cursor to next nibble or byte
        position++;
        if(cursorNibble && cursorX != hexEdit.state.columns - 1) position++;
        Edit_SetSel(hwnd, position, position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return true;
}

signed pHexEdit::rows() {
  return (max(1u, hexEdit.state.length) + hexEdit.state.columns - 1) / hexEdit.state.columns;
}

signed pHexEdit::rowsScrollable() {
  return max(0u, rows() - hexEdit.state.rows);
}

signed pHexEdit::scrollPosition() {
  return hexEdit.state.offset / hexEdit.state.columns;
}

void pHexEdit::scrollTo(signed position) {
  if(position > rowsScrollable()) position = rowsScrollable();
  if(position < 0) position = 0;
  if(position == scrollPosition()) return;
  hexEdit.setOffset(position * hexEdit.state.columns);
}

}
