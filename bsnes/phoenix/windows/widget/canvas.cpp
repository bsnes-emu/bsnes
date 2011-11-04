static LRESULT CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_PAINT) {
    Object *object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if(object && dynamic_cast<Canvas*>(object)) {
      Canvas &canvas = (Canvas&)*object;
      canvas.update();
    }
    return TRUE;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void pCanvas::setSize(const Size &size) {
  delete[] data;
  data = new uint32_t[size.width * size.height];
  memcpy(data, canvas.state.data, size.width * size.height * sizeof(uint32_t));
}

void pCanvas::update() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  unsigned width = canvas.state.width, height = canvas.state.height;  //rc.right, height = rc.bottom;

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
