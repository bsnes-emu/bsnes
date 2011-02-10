static LRESULT CALLBACK HexEditor_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  HexEditor &self = *(HexEditor*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  switch(msg) {
    case WM_CHAR: {
      if(self.keyPress(wparam)) return 0;
    }
  }

  return self.hexEditor->windowProc(hwnd, msg, wparam, lparam);
}

void HexEditor::setSize(unsigned size) {
  hexEditor->size = size;
  update();
}

void HexEditor::setOffset(unsigned offset) {
  hexEditor->offset = offset;
  update();
}

void HexEditor::setColumns(unsigned columns) {
  hexEditor->columns = columns;
  update();
}

void HexEditor::setRows(unsigned rows) {
  hexEditor->rows = rows;
  update();
}

void HexEditor::update() {
  if(!onRead) {
    SetWindowText(widget->window, L"");
    return;
  }

  //modifying text resets cursor position to zero, save position so we can restore it later
  unsigned cursorPosition = Edit_GetSel(widget->window);

  string output;
  unsigned offset = hexEditor->offset;
  for(unsigned row = 0; row < hexEditor->rows; row++) {
    output.append(hex<8>(offset));
    output.append("  ");

    string hexdata;
    string ansidata = " ";
    for(unsigned column = 0; column < hexEditor->columns; column++) {
      if(offset < hexEditor->size) {
        uint8_t data = onRead(offset++);
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
    if(offset >= hexEditor->size) break;
    if(row != hexEditor->rows - 1) output.append("\r\n");
  }

  SetWindowText(widget->window, utf16_t(output));
  Edit_SetSel(widget->window, LOWORD(cursorPosition), HIWORD(cursorPosition));
}

bool HexEditor::keyPress(unsigned scancode) {
  if(!onRead || !onWrite) return false;

  unsigned position = LOWORD(Edit_GetSel(widget->window));
  unsigned lineWidth = 10 + (hexEditor->columns * 3) + 1 + (hexEditor->columns) + 2;
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
      if(cursorX < hexEditor->columns) {
        //not in ANSI region
        unsigned offset = hexEditor->offset + (cursorY * hexEditor->columns + cursorX);

        if(offset >= hexEditor->size) return false;  //do not edit past end of file
        uint8_t data = onRead(offset);

        //write modified value
        if(cursorNibble == 1) {
          data = (data & 0xf0) | (scancode << 0);
        } else {
          data = (data & 0x0f) | (scancode << 4);
        }
        onWrite(offset, data);

        //auto-advance cursor to next nibble/byte
        position++;
        if(cursorNibble && cursorX != hexEditor->columns - 1) position++;
        Edit_SetSel(widget->window, position, position);

        //refresh output to reflect modified data
        update();
      }
    }
  }

  return true;
}

HexEditor::HexEditor() {
  hexEditor = new HexEditor::Data;
  hexEditor->windowProc = 0;
  hexEditor->size = 0;
  hexEditor->offset = 0;
  hexEditor->columns = 16;
  hexEditor->rows = 16;

  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);

  hexEditor->windowProc = (LRESULT CALLBACK (*)(HWND, UINT, LPARAM, WPARAM))GetWindowLongPtr(widget->window, GWLP_WNDPROC);
  SetWindowLongPtr(widget->window, GWLP_WNDPROC, (LONG_PTR)HexEditor_WindowProc);
}

HexEditor::~HexEditor() {
  delete[] hexEditor;
}
