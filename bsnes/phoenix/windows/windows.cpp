#include "windows.hpp"

#include "object.cpp"
#include "font.cpp"
#include "timer.cpp"
#include "message-window.cpp"
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
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/line-edit.cpp"
#include "widget/list-view.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-box.cpp"
#include "widget/text-edit.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

static void OS_keyboardProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK OS_windowProc(HWND, UINT, WPARAM, LPARAM);

pOS::State *pOS::state = 0;

Geometry pOS::availableGeometry() {
  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Geometry pOS::desktopGeometry() {
  return { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

static string pOS_fileDialog(bool save, Window &parent, const string &path, const lstring &filter) {
  string dir = path;
  dir.replace("/", "\\");

  string filterList;
  foreach(filterItem, filter) {
    lstring part;
    part.split("(", filterItem);
    if(part.size() != 2) { print("--", filterItem, "\n"); continue; }
    part[1].rtrim<1>(")");
    part[1].replace(" ", "");
    part[1].transform(",", ";");
    filterList.append(string(filterItem, "\t", part[1], "\t"));
  }

  utf16_t wfilter(filterList);
  utf16_t wdir(dir);
  wchar_t wfilename[PATH_MAX + 1] = L"";

  wchar_t *p = wfilter;
  while(*p != L'\0') {
    if(*p == L'\t') *p = L'\0';
    p++;
  }

  OPENFILENAME ofn;
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = &parent != &Window::None ? parent.p.hwnd : 0;
  ofn.lpstrFilter = wfilter;
  ofn.lpstrInitialDir = wdir;
  ofn.lpstrFile = wfilename;
  ofn.nMaxFile = PATH_MAX;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"";

  bool result = (save == false ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn));
  if(result == false) return "";
  string name = (const char*)utf8_t(wfilename);
  name.transform("\\", "/");
  return name;
}

string pOS::fileLoad(Window &parent, const string &path, const lstring &filter) {
  return pOS_fileDialog(false, parent, path, filter);
}

string pOS::fileSave(Window &parent, const string &path, const lstring &filter) {
  return pOS_fileDialog(true, parent, path, filter);
}

string pOS::folderSelect(Window &parent, const string &path) {
  wchar_t wfilename[PATH_MAX + 1] = L"";
  BROWSEINFO bi;
  bi.hwndOwner = &parent != &Window::None ? parent.p.hwnd : 0;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = wfilename;
  bi.lpszTitle = L"";
  bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;
  bool result = false;
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if(pidl) {
    if(SHGetPathFromIDList(pidl, wfilename)) {
      result = true;
      IMalloc *imalloc = 0;
      if(SUCCEEDED(SHGetMalloc(&imalloc))) {
        imalloc->Free(pidl);
        imalloc->Release();
      }
    }
  }
  if(result == false) return "";
  string name = (const char*)utf8_t(wfilename);
  if(name == "") return "";
  name.transform("\\", "/");
  if(name.endswith("/") == false) name.append("/");
  return name;
}

void pOS::main() {
  MSG msg;
  while(GetMessage(&msg, 0, 0, 0)) {
    if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
      OS_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }
    if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
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
      if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
        OS_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
      }
      if(!IsDialogMessage(GetParent(msg.hwnd) ? GetParent(msg.hwnd) : msg.hwnd, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
}

void pOS::quit() {
  PostQuitMessage(0);
}

void pOS::initialize() {
  CoInitialize(0);
  InitCommonControls();

  state = new State;

  state->defaultFont.setFamily("Tahoma");
  state->defaultFont.setSize(8);

  WNDCLASS wc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
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
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
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
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
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
}

static void OS_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg == WM_KEYDOWN) {
    GUITHREADINFO info;
    memset(&info, 0, sizeof(GUITHREADINFO));
    info.cbSize = sizeof(GUITHREADINFO);
    GetGUIThreadInfo(GetCurrentThreadId(), &info);
    Object *object = (Object*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
    if(object == 0) return;
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
    }
  }
}

static LRESULT CALLBACK OS_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object *object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object || !dynamic_cast<Window*>(object)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window &window = (Window&)*object;

  switch(msg) {
    case WM_CLOSE: {
      if(window.onClose) window.onClose();
      window.setVisible(false);
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

      foreach(layout, window.state.layout) {
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
    }

    case WM_COMMAND: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.p.hwnd, id);
      if(control == 0) {
        pObject *object = (pObject*)pObject::find(id);
        if(!object) break;
        if(dynamic_cast<pItem*>(object)) {
          Item &item = ((pItem*)object)->item;
          if(item.onTick) item.onTick();
        } else if(dynamic_cast<pCheckItem*>(object)) {
          CheckItem &checkItem = ((pCheckItem*)object)->checkItem;
          checkItem.setChecked(!checkItem.state.checked);
          if(checkItem.onTick) checkItem.onTick();
        } else if(dynamic_cast<pRadioItem*>(object)) {
          RadioItem &radioItem = ((pRadioItem*)object)->radioItem;
          if(radioItem.state.checked == false) {
            radioItem.setChecked();
            if(radioItem.onTick) radioItem.onTick();
          }
        }
      } else {
        Object *object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
        if(!object) break;
        if(dynamic_cast<Button*>(object)) {
          Button &button = (Button&)*object;
          if(button.onTick) button.onTick();
        } else if(dynamic_cast<CheckBox*>(object)) {
          CheckBox &checkBox = (CheckBox&)*object;
          checkBox.setChecked(!checkBox.state.checked);
          if(checkBox.onTick) checkBox.onTick();
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
            if(radioBox.onTick) radioBox.onTick();
          }
        } else if(dynamic_cast<TextEdit*>(object)) {
          TextEdit &textEdit = (TextEdit&)*object;
          if(HIWORD(wparam) == EN_CHANGE) {
            if(textEdit.p.locked == false && textEdit.onChange) textEdit.onChange();
          }
        }
      }
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
            if(listView.p.locked == false && listView.onTick) listView.onTick(nmlistview->iItem);
          } else if((nmlistview->uOldState & LVIS_FOCUSED) && !(nmlistview->uNewState & LVIS_FOCUSED)) {
            listView.p.lostFocus = true;
          } else {
            if(!(nmlistview->uOldState & LVIS_SELECTED) && (nmlistview->uNewState & LVIS_SELECTED)) {
              listView.state.selected = true;
              listView.state.selection = listView.selection();
              if(listView.p.locked == false && listView.onChange) listView.onChange();
            } else if(listView.p.lostFocus == false && listView.selected() == false) {
              listView.state.selected = true;
              listView.state.selection = listView.selection();
              if(listView.p.locked == false && listView.onChange) listView.onChange();
            }
            listView.p.lostFocus = false;
          }
        } else if(nmhdr->code == LVN_ITEMACTIVATE) {
          if(listView.onActivate) listView.onActivate();
        }
      }
    }

    case WM_HSCROLL:
    case WM_VSCROLL: {
      unsigned id = LOWORD(wparam);
      HWND control = GetDlgItem(window.p.hwnd, id);
      if(control == 0) break;
      Object *object = (Object*)GetWindowLongPtr(control, GWLP_USERDATA);
      if(object == 0) break;
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
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
