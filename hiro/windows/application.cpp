#if defined(Hiro_Application)

namespace hiro {

static auto Application_keyboardProc(HWND, UINT, WPARAM, LPARAM) -> bool;
static auto Application_processDialogMessage(MSG&) -> void;
static auto CALLBACK Application_windowProc(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

auto pApplication::run() -> void {
  MSG msg;
  if(Application::state.onMain) {
    while(!Application::state.quit) {
      Application::doMain();
      processEvents();
    }
  } else {
    MSG msg;
    while(GetMessage(&msg, 0, 0, 0)) {
      Application_processDialogMessage(msg);
    }
  }
}

auto pApplication::pendingEvents() -> bool {
  MSG msg;
  return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
}

auto pApplication::processEvents() -> void {
  while(pendingEvents()) {
    MSG msg;
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      Application_processDialogMessage(msg);
    }
  }
}

auto Application_processDialogMessage(MSG& msg) -> void {
  if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP
  || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) {
    if(Application_keyboardProc(msg.hwnd, msg.message, msg.wParam, msg.lParam)) {
      DispatchMessage(&msg);
      return;
    }
  }

  if(!IsDialogMessage(GetForegroundWindow(), &msg)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

auto pApplication::quit() -> void {
  PostQuitMessage(0);
}

auto pApplication::initialize() -> void {
  CoInitialize(0);
  InitCommonControls();

  WNDCLASS wc;

  #if defined(Hiro_Window)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Application_windowProc;
  wc.lpszClassName = L"hiroWindow";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_PopupMenu)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(2));
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Menu_windowProc;
  wc.lpszClassName = L"hiroPopupMenu";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_Canvas)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Canvas_windowProc;
  wc.lpszClassName = L"hiroCanvas";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_Label)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Label_windowProc;
  wc.lpszClassName = L"hiroLabel";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  #if defined(Hiro_Viewport)
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance = GetModuleHandle(0);
  wc.lpfnWndProc = Viewport_windowProc;
  wc.lpszClassName = L"hiroViewport";
  wc.lpszMenuName = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);
  #endif

  pKeyboard::initialize();
}

static auto Application_keyboardProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> bool {
  if(msg != WM_KEYDOWN && msg != WM_SYSKEYDOWN && msg != WM_KEYUP && msg != WM_SYSKEYUP) return false;

  GUITHREADINFO info{sizeof(GUITHREADINFO)};
  GetGUIThreadInfo(GetCurrentThreadId(), &info);

  auto object = (mObject*)GetWindowLongPtr(info.hwndFocus, GWLP_USERDATA);
  if(!object) return false;

  if(auto window = dynamic_cast<mWindow*>(object)) {
    if(auto self = window->self()) {
      if(!self->_modalityDisabled()) {
        if(auto code = pKeyboard::_translate(wparam, lparam)) {
          if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) window->doKeyPress(code);
          if(msg == WM_KEYUP || msg == WM_SYSKEYUP) window->doKeyRelease(code);
        }
      }
    }
    return false;
  }

  if(auto window = object->parentWindow(true)) {
    if(auto self = window->self()) {
      if(self->_modalityDisabled()) return false;
    }
  }

  if(msg == WM_KEYDOWN) {
    if(0);

    #if defined(Hiro_ListView)
    else if(auto listView = dynamic_cast<mListView*>(object)) {
      if(wparam == VK_RETURN) {
        if(listView->selected()) return true;  //returning true generates LVN_ITEMACTIVATE message
      }
    }
    #endif

    #if defined(Hiro_LineEdit)
    else if(auto lineEdit = dynamic_cast<mLineEdit*>(object)) {
      if(wparam == VK_RETURN) {
        lineEdit->doActivate();
      }
    }
    #endif

    #if defined(Hiro_TextEdit)
    else if(auto textEdit = dynamic_cast<mTextEdit*>(object)) {
      if(wparam == 'A' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+A = select all text
        //note: this is not a standard accelerator on Windows
        Edit_SetSel(textEdit->self()->hwnd, 0, ~0);
        return true;
      } else if(wparam == 'V' && GetKeyState(VK_CONTROL) < 0) {
        //Ctrl+V = paste text
        //note: this formats Unix (LF) and OS9 (CR) line-endings to Windows (CR+LF) line-endings
        //this is necessary as the EDIT control only supports Windows line-endings
        OpenClipboard(hwnd);
        if(auto handle = GetClipboardData(CF_UNICODETEXT)) {
          if(auto text = (wchar_t*)GlobalLock(handle)) {
            string data = (const char*)utf8_t(text);
            data.replace("\r\n", "\n");
            data.replace("\r", "\n");
            data.replace("\n", "\r\n");
            GlobalUnlock(handle);
            utf16_t output(data);
            if(auto resource = GlobalAlloc(GMEM_MOVEABLE, (wcslen(output) + 1) * sizeof(wchar_t))) {
              if(auto write = (wchar_t*)GlobalLock(resource)) {
                wcscpy(write, output);
                GlobalUnlock(write);
                if(SetClipboardData(CF_UNICODETEXT, resource) == NULL) {
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
    #endif
  }

  return false;
}

/*
case WM_GETMINMAXINFO: {
  MINMAXINFO* mmi = (MINMAXINFO*)lparam;
  mmi->ptMinTrackSize.x = 256 + window.p.frameMargin().width;
  mmi->ptMinTrackSize.y = 256 + window.p.frameMargin().height;
  return TRUE;
  break;
}
*/

static auto CALLBACK Application_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
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
  case WM_CLOSE: pWindow->onClose(); return true;
  case WM_MOVE: pWindow->onMove(); break;
  case WM_SIZE: pWindow->onSize(); break;
  case WM_DROPFILES: pWindow->onDrop(wparam); return false;
  case WM_ERASEBKGND: if(pWindow->onEraseBackground()) return true; break;
  case WM_ENTERMENULOOP: case WM_ENTERSIZEMOVE: pWindow->onModalBegin(); return false;
  case WM_EXITMENULOOP: case WM_EXITSIZEMOVE: pWindow->onModalEnd(); return false;
  }

  return Shared_windowProc(DefWindowProc, hwnd, msg, wparam, lparam);
}

}

#endif
