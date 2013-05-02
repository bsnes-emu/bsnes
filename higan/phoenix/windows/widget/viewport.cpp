namespace phoenix {

static LRESULT CALLBACK Viewport_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object* object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  if(!dynamic_cast<Viewport*>(object)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Viewport& viewport = (Viewport&)*object;

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT tracker = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd};
    TrackMouseEvent(&tracker);
    if(viewport.onMouseMove) viewport.onMouseMove({(int16_t)LOWORD(lparam), (int16_t)HIWORD(lparam)});
  }

  if(msg == WM_MOUSELEAVE) {
    if(viewport.onMouseLeave) viewport.onMouseLeave();
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    if(viewport.onMousePress) switch(msg) {
    case WM_LBUTTONDOWN: viewport.onMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: viewport.onMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: viewport.onMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    if(viewport.onMouseRelease) switch(msg) {
    case WM_LBUTTONUP: viewport.onMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: viewport.onMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: viewport.onMouseRelease(Mouse::Button::Right); break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

uintptr_t pViewport::handle() {
  return (uintptr_t)hwnd;
}

void pViewport::constructor() {
  hwnd = CreateWindow(L"phoenix_viewport", L"", WS_CHILD | WS_DISABLED, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&viewport);
  synchronize();
}

void pViewport::destructor() {
  DestroyWindow(hwnd);
}

void pViewport::orphan() {
  destructor();
  constructor();
}

}
