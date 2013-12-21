namespace phoenix {

static const unsigned Windows2000  = 0x0500;
static const unsigned WindowsXP    = 0x0501;
static const unsigned WindowsVista = 0x0600;
static const unsigned Windows7     = 0x0601;

static unsigned OsVersion() {
  OSVERSIONINFO versionInfo = {0};
  versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&versionInfo);
  return (versionInfo.dwMajorVersion << 8) + (versionInfo.dwMajorVersion << 0);
}

static HBITMAP CreateBitmap(const image& image) {
  HDC hdc = GetDC(0);
  BITMAPINFO bitmapInfo;
  memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
  bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfo.bmiHeader.biWidth = image.width;
  bitmapInfo.bmiHeader.biHeight = -image.height;  //bitmaps are stored upside down unless we negate height
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = BI_RGB;
  bitmapInfo.bmiHeader.biSizeImage = image.width * image.height * 4;
  void* bits = nullptr;
  HBITMAP hbitmap = CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &bits, NULL, 0);
  if(bits) memcpy(bits, image.data, image.width * image.height * 4);
  ReleaseDC(0, hdc);
  return hbitmap;
}

static lstring DropPaths(WPARAM wparam) {
  auto dropList = HDROP(wparam);
  auto fileCount = DragQueryFile(dropList, ~0u, nullptr, 0);

  lstring paths;
  for(unsigned n = 0; n < fileCount; n++) {
    auto length = DragQueryFile(dropList, n, nullptr, 0);
    auto buffer = new wchar_t[length + 1];

    if(DragQueryFile(dropList, n, buffer, length + 1)) {
      string path = (const char*)utf8_t(buffer);
      path.transform("\\", "/");
      if(directory::exists(path) && !path.endsWith("/")) path.append("/");
      paths.append(path);
    }

    delete[] buffer;
  }

  return paths;
}

static Layout* GetParentWidgetLayout(Sizable* sizable) {
  while(sizable) {
    if(sizable->state.parent && dynamic_cast<TabFrame*>(sizable->state.parent)) return (Layout*)sizable;
    sizable = sizable->state.parent;
  }
  return nullptr;
}

static Widget* GetParentWidget(Sizable* sizable) {
  while(sizable) {
    if(sizable->state.parent && dynamic_cast<TabFrame*>(sizable->state.parent)) return (Widget*)sizable->state.parent;
    sizable = sizable->state.parent;
  }
  return nullptr;
}

static unsigned GetWindowZOrder(HWND hwnd) {
  unsigned z = 0;
  for(HWND next = hwnd; next != NULL; next = GetWindow(next, GW_HWNDPREV)) z++;
  return z;
}

static void ImageList_Append(HIMAGELIST imageList, const nall::image& source, unsigned scale) {
  auto image = source;
  if(image.empty()) {
    image.allocate(scale, scale);
    image.fill(GetSysColor(COLOR_WINDOW));
  }
  image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
  image.scale(scale, scale);
  HBITMAP bitmap = CreateBitmap(image);
  ImageList_Add(imageList, bitmap, NULL);
  DeleteObject(bitmap);
}

