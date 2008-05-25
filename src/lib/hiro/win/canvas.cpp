void pCanvas::create(uint style, uint width, uint height) {
  hwnd = CreateWindow(L"hiro_window", L"", WS_CHILD,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
  resize(width, height);
  ShowWindow(hwnd, SW_NORMAL);
}

void pCanvas::redraw() {
}

uint32_t* pCanvas::buffer() {
  return ibuffer;
}

pCanvas::pCanvas(Canvas &self_) : pFormControl(self_), self(self_) {
  ibuffer = 0;
  ipitch = 0;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
}

pCanvas::~pCanvas() {
  if(ibuffer) free(ibuffer);
}

/* internal */

void pCanvas::blit() {
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  SetDIBitsToDevice(ps.hdc, 0, 0, iwidth, iheight, 0, 0, 0, iheight, (void*)ibuffer, &bmi, DIB_RGB_COLORS);
  EndPaint(hwnd, &ps);
}

void pCanvas::resize(uint width, uint height) {
  if(ibuffer) free(ibuffer);

  ipitch = width * sizeof(uint32_t);
  iwidth = width;
  iheight = height;
  ibuffer = (uint32_t*)malloc(ipitch * height);
  memset(ibuffer, 0, ipitch * height);

  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height; //use negative height to tell GDI not to flip bitmap vertically
  bmi.bmiHeader.biSizeImage = ipitch * height;

  pFormControl::resize(width, height);
}
