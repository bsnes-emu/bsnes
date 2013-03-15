namespace phoenix {

static LRESULT CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object *object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  if(!dynamic_cast<Canvas*>(object)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Canvas &canvas = (Canvas&)*object;

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_PAINT) {
    canvas.p.paint();
    return TRUE;
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT tracker = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd };
    TrackMouseEvent(&tracker);
    if(canvas.onMouseMove) canvas.onMouseMove({ (int16_t)LOWORD(lparam), (int16_t)HIWORD(lparam) });
  }

  if(msg == WM_MOUSELEAVE) {
    if(canvas.onMouseLeave) canvas.onMouseLeave();
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    if(canvas.onMousePress) switch(msg) {
    case WM_LBUTTONDOWN: canvas.onMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: canvas.onMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: canvas.onMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    if(canvas.onMouseRelease) switch(msg) {
    case WM_LBUTTONUP: canvas.onMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: canvas.onMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: canvas.onMouseRelease(Mouse::Button::Right); break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void pCanvas::setSize(const Size &size) {
  delete[] data;
  data = new uint32_t[size.width * size.height];
}

void pCanvas::update() {
  memcpy(data, canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  InvalidateRect(hwnd, 0, false);
}

void pCanvas::constructor() {
  data = new uint32_t[canvas.state.width * canvas.state.height];
  memcpy(data, canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  hwnd = CreateWindow(L"phoenix_canvas", L"", WS_CHILD, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&canvas);
  synchronize();
}

void pCanvas::destructor() {
  DestroyWindow(hwnd);
  delete[] data;
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::paint() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  unsigned width = canvas.state.width, height = canvas.state.height;

  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;  //GDI stores bitmaps upside now; negative height flips bitmap
  bmi.bmiHeader.biSizeImage = sizeof(uint32_t) * width * height;

  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  SetDIBitsToDevice(ps.hdc, 0, 0, width, height, 0, 0, 0, height, (void*)data, &bmi, DIB_RGB_COLORS);
  EndPaint(hwnd, &ps);
}

}
