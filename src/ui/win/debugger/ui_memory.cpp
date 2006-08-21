namespace NSMemoryEditor {
WNDPROC wndproc_old_memory_view = 0;

long __stdcall wndproc_new_memory_view(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return wMemory.wndproc_view(hwnd, msg, wparam, lparam);
}

};

long MemoryEditor::wndproc_view(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {

  case WM_KEYDOWN: {
    if(r_mem->cart_loaded() == false)break;

  int pos, len, xpos, ypos, t, data, read;
    pos = SendMessage(hwnd, EM_GETSEL, 0, 0);
    pos = LOWORD(pos);
    ypos = pos / 57;
    xpos = pos % 57;

    if(wparam == VK_UP) {
      if(ypos != 0)break;
      status.addr -= 16;
      status.addr &= status.mask;
      Refresh();
      SendMessage(hwnd, EM_SETSEL, pos, pos);
      break;
    }

    if(wparam == VK_DOWN) {
      if(ypos != 15)break;
      status.addr += 16;
      status.addr &= status.mask;
      Refresh();
      SendMessage(hwnd, EM_SETSEL, pos, pos);
      break;
    }

    if(wparam == VK_PRIOR) {
      if(KeyDown(VK_CONTROL))len = 65536;
      else if(KeyDown(VK_SHIFT))len = 4096;
      else len = 256;
      status.addr -= len;
      status.addr &= status.mask;
      Refresh();
      SendMessage(hwnd, EM_SETSEL, pos, pos);
      break;
    }

    if(wparam == VK_NEXT) {
      if(KeyDown(VK_CONTROL))len = 65536;
      else if(KeyDown(VK_SHIFT))len = 4096;
      else len = 256;
      status.addr += len;
      status.addr &= status.mask;
      Refresh();
      SendMessage(hwnd, EM_SETSEL, pos, pos);
      break;
    }

    if(xpos < 8)break;
    xpos -= 8;
    t = xpos % 3;
    xpos /= 3;
    if(xpos > 15)break;

    if(wparam >= '0' && wparam <= '9')read = wparam - '0';
    else if(wparam >= 'A' && wparam <= 'F')read = wparam - 'A' + 0x0a;
    else if(wparam >= 'a' && wparam <= 'f')read = wparam - 'a' + 0x0a;
    else break;

    data = debugger.read(status.mode, status.addr + ypos * 16 + xpos);
    if(t == 0) {
      data = (read << 4) | (data & 0x0f);
      pos++;
    } else {
      data = (data & 0xf0) | (read);
      if(xpos == 15) { //go to new line
        if(ypos == 15) { //so long as we aren't on the last line
          pos++;
        } else {
          pos += 11;
        }
      } else { //go to next byte
        pos += 2;
      }
    }
    debugger.write(status.mode, status.addr + ypos * 16 + xpos, data);
    Refresh();
    SendMessage(hwnd, EM_SETSEL, pos, pos);
  } break;

  }

  if(!NSMemoryEditor::wndproc_old_memory_view)return DefWindowProc(hwnd, msg, wparam, lparam);
  return CallWindowProc(NSMemoryEditor::wndproc_old_memory_view, hwnd, msg, wparam, lparam);
}

bool MemoryEditor::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    Hide();
    return true;
  } break;

  case EVENT_CHANGED:
  case EVENT_CLICKED: {
    if(info.control == &Mode && info.event_id == EVENT_CHANGED) {
      switch(Mode.GetSelection()) {
      case 0:
        status.mode = Debugger::DRAM;
        status.addr = 0x7e0000;
        status.mask = 0xffffff;
        break;
      case 1:
        status.mode = Debugger::DRAM;
        status.addr = 0x008000;
        status.mask = 0xffffff;
        break;
      case 2:
        status.mode = Debugger::DRAM;
        status.addr = 0x700000;
        status.mask = 0xffffff;
        break;
      case 3:
        status.mode = Debugger::SPCRAM;
        status.addr = 0x0000;
        status.mask = 0xffff;
        break;
      case 4:
        status.mode = Debugger::VRAM;
        status.addr = 0x0000;
        status.mask = 0xffff;
        break;
      case 5:
        status.mode = Debugger::OAM;
        status.addr = 0x0000;
        status.mask = 0x03ff;
        break;
      case 6:
        status.mode = Debugger::CGRAM;
        status.addr = 0x0000;
        status.mask = 0x01ff;
        break;
      }

      Refresh();
    } else if(info.control == &GotoAddr) {
    char t[16 + 1];
      GotoAddr.GetText(t, 16);
      status.addr = strhex(t) & status.mask;
      Refresh();
    } else if(info.control == &EditMem) {
    char t[16 + 1];
      Offset.GetText(t, 16);
    uint32 addr = strhex(t) & status.mask;
      Data.GetText(t, 16);
      debugger.write(status.mode, addr, strhex(t));
      Refresh();
    } else if(info.control == &ExportMem) {
      ExportMemory();
    } else if(info.control == &Update) {
      Refresh();
    }
  } break;

  }

  return false;
}

