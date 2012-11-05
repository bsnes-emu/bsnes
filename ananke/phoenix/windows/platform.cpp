#include "platform.hpp"
#include "utility.cpp"
#include "settings.cpp"

#include "desktop.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"
#include "dialog-window.cpp"
#include "message-window.cpp"

#include "object.cpp"
#include "font.cpp"
#include "timer.cpp"
#include "window.cpp"

#include "action/action.cpp"
#include "action/menu.cpp"
#include "action/separator.cpp"
#include "action/item.cpp"
#include "action/check-item.cpp"
#include "action/radio-item.cpp"

#include "widget/widget.cpp"
#include "widget/button.cpp"
#include "widget/canvas.cpp"
#include "widget/check-box.cpp"
#include "widget/combo-box.cpp"
#include "widget/hex-edit.cpp"
#include "widget/horizontal-scroll-bar.cpp"
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/line-edit.cpp"
#include "widget/list-view.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-box.cpp"
#include "widget/text-edit.cpp"
#include "widget/vertical-scroll-bar.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

static bool OS_keyboardProc(HWND, UINT, WPARAM, LPARAM);
static void OS_processDialogMessage(MSG&);
static LRESULT CALLBACK OS_windowProc(HWND, UINT, WPARAM, LPARAM);

void pOS::main() {
  MSG msg;
  while(GetMessage(&msg, 0, 0, 0)) {
    OS_processDialogMessage(msg);
  }
}

bool pOS::pendingEvents() {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

void pOS::processEvents() {
  while(pendingEvents()) {
    MSG msg;
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      OS_processDialogMessage(msg);
    }
  }
}

void OS_processDialogMessage(MSG &msg) {
  if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP
  || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) {
    if(OS_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam)) {
      DispatchMessage(&msg);
      return;
    }
  }

  if(!IsDialogMessage(GetForegroundWindow(), &msg)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void pOS::quit() {
  osQuit = true;
  PostQuitMessage(0);
}

void pOS::initialize() {
  CoInitialize(0);
  InitCommonControls();

  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = OS_windowProc;
  wc.lpszClassName = L"phoenix_window";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Canvas_windowProc;
  wc.lpszClassName = L"phoenix_canvas";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Label_windowProc;
  wc.lpszClassName = L"phoenix_label";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Viewport_windowProc;
  wc.lpszClassName = L"phoenix_viewport";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  settings = new Settings;
  pKeyboard::initialize();
}

static bool OS_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg != WM_KEYDOWN && msg != WM_SYSKEYDOWN && msg != WM_KEYUP && msg != WM_SYSKEYUP) return false;

  GUITHREADINFO info;
  memset(&info, 0, sizeof(GUITHREADINFO));
  info.cbSize = sizeof(GUITHREADINFO);
  GetGUIThreadInfo(GetCurrentThreadId(), &info);
  Object *object = (Object*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
  if(object == nullptr) return false;

  if(dynamic_cast<Window*>(object)) {
    Window &window = (Window&)*object;
    if(pWindow::modal.size() > 0 && !pWindow::modal.find(&window.p)) return false;
    Keyboard::Keycode keysym = Keysym(wparam, lparam);
    if(keysym != Keyboard::Keycode::None) {
      if((msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) && window.onKeyPress) window.onKeyPress(keysym);
      if((msg == WM_KEYUP || msg == WM_SYSKEYUP) && window.onKeyRelease) window.onKeyRelease(keysym);
    }
    return false;
  }

  if(msg == WM_KEYDOWN) {
    if(dynamic_cast<ListView*>(object)) {
      ListView &listView = (ListView&)*object;
      if(wparam == VK_RETURN) {
        if(listView.onActivate) listView.onActivate();
      }
    } else if(dynamic_cast<LineEdit*>(object)) {
      LineEdit &lineEdit = (LineEdit&)*object;
      if(wparam == VK_RETURN) {
        if(lineEdit.onActivate) lineEdit.onActivate();
      }
    } else if(dynamic_cast<TextEdit*>(object)) {
      TextEdit &textEdit = (TextEdit&)*object;
      if(wparam == 'A' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+A = select all text
        //note: this is not a standard accelerator on Windows
        Edit_SetSel(textEdit.p.hwnd, 0, ~0);
        return true;
      } else if(wparam == 'V' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+V = paste text
        //note: this formats Unix (LF) and OS9 (CR) line-endings to Windows (CR+LF) line-endings
        //this is necessary as the EDIT control only supports Windows line-endings
        OpenClipboard(hwnd);
        HANDLE handle = GetClipboardData(CF_UNICODETEXT);
        if(handle) {
          wchar_t *text = (wchar_t*)GlobalLock(handle);
          if(text) {
            string data = (const char*)utf8_t(text);
            data.replace("\r\n", "\n");
            data.replace("\r", "\n");
            data.replace("\n", "\r\n");
            GlobalUnlock(handle);
            utf16_t output(data);
            HGLOBAL resource = GlobalAlloc(GMEM_MOVEABLE, (wcslen(output) + 1) * sizeof(wchar_t));
            if(resource) {
              wchar_t *write = (wchar_t*)GlobalLock(resource);
              if(write) {
                wcscpy(write, output);
                GlobalUnlock(write);
                if(SetClipboardData(CF_UNICODETEXT, resource) == FALSE) {
                  GlobalFree(resource);
                }
              }
            }
          }
        }
        CloseClipboard();
        return false;
      }
    }
  }

  return false;
}

