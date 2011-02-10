void Canvas::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  canvas->buffer = new uint32_t[width * height]();
  canvas->pitch = width * sizeof(uint32_t);
  canvas->width = width;
  canvas->height = height;
  memset(&canvas->bmi, 0, sizeof(BITMAPINFO));
  canvas->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  canvas->bmi.bmiHeader.biPlanes = 1;
  canvas->bmi.bmiHeader.biBitCount = 32;
  canvas->bmi.bmiHeader.biCompression = BI_RGB;
  canvas->bmi.bmiHeader.biWidth = width;
  canvas->bmi.bmiHeader.biHeight = -height;  //GDI stores bitmaps upside down; negative height flips bitmap
  canvas->bmi.bmiHeader.biSizeImage = canvas->pitch * canvas->height;

  widget->window = CreateWindow(
    L"phoenix_canvas", L"",
    WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    parent.window->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}

uint32_t* Canvas::buffer() {
  return canvas->buffer;
}

void Canvas::redraw() {
  PAINTSTRUCT ps;
  BeginPaint(widget->window, &ps);
  SetDIBitsToDevice(ps.hdc, 0, 0, canvas->width, canvas->height, 0, 0, 0, canvas->height, (void*)canvas->buffer, &canvas->bmi, DIB_RGB_COLORS);
  EndPaint(widget->window, &ps);
  InvalidateRect(widget->window, 0, false);
}

Canvas::Canvas() {
  canvas = new Canvas::Data;
  canvas->buffer = 0;
}

Canvas::~Canvas() {
  delete[] canvas->buffer;
  delete canvas;
}

static LRESULT CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
    case WM_PAINT: {
      Object *object_ptr = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if(object_ptr) {
        if(dynamic_cast<Canvas*>(object_ptr)) {
          Canvas &canvas = (Canvas&)*object_ptr;
          canvas.redraw();
        }
      }
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
