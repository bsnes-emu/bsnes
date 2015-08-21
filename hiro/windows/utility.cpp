namespace hiro {

static const unsigned Windows2000  = 0x0500;
static const unsigned WindowsXP    = 0x0501;
static const unsigned WindowsVista = 0x0600;
static const unsigned Windows7     = 0x0601;

static auto OsVersion() -> unsigned {
  OSVERSIONINFO versionInfo = {0};
  versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&versionInfo);
  return (versionInfo.dwMajorVersion << 8) + (versionInfo.dwMajorVersion << 0);
}

static auto CreateBitmap(const image& icon) -> HBITMAP {
  HDC hdc = GetDC(0);
  BITMAPINFO bitmapInfo;
  memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
  bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfo.bmiHeader.biWidth = icon.width();
  bitmapInfo.bmiHeader.biHeight = -(signed)icon.height();  //bitmaps are stored upside down unless we negate height
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = BI_RGB;
  bitmapInfo.bmiHeader.biSizeImage = icon.size();
  void* bits = nullptr;
  HBITMAP hbitmap = CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &bits, NULL, 0);
  if(bits) memory::copy(bits, icon.data(), icon.size());
  ReleaseDC(0, hdc);
  return hbitmap;
}

static auto CreateRGB(const Color& color) -> COLORREF {
  return RGB(color.red(), color.green(), color.blue());
}