void MemoryEditor::Refresh() {
char s[4096], t[256];
  if(r_mem->cart_loaded() == false) {
    View.SetText("");
    return;
  }
  if(Visible() == false)return;

  strcpy(s, "");
  for(int y = 0; y < 16; y++) {
    sprintf(t, "%0.6x: ", (status.addr + y * 16) & status.mask);
    strcat(s, t);
    for(int x = 0; x < 16; x++) {
      sprintf(t, "%0.2x", debugger.read(status.mode, status.addr + y * 16 + x));
      strcat(s, t);
      if(x != 15)strcat(s, " ");
    }
    if(y != 15)strcat(s, "\r\n");
  }
  View.SetText(s);
}

void MemoryEditor::ExportMemory() {
FILE *fp;
  fp = fopen("dram.bin", "wb");
  if(fp) {
    for(int i = 0x7e0000; i <= 0x7fffff; i++) {
      fputc(debugger.read(Debugger::DRAM, i), fp);
    }
    fclose(fp);
  }
  fp = fopen("spcram.bin", "wb");
  if(fp) {
    for(int i = 0; i <= 0xffff; i++) {
      fputc(debugger.read(Debugger::SPCRAM, i), fp);
    }
    fclose(fp);
  }
  fp = fopen("vram.bin", "wb");
  if(fp) {
    for(int i = 0; i <= 0xffff; i++) {
      fputc(debugger.read(Debugger::VRAM, i), fp);
    }
    fclose(fp);
  }
  fp = fopen("oam.bin", "wb");
  if(fp) {
    for(int i = 0; i <= 0x03ff; i++) {
      fputc(debugger.read(Debugger::OAM, i), fp);
    }
    fclose(fp);
  }
  fp = fopen("cgram.bin", "wb");
  if(fp) {
    for(int i = 0; i <= 0x01ff; i++) {
      fputc(debugger.read(Debugger::CGRAM, i), fp);
    }
    fclose(fp);
  }
}

void MemoryEditor::Show() {
//Window will not refresh unless it is visible, so call Show() first
  Window::Show();
  Refresh();
}

void MemoryEditor::Setup() {
  View.Create(this, "visible|edge|multiline|readonly", 5, 5, 395, 235);
  View.SetFont(global::fwf);
  View.SetBackgroundColor(32, 32, 32);
  View.SetTextColor(255, 255, 255);
  NSMemoryEditor::wndproc_old_memory_view = (WNDPROC)GetWindowLong(View.hwnd, GWL_WNDPROC);
  SetWindowLong(View.hwnd, GWL_WNDPROC, (long)NSMemoryEditor::wndproc_new_memory_view);

  Mode.Create(this, "visible", 405, 5, 90, 200, "DRAM|ROM|SRAM|SPCRAM|VRAM|OAM|CGRAM");
  GotoLabel.Create(this, "visible", 405, 26 + 4, 35, 15, "Goto:");
  GotoAddr.Create(this, "visible|edge", 440, 26, 55, 20, "000000");
  GotoAddr.SetFont(global::fwf);

  OffsetLabel.Create(this, "visible", 405, 50, 90, 15, "Offset:       Data:");
  Offset.Create(this, "visible|edge", 405, 65, 55, 20, "000000");
  Offset.SetFont(global::fwf);
  Data.Create(this, "visible|edge", 460, 65, 35, 20, "00");
  Data.SetFont(global::fwf);
  EditMem.Create(this, "visible", 405, 85, 90, 20, "Edit Memory");
  ExportMem.Create(this, "visible", 405, 105, 90, 20, "Export Memory");

  AutoUpdate.Create(this, "visible|disabled", 405, 203, 90, 15, "Auto Update");
  Update.Create(this, "visible", 405, 220, 90, 20, "Update");
}

MemoryEditor::MemoryEditor() {
  status.mode = Debugger::DRAM;
  status.addr = 0x000000;
  status.mask = 0xffffff;
}
