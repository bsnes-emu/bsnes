struct Object::Data {
  bool locked;
  GtkWidget *widget;
  GtkWidget *subWidget;
  GtkWidget *menuContainer;
  GtkWidget *formContainer;
  GtkWidget *statusContainer;
  GtkWidget *menu;
  GtkWidget *status;
  Menu *parentMenu;
  Window *parentWindow;
  GtkTextBuffer *textBuffer;
  unsigned position;
};

struct Font::Data {
  PangoFontDescription *font;
};

struct Action::Data {
  Font *font;
};

struct Window::Data {
  Font *defaultFont;
  bool isFullscreen;
  unsigned x;
  unsigned y;
  unsigned width;
  unsigned height;
};

struct Widget::Data {
  Window *parent;
};

struct Layout::Data {
};

struct FixedLayout::Data {
  Window *parent;
  GtkWidget *container;

  struct Widgets {
    Widget *widget;
    unsigned x, y;
    unsigned width, height;
  };
  linear_vector<Widgets> widgets;
};

struct Canvas::Data {
  uint32_t *bufferRGB;
  uint32_t *bufferBGR;
  unsigned pitch;
};

struct HexEditor::Data {
  GtkWidget *container;
  GtkWidget *widget;
  GtkWidget *scroll;

  GtkTextMark *cursor;
  unsigned size;
  unsigned offset;
  unsigned columns;
  unsigned rows;
};

struct ListBox::Data {
  GtkListStore *store;
  struct GtkColumn {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *label;
  };
  linear_vector<GtkColumn> column;
  bool checkable;
  signed selection;
};

void Object::unused() {
}

Object::Object() {
  OS::initialize();
  object = new Object::Data;
  object->locked = false;
}
