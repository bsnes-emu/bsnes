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

struct Widget::Data {
  Window *parent;
};

struct Window::Data {
  Font *defaultFont;
};

struct Canvas::Data {
  uint32_t *bufferRGB;
  uint32_t *bufferBGR;
  unsigned pitch;
};

struct ListBox::Data {
  GtkListStore *store;
  struct GtkColumn {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *label;
  };
  linear_vector<GtkColumn> column;
  signed selection;
};

struct OS::Data {
};

void Object::unused() {
}

Object::Object() {
  object = new Object::Data;
  object->locked = false;
}