static Keyboard::Keycode Keysym(unsigned keysym, unsigned keyflags) {
  #define pressed(keysym) (GetAsyncKeyState(keysym) & 0x8000)
  #define enabled(keysym) (GetKeyState(keysym))
  #define shifted() (pressed(VK_LSHIFT) || pressed(VK_RSHIFT))
  #define extended() (keyflags & (1 << 24))

  switch(keysym) {
  case VK_ESCAPE: return Keyboard::Keycode::Escape;
  case VK_F1: return Keyboard::Keycode::F1;
  case VK_F2: return Keyboard::Keycode::F2;
  case VK_F3: return Keyboard::Keycode::F3;
  case VK_F4: return Keyboard::Keycode::F4;
  case VK_F5: return Keyboard::Keycode::F5;
  case VK_F6: return Keyboard::Keycode::F6;
  case VK_F7: return Keyboard::Keycode::F7;
  case VK_F8: return Keyboard::Keycode::F8;
  case VK_F9: return Keyboard::Keycode::F9;
  //Keyboard::Keycode::F10 (should be captured under VK_MENU from WM_SYSKEY(UP,DOWN); but this is not working...)
  case VK_F11: return Keyboard::Keycode::F11;
  case VK_F12: return Keyboard::Keycode::F12;

  //Keyboard::Keycode::PrintScreen
  //Keyboard::Keycode::SysRq
  case VK_SCROLL: return Keyboard::Keycode::ScrollLock;
  case VK_PAUSE: return Keyboard::Keycode::Pause;
  //Keyboard::Keycode::Break

  case VK_INSERT: return extended() ? Keyboard::Keycode::Insert : Keyboard::Keycode::KeypadInsert;
  case VK_DELETE: return extended() ? Keyboard::Keycode::Delete : Keyboard::Keycode::KeypadDelete;
  case VK_HOME: return extended() ? Keyboard::Keycode::Home : Keyboard::Keycode::KeypadHome;
  case VK_END: return extended() ? Keyboard::Keycode::End : Keyboard::Keycode::KeypadEnd;
  case VK_PRIOR: return extended() ? Keyboard::Keycode::PageUp : Keyboard::Keycode::KeypadPageUp;
  case VK_NEXT: return extended() ? Keyboard::Keycode::PageDown : Keyboard::Keycode::KeypadPageDown;

  case VK_UP: return extended() ? Keyboard::Keycode::Up : Keyboard::Keycode::KeypadUp;
  case VK_DOWN: return extended() ? Keyboard::Keycode::Down : Keyboard::Keycode::KeypadDown;
  case VK_LEFT: return extended() ? Keyboard::Keycode::Left : Keyboard::Keycode::KeypadLeft;
  case VK_RIGHT: return extended() ? Keyboard::Keycode::Right : Keyboard::Keycode::KeypadRight;

  case VK_OEM_3: return !shifted() ? Keyboard::Keycode::Grave : Keyboard::Keycode::Tilde;
  case '1': return !shifted() ? Keyboard::Keycode::Number1 : Keyboard::Keycode::Exclamation;
  case '2': return !shifted() ? Keyboard::Keycode::Number2 : Keyboard::Keycode::At;
  case '3': return !shifted() ? Keyboard::Keycode::Number3 : Keyboard::Keycode::Pound;
  case '4': return !shifted() ? Keyboard::Keycode::Number4 : Keyboard::Keycode::Dollar;
  case '5': return !shifted() ? Keyboard::Keycode::Number5 : Keyboard::Keycode::Percent;
  case '6': return !shifted() ? Keyboard::Keycode::Number6 : Keyboard::Keycode::Power;
  case '7': return !shifted() ? Keyboard::Keycode::Number7 : Keyboard::Keycode::Ampersand;
  case '8': return !shifted() ? Keyboard::Keycode::Number8 : Keyboard::Keycode::Asterisk;
  case '9': return !shifted() ? Keyboard::Keycode::Number9 : Keyboard::Keycode::ParenthesisLeft;
  case '0': return !shifted() ? Keyboard::Keycode::Number0 : Keyboard::Keycode::ParenthesisRight;
  case VK_OEM_MINUS: return !shifted() ? Keyboard::Keycode::Minus : Keyboard::Keycode::Underscore;
  case VK_OEM_PLUS: return !shifted() ? Keyboard::Keycode::Equal : Keyboard::Keycode::Plus;
  case VK_BACK: return Keyboard::Keycode::Backspace;

  case VK_OEM_4: return !shifted() ? Keyboard::Keycode::BracketLeft : Keyboard::Keycode::BraceLeft;
  case VK_OEM_6: return !shifted() ? Keyboard::Keycode::BracketRight : Keyboard::Keycode::BraceRight;
  case VK_OEM_5: return !shifted() ? Keyboard::Keycode::Backslash : Keyboard::Keycode::Pipe;
  case VK_OEM_1: return !shifted() ? Keyboard::Keycode::Semicolon : Keyboard::Keycode::Colon;
  case VK_OEM_7: return !shifted() ? Keyboard::Keycode::Apostrophe : Keyboard::Keycode::Quote;
  case VK_OEM_COMMA: return !shifted() ? Keyboard::Keycode::Comma : Keyboard::Keycode::CaretLeft;
  case VK_OEM_PERIOD: return !shifted() ? Keyboard::Keycode::Period : Keyboard::Keycode::CaretRight;
  case VK_OEM_2: return !shifted() ? Keyboard::Keycode::Slash : Keyboard::Keycode::Question;

  case VK_TAB: return Keyboard::Keycode::Tab;
  case VK_CAPITAL: return Keyboard::Keycode::CapsLock;
  case VK_RETURN: return !extended() ? Keyboard::Keycode::Return : Keyboard::Keycode::Enter;
  case VK_SHIFT: return !pressed(VK_RSHIFT) ? Keyboard::Keycode::ShiftLeft : Keyboard::Keycode::ShiftRight;
  case VK_CONTROL: return !pressed(VK_RCONTROL) ? Keyboard::Keycode::ControlLeft : Keyboard::Keycode::ControlRight;
  case VK_LWIN: return Keyboard::Keycode::SuperLeft;
  case VK_RWIN: return Keyboard::Keycode::SuperRight;
  case VK_MENU:
    if(keyflags & (1 << 24)) return Keyboard::Keycode::AltRight;
    return Keyboard::Keycode::AltLeft;
  case VK_SPACE: return Keyboard::Keycode::Space;
  case VK_APPS: return Keyboard::Keycode::Menu;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
  case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
    if(enabled(VK_CAPITAL)) {
      if(shifted()) {
        return (Keyboard::Keycode)((unsigned)Keyboard::Keycode::a + keysym - 'A');
      } else {
        return (Keyboard::Keycode)((unsigned)Keyboard::Keycode::A + keysym - 'A');
      }
    } else {
      if(shifted()) {
        return (Keyboard::Keycode)((unsigned)Keyboard::Keycode::A + keysym - 'A');
      } else {
        return (Keyboard::Keycode)((unsigned)Keyboard::Keycode::a + keysym - 'A');
      }
    }
    break;

  case VK_NUMLOCK: return Keyboard::Keycode::NumLock;
  case VK_DIVIDE: return Keyboard::Keycode::Divide;
  case VK_MULTIPLY: return Keyboard::Keycode::Multiply;
  case VK_SUBTRACT: return Keyboard::Keycode::Subtract;
  case VK_ADD: return Keyboard::Keycode::Add;
  case VK_DECIMAL: return Keyboard::Keycode::Point;
  case VK_NUMPAD1: return Keyboard::Keycode::Keypad1;
  case VK_NUMPAD2: return Keyboard::Keycode::Keypad2;
  case VK_NUMPAD3: return Keyboard::Keycode::Keypad3;
  case VK_NUMPAD4: return Keyboard::Keycode::Keypad4;
  case VK_NUMPAD5: return Keyboard::Keycode::Keypad5;
  case VK_NUMPAD6: return Keyboard::Keycode::Keypad6;
  case VK_NUMPAD7: return Keyboard::Keycode::Keypad7;
  case VK_NUMPAD8: return Keyboard::Keycode::Keypad8;
  case VK_NUMPAD9: return Keyboard::Keycode::Keypad9;
  case VK_NUMPAD0: return Keyboard::Keycode::Keypad0;

  case VK_CLEAR: return Keyboard::Keycode::KeypadCenter;
  }

  return Keyboard::Keycode::None;

  #undef pressed
  #undef enabled
  #undef shifted
  #undef extended
}