static LRESULT CALLBACK OS_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object *object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object || !dynamic_cast<Window*>(object)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window &window = (Window&)*object;

  bool process = true;
  if(pWindow::modal.size() > 0 && !pWindow::modal.find(&window.p)) process = false;
  if(osQuit) process = false;

  if(process) switch(msg) {
    case WM_CLOSE: {
      window.state.ignore = false;
      if(window.onClose) window.onClose();
      if(window.state.ignore == false) {
        window.setVisible(false);
        window.setModal(false);
      }
      return TRUE;
    }

    case WM_MOVE: {
      if(window.p.locked) break;

      Geometry geometry = window.geometry();
      window.state.geometry.x = geometry.x;
      window.state.geometry.y = geometry.y;

      if(window.onMove) window.onMove();
      break;
    }

    case WM_SIZE: {
      if(window.p.locked) break;
      SetWindowPos(window.p.hstatus, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);

      Geometry geometry = window.geometry();
      window.state.geometry.width = geometry.width;
      window.state.geometry.height = geometry.height;

      for(auto &layout : window.state.layout) {
        Geometry geom = window.geometry();
        geom.x = geom.y = 0;
        layout.setGeometry(geom);
      }

      if(window.onSize) window.onSize();
      break;
    }

    case WM_GETMINMAXINFO: {
      MINMAXINFO *mmi = (MINMAXINFO*)lparam;
    //mmi->ptMinTrackSize.x = 256 + window.p.frameMargin().width;
    //mmi->ptMinTrackSize.y = 256 + window.p.frameMargin().height;
    //return TRUE;
      break;
    }

    case WM_ERASEBKGND: {
      if(window.p.brush == 0) break;
      RECT rc;
      GetClientRect(window.p.hwnd, &rc);
      PAINTSTRUCT ps;
      BeginPaint(window.p.hwnd, &ps);
      FillRect(ps.hdc, &rc, window.p.brush);
      EndPaint(window.p.hwnd, &ps);
      return TRUE;
    }

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC: {
      Object *object = (Object*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
      if(object && window.p.brush) {
        HDC hdc = (HDC)wparam;
        SetBkColor((HDC)wparam, window.p.brushColor);
        return (INT_PTR)window.p.brush;
      }
      break;
    }

    case WM_COMMAND: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.p.hwnd, id);
      if(control == 0) {
        pObject *object = (pObject*)pObject::find(id);
        if(!object) break;
        if(dynamic_cast<pItem*>(object)) {
          Item &item = ((pItem*)object)->item;
          if(item.onActivate) item.onActivate();
        } else if(dynamic_cast<pCheckItem*>(object)) {
          CheckItem &checkItem = ((pCheckItem*)object)->checkItem;
          checkItem.setChecked(!checkItem.state.checked);
          if(checkItem.onToggle) checkItem.onToggle();
        } else if(dynamic_cast<pRadioItem*>(object)) {
          RadioItem &radioItem = ((pRadioItem*)object)->radioItem;
          if(radioItem.state.checked == false) {
            radioItem.setChecked();
            if(radioItem.onActivate) radioItem.onActivate();
          }
        }
      } else {
        Object *object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(!object) break;
        if(dynamic_cast<Button*>(object)) {
          Button &button = (Button&)*object;
          if(button.onActivate) button.onActivate();
        } else if(dynamic_cast<CheckBox*>(object)) {
          CheckBox &checkBox = (CheckBox&)*object;
          checkBox.setChecked(!checkBox.state.checked);
          if(checkBox.onToggle) checkBox.onToggle();
        } else if(dynamic_cast<ComboBox*>(object)) {
          ComboBox &comboBox = (ComboBox&)*object;
          if(HIWORD(wparam) == CBN_SELCHANGE) {
            if(comboBox.state.selection != comboBox.selection()) {
              comboBox.state.selection = comboBox.selection();
              if(comboBox.onChange) comboBox.onChange();
            }
          }
        } else if(dynamic_cast<LineEdit*>(object)) {
          LineEdit &lineEdit = (LineEdit&)*object;
          if(HIWORD(wparam) == EN_CHANGE) {
            if(lineEdit.p.locked == false && lineEdit.onChange) lineEdit.onChange();
          }
        } else if(dynamic_cast<RadioBox*>(object)) {
          RadioBox &radioBox = (RadioBox&)*object;
          if(radioBox.state.checked == false) {
            radioBox.setChecked();
            if(radioBox.onActivate) radioBox.onActivate();
          }
        } else if(dynamic_cast<TextEdit*>(object)) {
          TextEdit &textEdit = (TextEdit&)*object;
          if(HIWORD(wparam) == EN_CHANGE) {
            if(textEdit.p.locked == false && textEdit.onChange) textEdit.onChange();
          }
        }
      }
      break;
    }

    case WM_NOTIFY: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.p.hwnd, id);
      if(control == 0) break;
      Object *object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
      if(object == 0) break;
      if(dynamic_cast<ListView*>(object)) {
        ListView &listView = (ListView&)*object;
        LPNMHDR nmhdr = (LPNMHDR)lparam;
        LPNMLISTVIEW nmlistview = (LPNMLISTVIEW)lparam;

        if(nmhdr->code == LVN_ITEMCHANGED && (nmlistview->uChanged & LVIF_STATE)) {
          unsigned imagemask = ((nmlistview->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          if(imagemask == 0 || imagemask == 1) {
            if(listView.p.locked == false && listView.onToggle) listView.onToggle(nmlistview->iItem);
          } else if((nmlistview->uOldState & LVIS_FOCUSED) && !(nmlistview->uNewState & LVIS_FOCUSED)) {
            listView.p.lostFocus = true;
          } else if(!(nmlistview->uOldState & LVIS_SELECTED) && (nmlistview->uNewState & LVIS_SELECTED)) {
            listView.p.lostFocus = false;
            listView.state.selected = true;
            listView.state.selection = listView.selection();
            if(listView.p.locked == false && listView.onChange) listView.onChange();
          } else if(listView.p.lostFocus == false && listView.selected() == false) {
            listView.p.lostFocus = false;
            listView.state.selected = false;
            listView.state.selection = 0;
            if(listView.p.locked == false && listView.onChange) listView.onChange();
          }
        } else if(nmhdr->code == LVN_ITEMACTIVATE) {
          if(listView.onActivate) listView.onActivate();
        } else if(nmhdr->code == NM_CUSTOMDRAW) {
          LPNMLVCUSTOMDRAW lvcd = (LPNMLVCUSTOMDRAW)nmhdr;
          switch(lvcd->nmcd.dwDrawStage) {
          case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;
          case CDDS_ITEMPREPAINT:
            if(listView.state.headerText.size() >= 2) {
              //draw alternating row colors of there are two or more columns
              if(lvcd->nmcd.dwItemSpec % 2) lvcd->clrTextBk = GetSysColor(COLOR_WINDOW) ^ 0x070707;
            }
            return CDRF_DODEFAULT;
          default:
            return CDRF_DODEFAULT;
          }
        }
      }
      break;
    }

    case WM_HSCROLL:
    case WM_VSCROLL: {
      Object *object = 0;
      if(lparam) {
        object = (Object*)GetWindowLongPtr((HWND)lparam, GWLP_USERDATA);
      } else {
        unsigned id = LOWORD(wparam);
        HWND control = GetDlgItem(window.p.hwnd, id);
        if(control == 0) break;
        object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
      }
      if(object == 0) break;

      if(dynamic_cast<HorizontalScrollBar*>(object)
      || dynamic_cast<VerticalScrollBar*>(object)) {
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

        //Windows may clamp position to scrollbar range
        GetScrollInfo((HWND)lparam, SB_CTL, &info);

        if(dynamic_cast<HorizontalScrollBar*>(object)) {
          HorizontalScrollBar &horizontalScrollBar = (HorizontalScrollBar&)*object;
          if(horizontalScrollBar.state.position != info.nPos) {
            horizontalScrollBar.state.position = info.nPos;
            if(horizontalScrollBar.onChange) horizontalScrollBar.onChange();
          }
        } else {
          VerticalScrollBar &verticalScrollBar = (VerticalScrollBar&)*object;
          if(verticalScrollBar.state.position != info.nPos) {
            verticalScrollBar.state.position = info.nPos;
            if(verticalScrollBar.onChange) verticalScrollBar.onChange();
          }
        }

        return TRUE;
      }

      if(dynamic_cast<HorizontalSlider*>(object)) {
        HorizontalSlider &horizontalSlider = (HorizontalSlider&)*object;
        if(horizontalSlider.state.position != horizontalSlider.position()) {
          horizontalSlider.state.position = horizontalSlider.position();
          if(horizontalSlider.onChange) horizontalSlider.onChange();
        }
      } else if(dynamic_cast<VerticalSlider*>(object)) {
        VerticalSlider &verticalSlider = (VerticalSlider&)*object;
        if(verticalSlider.state.position != verticalSlider.position()) {
          verticalSlider.state.position = verticalSlider.position();
          if(verticalSlider.onChange) verticalSlider.onChange();
        }
      }

      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
