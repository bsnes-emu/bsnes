static LRESULT CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg == WM_PAINT) {
    Object *object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if(object && dynamic_cast<Canvas*>(object)) {
      Canvas &canvas = (Canvas&)*object;
      canvas.update();
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

uint32_t* pCanvas::buffer() {
  return bufferRGB;
}

void pCanvas::setGeometry(const Geometry &geometry) {
  delete[] bufferRGB;
  bufferRGB = new uint32_t[geometry.width * geometry.height]();
  pWidget::setGeometry(geometry);
  update();
}

void pCanvas::update() {
  RECT rc;
  GetClientRect(hwnd, &rc);
  unsigned width = rc.right, height = rc.bottom;

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
  SetDIBitsToDevice(ps.hdc, 0, 0, width, height, 0, 0, 0, height, (void*)bufferRGB, &bmi, DIB_RGB_COLORS);
  EndPaint(hwnd, &ps);
  InvalidateRect(hwnd, 0, false);
}

void pCanvas::constructor() {
  bufferRGB = new uint32_t[256 * 256]();
  hwnd = CreateWindow(L"phoenix_canvas", L"", WS_CHILD, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&canvas);
  synchronize();
}

void pCanvas::destructor() {
  DestroyWindow(hwnd);
}

void pCanvas::orphan() {
  destructor();
  constructor();
}