static auto DropPaths(WPARAM wparam) -> lstring {
  auto dropList = HDROP(wparam);
  auto fileCount = DragQueryFile(dropList, ~0u, nullptr, 0);

  lstring paths;
  for(auto n : range(fileCount)) {
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

static auto GetWindowZOrder(HWND hwnd) -> unsigned {
  unsigned z = 0;
  for(HWND next = hwnd; next != NULL; next = GetWindow(next, GW_HWNDPREV)) z++;
  return z;
}

static auto ImageList_Append(HIMAGELIST imageList, const image& source, unsigned scale) -> void {
  auto image = source;
  if(image.empty()) {
    image.allocate(scale, scale);
    image.fill(GetSysColor(COLOR_WINDOW));
  }
  image.transform();
  image.scale(scale, scale);
  HBITMAP bitmap = CreateBitmap(image);
  ImageList_Add(imageList, bitmap, NULL);
  DeleteObject(bitmap);
}

//post message only if said message is not already pending in the queue
static auto PostMessageOnce(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam) -> void {
  MSG msg;
  if(!PeekMessage(&msg, hwnd, id, id, PM_NOREMOVE)) {
    PostMessage(hwnd, id, wparam, lparam);
  }
}

static auto ScrollEvent(HWND hwnd, WPARAM wparam) -> unsigned {
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

  //Windows may clamp position to scrollbar range
  GetScrollInfo(hwnd, SB_CTL, &info);
  return info.nPos;
}

//separate because PopupMenu HWND does not contain GWLP_USERDATA pointing at Window needed for Shared_windowProc
static auto CALLBACK Menu_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  switch(msg) {
  case WM_MENUCOMMAND: {
    MENUITEMINFO mii{sizeof(MENUITEMINFO)};
    mii.fMask = MIIM_DATA;
    GetMenuItemInfo((HMENU)lparam, wparam, true, &mii);

    auto object = (mObject*)mii.dwItemData;
    if(!object) break;

    #if defined(Hiro_MenuItem)
    if(auto menuItem = dynamic_cast<mMenuItem*>(object)) {
      return menuItem->self()->onActivate(), false;
    }
    #endif

    #if defined(Hiro_MenuCheckItem)
    if(auto menuCheckItem = dynamic_cast<mMenuCheckItem*>(object)) {
      return menuCheckItem->self()->onToggle(), false;
    }
    #endif

    #if defined(Hiro_MenuRadioItem)
    if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object)) {
      return menuRadioItem->self()->onActivate(), false;
    }
    #endif

    break;
  }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

static auto CALLBACK Shared_windowProc(WindowProc windowProc, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(Application::state.quit) return DefWindowProc(hwnd, msg, wparam, lparam);

  auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto window = dynamic_cast<mWindow*>(object);
  if(!window) window = object->parentWindow(true);
  if(!window) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto pWindow = window->self();
  if(!pWindow) return DefWindowProc(hwnd, msg, wparam, lparam);

  if(pWindow->_modalityDisabled()) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_CTLCOLORBTN:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORSTATIC: {
    auto object = (mObject*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
    if(!object) break;

    //allow custom colors for various widgets
    //note that this happens always: default colors are black text on a white background, unless overridden
    //this intentionally overrides the default behavior of Windows to paint disabled controls with the window background color

    #if defined(Hiro_Window) && defined(Hiro_TabFrame)
    if(!object->parentTabFrame(true) && window->self()->hbrush) {
      SetBkColor((HDC)wparam, window->self()->hbrushColor);
      return (LRESULT)window->self()->hbrush;
    }
    #endif

    #if defined(Hiro_HexEdit)
    if(auto hexEdit = dynamic_cast<mHexEdit*>(object)) {
      if(auto background = hexEdit->backgroundColor()) SetBkColor((HDC)wparam, CreateRGB(background));
      if(auto foreground = hexEdit->foregroundColor()) SetTextColor((HDC)wparam, CreateRGB(foreground));
      return (LRESULT)hexEdit->self()->backgroundBrush;
    }
    #endif

    #if defined(Hiro_LineEdit)
    if(auto lineEdit = dynamic_cast<mLineEdit*>(object)) {
      if(auto background = lineEdit->backgroundColor()) SetBkColor((HDC)wparam, CreateRGB(background));
      if(auto foreground = lineEdit->foregroundColor()) SetTextColor((HDC)wparam, CreateRGB(foreground));
      return (LRESULT)lineEdit->self()->backgroundBrush;
    }
    #endif

    #if defined(Hiro_TextEdit)
    if(auto textEdit = dynamic_cast<mTextEdit*>(object)) {
      if(auto background = textEdit->backgroundColor()) SetBkColor((HDC)wparam, CreateRGB(background));
      if(auto foreground = textEdit->foregroundColor()) SetTextColor((HDC)wparam, CreateRGB(foreground));
      return (LRESULT)textEdit->self()->backgroundBrush;
    }
    #endif

    break;
  }

  case WM_DRAWITEM: {
    auto drawItem = (LPDRAWITEMSTRUCT)lparam;
    auto object = (mObject*)GetWindowLongPtr((HWND)drawItem->hwndItem, GWLP_USERDATA);
    if(!object) break;

    #if defined(Hiro_TabFrame)
    if(auto tabFrame = dynamic_cast<mTabFrame*>(object)) {
      return tabFrame->self()->onDrawItem(lparam), true;
    }
    #endif

    break;
  }

  case WM_MENUCOMMAND: {
    return Menu_windowProc(hwnd, msg, wparam, lparam);
  }

  case WM_COMMAND: {
    if(!lparam) break;
    auto object = (mObject*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
    if(!object) break;

    #if defined(Hiro_Button)
    if(auto button = dynamic_cast<mButton*>(object)) {
      return button->self()->onActivate(), false;
    }
    #endif

    #if defined(Hiro_CheckButton)
    if(auto checkButton = dynamic_cast<mCheckButton*>(object)) {
      return checkButton->self()->onToggle(), false;
    }
    #endif

    #if defined(Hiro_CheckLabel)
    if(auto checkLabel = dynamic_cast<mCheckLabel*>(object)) {
      return checkLabel->self()->onToggle(), false;
    }
    #endif

    #if defined(Hiro_ComboButton)
    if(auto comboButton = dynamic_cast<mComboButton*>(object)) {
      if(HIWORD(wparam) == CBN_SELCHANGE) {
        return comboButton->self()->onChange(), false;
      }
    }
    #endif

    #if defined(Hiro_LineEdit)
    if(auto lineEdit = dynamic_cast<mLineEdit*>(object)) {
      if(HIWORD(wparam) == EN_CHANGE) {
        return lineEdit->self()->onChange(), false;
      }
    }
    #endif

    #if defined(Hiro_RadioButton)
    if(auto radioButton = dynamic_cast<mRadioButton*>(object)) {
      return radioButton->self()->onActivate(), false;
    }
    #endif

    #if defined(Hiro_RadioLabel)
    if(auto radioLabel = dynamic_cast<mRadioLabel*>(object)) {
      return radioLabel->self()->onActivate(), false;
    }
    #endif

    #if defined(Hiro_TextEdit)
    if(auto textEdit = dynamic_cast<mTextEdit*>(object)) {
      if(HIWORD(wparam) == EN_CHANGE) {
        return textEdit->self()->onChange(), false;
      }
    }
    #endif

    break;
  }

  case WM_NOTIFY: {
    auto header = (LPNMHDR)lparam;
    auto object = (mObject*)GetWindowLongPtr((HWND)header->hwndFrom, GWLP_USERDATA);
    if(!object) break;

    #if defined(Hiro_ListView)
    if(auto listView = dynamic_cast<mListView*>(object)) {
      if(header->code == LVN_ITEMACTIVATE) {
        listView->self()->onActivate(lparam);
        break;
      }
      if(header->code == LVN_ITEMCHANGED) {
        listView->self()->onChange(lparam);
        break;
      }
      if(header->code == LVN_COLUMNCLICK) {
        listView->self()->onSort(lparam);
        break;
      }
      if(header->code == NM_CLICK || header->code == NM_DBLCLK) {
        listView->self()->onToggle(lparam);
        break;
      }
      if(header->code == NM_RCLICK) {
        listView->self()->onContext(lparam);
        break;
      }
      if(header->code == NM_CUSTOMDRAW) {
        return listView->self()->onCustomDraw(lparam);
      }
    }
    #endif

    #if defined(Hiro_TabFrame)
    if(auto tabFrame = dynamic_cast<mTabFrame*>(object)) {
      if(header->code == TCN_SELCHANGE) {
        tabFrame->self()->onChange();
        break;
      }
    }
    #endif

    break;
  }

  #if defined(Hiro_ListView)
  case AppMessage::ListView_doPaint: {
    if(auto listView = (mListView*)lparam) {
      if(auto self = listView->self()) InvalidateRect(self->hwnd, nullptr, true);
    }
    break;
  }

  case AppMessage::ListView_onActivate: {
    if(auto listView = (mListView*)lparam) listView->doActivate();
    break;
  }

  case AppMessage::ListView_onChange: {
    if(auto listView = (mListView*)lparam) listView->doChange();
  }
  #endif

  case WM_HSCROLL:
  case WM_VSCROLL: {
    if(!lparam) break;
    auto object = (mObject*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
    if(!object) break;

    #if defined(Hiro_HorizontalScrollBar)
    if(auto horizontalScrollBar = dynamic_cast<mHorizontalScrollBar*>(object)) {
      return horizontalScrollBar->self()->onChange(wparam), true;
    }
    #endif

    #if defined(Hiro_HorizontalSlider)
    if(auto horizontalSlider = dynamic_cast<mHorizontalSlider*>(object)) {
      return horizontalSlider->self()->onChange(), true;
    }
    #endif

    #if defined(Hiro_VerticalScrollBar)
    if(auto verticalScrollBar = dynamic_cast<mVerticalScrollBar*>(object)) {
      return verticalScrollBar->self()->onChange(wparam), true;
    }
    #endif

    #if defined(Hiro_VerticalSlider)
    if(auto verticalSlider = dynamic_cast<mVerticalSlider*>(object)) {
      return verticalSlider->self()->onChange(), true;
    }
    #endif

    break;
  }
  }

  return windowProc(hwnd, msg, wparam, lparam);
}

}
