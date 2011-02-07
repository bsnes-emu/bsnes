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

  Data() {
    locked = false;
    widget = 0;
    subWidget = 0;
    menuContainer = 0;
    formContainer = 0;
    statusContainer = 0;
    menu = 0;
    status = 0;
    parentMenu = 0;
    parentWindow = 0;
    textBuffer = 0;
    position = 0;
  }
};

struct Font::Data {
  PangoFontDescription *font;

  Data() {
    font = 0;
  }
};

struct Action::Data {
  Font *font;

  Data() {
    font = 0;
  }
};

struct Window::Data {
  Layout *layout;
  Font *defaultFont;
  bool resizable;
  bool fullscreen;
  unsigned x, y;
  unsigned width, height;

  Data() {
    layout = 0;
    defaultFont = 0;
    resizable = true;
    fullscreen = false;
    x = y = 0;
    width = height = 0;
  }
};

struct Widget::Data {
  Window *parent;
  Font *font;
  unsigned x, y;
  unsigned width, height;

  Data() {
    parent = 0;
    font = 0;
    x = y = 0;
    width = height = 0;
  }
};

struct Layout::Data {
  Window *parent;

  Data() {
    parent = 0;
  }
};

struct ComboBox::Data {
  unsigned counter;

  Data() {
    counter = 0;
  }
};

struct Canvas::Data {
  uint32_t *bufferRGB;
  uint32_t *bufferBGR;
  unsigned pitch;

  Data() {
    bufferRGB = 0;
    bufferBGR = 0;
    pitch = 0;
  }
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

  Data() {
    container = 0;
    widget = 0;
    scroll = 0;
    cursor = 0;
    size = 0;
    offset = 0;
    columns = 0;
    rows = 0;
  }
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

  Data() {
    store = 0;
    checkable = false;
    selection = -1;
  }
};

void Object::unused() {
}

Object::Object() {
  OS::initialize();
  object = new Object::Data;
  object->locked = false;
}
