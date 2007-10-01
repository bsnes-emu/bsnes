namespace ControlType {
  enum {
    Invalid,
    MenuBar,
    MenuGroup,
    MenuItem,
    MenuCheckItem,
    MenuRadioItem,
    MenuSeparator,
    Panel,
    Container,
    Canvas,
    Frame,
    Label,
    Button,
    Checkbox,
    Radiobox,
    Editbox,
    Listbox,
    Combobox,
    Progressbar,
    Slider,
  };
};

class Control { public:
uint type;
  void move(uint x, uint y);
  void resize(uint width, uint height);

  void focus();

  void show(bool state);
  void show();
  void hide();
  bool visible();

  void enable(bool state);
  void enable();
  void disable();
  bool enabled();

//protected:
Window *owner;
GtkWidget *widget;
uint id;

public:
  Control() : owner(0), widget(0), id(0), type(ControlType::Invalid) {}
};

class ControlGroup { public:
  uint count() { return list.size(); }
  void add(Control &control) { list[list.size()] = &control; }
  void reset() { list.reset(); }
  ControlGroup &operator=(ControlGroup &source) { list = source.list; return *this; }
  Control &operator[](int index) { return *list[index]; }

private:
array<Control*> list;
};

class MenuBar : public Control { public:
  void create(Window &owner);
  void finish();
};

class MenuGroup : public Control { public:
  void create(MenuBar &owner, const char *caption);
  void create(MenuGroup &owner, const char *caption);
  void finish();

private:
GtkWidget *parent, *item;
};

class MenuItem : public Control { public:
  void create(MenuGroup &owner, const char *caption);

private:
GtkWidget *parent;
};

class MenuCheckItem : public Control { public:
  void create(MenuGroup &owner, const char *caption);
  void check();
  void uncheck();
  void check(bool state);
  bool checked();

private:
GtkWidget *parent;
};

class MenuRadioItem : public Control { public:
  void create(MenuGroup &owner, ControlGroup &list, const char *caption);
  void check();
  bool checked();

private:
GtkWidget *parent;
};

class MenuSeparator : public Control { public:
  void create(MenuGroup &owner);

private:
GtkWidget *parent;
};

class Panel : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void attach(Window &window);
  void detach();

private:
Window *attached;

public:
  Panel() : attached(0) {}
};

class Container : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void set_background_color(uint8 r, uint8 g, uint8 b);

//platform-dependent:
  unsigned long x_handle();
  GtkWidget *handle();
};

class Canvas : public Control { public:
uint32 *buffer;
uint pitch;
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void redraw();
  Canvas();
  ~Canvas();

private:
uint32 *rbuffer;
  friend void libui_canvas_expose(GtkWidget *widget, GdkEventAny *any, Canvas *canvas);
};

class Frame : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
};

class Label : public Control { public:
enum { ideal_height = 18 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void set_text(const char *str);
};

class Button : public Control { public:
enum { ideal_height = 30 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void set_text(const char *str);
};

class Checkbox : public Control { public:
enum { ideal_height = 18 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
};

class Radiobox : public Control { public:
enum { ideal_height = 18 };
  void create(Window &owner, ControlGroup &group, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void check();
  bool checked();
};

class Editbox : public Control { public:
enum { ideal_height = 30 };
enum {
  Multiline = (1 << 1),
  Readonly  = (1 << 2),

  HorizontalScrollAuto   = 0,
  HorizontalScrollAlways = (1 << 3),
  HorizontalScrollNever  = (1 << 4),

  VerticalScrollAuto   = 0,
  VerticalScrollAlways = (1 << 5),
  VerticalScrollNever  = (1 << 6),
};

  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void set_text(const char *str);
  uint get_text(char *str, uint length);

private:
GtkWidget *subwidget;
GtkTextBuffer *buffer;
bool multiline;
};

class Listbox : public Control { public:
enum {
  Header = (1 << 1),

  HorizontalScrollAuto   = 0,
  HorizontalScrollAlways = (1 << 2),
  HorizontalScrollNever  = (1 << 3),

  VerticalScrollAuto   = 0,
  VerticalScrollAlways = (1 << 4),
  VerticalScrollNever  = (1 << 5),
};

  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *columns = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *data);
  void set_item(uint index, const char *data);
  int  get_selection();
  void set_selection(int index);
  void reset();

private:
GtkWidget *subwidget;
GtkListStore *store;
GtkCellRenderer *renderer;
GtkTreeViewColumn *column;
array<GtkTreeViewColumn*> column_list;
GtkTreeIter iter;
};

class Combobox : public Control { public:
enum { ideal_height = 30 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void add_item(const char *data);
  int  get_selection();
  void set_selection(int index);
  void reset();

private:
uint counter;
};

class Progressbar : public Control { public:
enum { ideal_height = 30 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void set_progress(uint progress);
  uint get_progress();
};

class Slider : public Control { public:
enum { ideal_height = 25 };
enum {
  Horizontal = 0,
  Vertical   = 1,
};

  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, uint range);
  void set_position(uint position);
  uint get_position();

private:
bool orientation;
};
