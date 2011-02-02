struct Object::Data {
  unsigned id;
  bool locked;
};

struct Font::Data {
  HFONT font;
};

struct Action::Data {
  Menu *parent;
  HMENU parentMenu;
  HMENU menu;
  MenuRadioItem *radioParent;
  array<MenuRadioItem*> items;
};

struct Widget::Data {
  HWND window;
  HFONT font;
};

struct Window::Data {
  HFONT defaultFont;
  HBRUSH brush;
  COLORREF brushColor;
  HMENU menu;
  HWND status;
  bool isFullscreen;
  unsigned x;
  unsigned y;
  unsigned width;
  unsigned height;
};

struct Canvas::Data {
  uint32_t *buffer;
  BITMAPINFO bmi;
  unsigned pitch;
  unsigned width;
  unsigned height;
};

struct ComboBox::Data {
  unsigned selection;
};

struct EditBox::Data {
  bool wordWrap;
  unsigned x;
  unsigned y;
  unsigned width;
  unsigned height;
};

struct HexEditor::Data {
  LRESULT CALLBACK (*windowProc)(HWND, UINT, LPARAM, WPARAM);
  unsigned size;
  unsigned offset;
  unsigned columns;
  unsigned rows;
};

struct HorizontalSlider::Data {
  unsigned position;
};

struct ListBox::Data {
  unsigned columns;
  bool lostFocus;
};

struct RadioBox::Data {
  Window *parentWindow;
  RadioBox *parent;
  array<RadioBox*> items;
};

struct VerticalSlider::Data {
  unsigned position;
};

struct OS::Data {
  nall::array<Object*> objects;
  HFONT proportionalFont;
  HFONT monospaceFont;
};

void Object::unused() {
}

Object::Object() {
  OS::initialize();
  static unsigned guid = 100;
  object = new Object::Data;
  object->id = guid++;
  object->locked = false;
}