static unsigned ScrollEvent(HWND hwnd, WPARAM wparam) {
  SCROLLINFO info;
  memset(&info, 0, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_ALL;
  GetScrollInfo(hwnd, SB_CTL, &info);

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
  SetScrollInfo(hwnd, SB_CTL, &info, TRUE);

  //Windows may clamp position to scroller range
  GetScrollInfo(hwnd, SB_CTL, &info);
  return info.nPos;
}

static LRESULT CALLBACK Shared_windowProc(WindowProc windowProc, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object* object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window& window = dynamic_cast<Window*>(object) ? *(Window*)object : *((Widget*)object)->Sizable::state.window;

  bool process = true;
  if(!pWindow::modal.empty() && !pWindow::modal.find(&window.p)) process = false;
  if(applicationState.quit) process = false;
  if(process == false) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORSTATIC: {
    Object* object = (Object*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
    if(object == nullptr) break;
    if(dynamic_cast<HexEdit*>(object) || dynamic_cast<LineEdit*>(object) || dynamic_cast<TextEdit*>(object)) {
      //text edit controls, when disabled, use CTLCOLORSTATIC instead of CTLCOLOREDIT
      //override this behavior: we do not want read-only edit controls to use the parent window background color
      return windowProc(hwnd, WM_CTLCOLOREDIT, wparam, lparam);
    } else if(!GetParentWidget((Sizable*)object) && window.p.brush) {
      SetBkColor((HDC)wparam, window.p.brushColor);
      return (INT_PTR)window.p.brush;
    }
    break;
  }

  case WM_DRAWITEM: {
    unsigned id = LOWORD(wparam);
    HWND control = GetDlgItem(hwnd, id);
    Object* object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
    if(object == nullptr) break;
    if(dynamic_cast<TabFrame*>(object)) { ((TabFrame*)object)->p.onDrawItem(lparam); return TRUE; }
    break;
  }

  case WM_COMMAND: {
    unsigned id = LOWORD(wparam);
    HWND control = GetDlgItem(hwnd, id);
    Object* object = control ? (Object*)GetWindowLongPtr(control, GWLP_USERDATA) : pObject::find(id);
    if(object == nullptr) break;
    if(dynamic_cast<Item*>(object)) { ((Item*)object)->p.onActivate(); return FALSE; }
    if(dynamic_cast<CheckItem*>(object)) { ((CheckItem*)object)->p.onToggle(); return FALSE; }
    if(dynamic_cast<RadioItem*>(object)) { ((RadioItem*)object)->p.onActivate(); return FALSE; }
    if(dynamic_cast<Button*>(object)) { ((Button*)object)->p.onActivate(); return FALSE; }
    if(dynamic_cast<CheckButton*>(object)) { ((CheckButton*)object)->p.onToggle(); return FALSE; }
    if(dynamic_cast<CheckLabel*>(object)) { ((CheckLabel*)object)->p.onToggle(); return FALSE; }
    if(dynamic_cast<ComboButton*>(object) && HIWORD(wparam) == CBN_SELCHANGE) { ((ComboButton*)object)->p.onChange(); return FALSE; }
    if(dynamic_cast<LineEdit*>(object) && HIWORD(wparam) == EN_CHANGE) { ((LineEdit*)object)->p.onChange(); return FALSE; }
    if(dynamic_cast<RadioButton*>(object)) { ((RadioButton*)object)->p.onActivate(); return FALSE; }
    if(dynamic_cast<RadioLabel*>(object)) { ((RadioLabel*)object)->p.onActivate(); return FALSE; }
    if(dynamic_cast<TextEdit*>(object) && HIWORD(wparam) == EN_CHANGE) { ((TextEdit*)object)->p.onChange(); return FALSE; }
    break;
  }

  case WM_NOTIFY: {
    unsigned id = LOWORD(wparam);
    HWND control = GetDlgItem(hwnd, id);
    Object* object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
    if(object == nullptr) break;
    if(dynamic_cast<ListView*>(object) && ((LPNMHDR)lparam)->code == LVN_ITEMACTIVATE) { ((ListView*)object)->p.onActivate(lparam); break; }
    if(dynamic_cast<ListView*>(object) && ((LPNMHDR)lparam)->code == LVN_ITEMCHANGED) { ((ListView*)object)->p.onChange(lparam); break; }
    if(dynamic_cast<ListView*>(object) && ((LPNMHDR)lparam)->code == NM_CUSTOMDRAW) { return ((ListView*)object)->p.onCustomDraw(lparam); }
    if(dynamic_cast<TabFrame*>(object) && ((LPNMHDR)lparam)->code == TCN_SELCHANGE) { ((TabFrame*)object)->p.onChange(); break; }
    break;
  }

  case WM_HSCROLL:
  case WM_VSCROLL: {
    Object* object = nullptr;
    if(lparam) {
      object = (Object*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
    } else {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(hwnd, id);
      object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
    }
    if(object == nullptr) break;
    if(dynamic_cast<HorizontalScroller*>(object)) { ((HorizontalScroller*)object)->p.onChange(wparam); return TRUE; }
    if(dynamic_cast<VerticalScroller*>(object)) { ((VerticalScroller*)object)->p.onChange(wparam); return TRUE; }
    if(dynamic_cast<HorizontalSlider*>(object)) { ((HorizontalSlider*)object)->p.onChange(); return TRUE; }
    if(dynamic_cast<VerticalSlider*>(object)) { ((VerticalSlider*)object)->p.onChange(); return TRUE; }
    break;
  }
  }

  return windowProc(hwnd, msg, wparam, lparam);
}

}
