static LRESULT CALLBACK HexEdit_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  HexEdit &hexEdit = *(HexEdit*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(msg == WM_CHAR) {
    if(hexEdit.p.keyPress(wparam)) return 0;
  }
  return hexEdit.p.windowProc(hwnd, msg, wparam, lparam);
}

void pHexEdit::setColumns(unsigned columns) {
  update();
}

void pHexEdit::setLength(unsigned length) {
  update();
}

void pHexEdit::setOffset(unsigned offset) {
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
        char buffer[2] = { data >= 0x20 && data <= 0x7e ? (char)data : '.', 0 };
        ansidata.append(buffer);
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
  setParent(Window::None);
}

bool pHexEdit::keyPress(unsigned scancode) {
  if(!hexEdit.onRead) return false;

  unsigned position = LOWORD(Edit_GetSel(hwnd));
  unsigned lineWidth = 10 + (hexEdit.state.columns * 3) + 1 + hexEdit.state.columns + 2;
  unsigned cursorY = position / lineWidth;
  unsigned cursorX = position % lineWidth;

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

void pHexEdit::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&hexEdit);
  setDefaultFont();
  update();

  windowProc = (LRESULT CALLBACK (*)(HWND, UINT, LPARAM, WPARAM))GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)HexEdit_windowProc);
}
