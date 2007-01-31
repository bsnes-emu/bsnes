class Control { public:
Window *owner;
GtkWidget *widget;
uint id;
uint type;

enum {
  Invalid,
  MenuGroup,
  MenuItem,
  MenuCheckItem,
  MenuRadioItem,
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

  void move(uint x, uint y);
  void resize(uint width, uint height);

  void show();
  void hide();
  void show(bool state);
  bool visible();

  void enable();
  void disable();
  void enable(bool state);
  bool enabled();

  Control() : owner(0), widget(0), id(0), type(Invalid) {}
};

class ControlGroup { public:
array<Control*> list;
  uint count() { return list.size(); }
  void add(Control &control) { list[list.size()] = &control; }
  void reset() { list.reset(); }
  ControlGroup &operator=(ControlGroup &source) { list = source.list; return *this; }
  Control &operator[](int index) { return *list[index]; }
};

class MenuGroup : public Control { public:
bool master;
GtkWidget *item;
  MenuGroup &create(const char *caption);
  MenuGroup() : master(false) {}
};
class MenuItem : public Control { public:
  MenuItem &create(const char *caption);
};
class MenuCheckItem : public Control { public:
  MenuCheckItem &create(const char *caption);
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
};
class MenuRadioItem : public Control { public:
  MenuRadioItem &create(ControlGroup &list, const char *caption);
  void check();
  bool checked();
};
class Container : public Control { public:
  Container &create(const char *style, uint width, uint height);
  void attach(Control &control, uint x, uint y);
};
class Canvas : public Control { public:
  Canvas &create(const char *style, uint width, uint height);
  WindowHandle handle();
  void set_background_color(uint8 r, uint8 g, uint8 b);
};
class Frame : public Control { public:
  Frame &create(const char *style, uint width, uint height, const char *caption = "");
};
class Label : public Control { public:
  Label &create(const char *style, uint width, uint height, const char *caption = "");
  void set_text(const char *str, ...);
  uint get_text(char *str, uint length);
};
class Button : public Control { public:
  Button &create(const char *style, uint width, uint height, const char *caption = "");
  void set_text(const char *str, ...);
  uint get_text(char *str, uint length);
};
class Checkbox : public Control { public:
  Checkbox &create(const char *style, uint width, uint height, const char *caption = "");
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
};
class Radiobox : public Control { public:
  Radiobox &create(ControlGroup &group, const char *style, uint width, uint height, const char *caption = "");
  void check();
  bool checked();
};
class Editbox : public Control { public:
GtkTextBuffer *buffer;
bool multiline;
  Editbox &create(const char *style, uint width, uint height, const char *caption = "");
  void set_text(const char *str, ...);
  uint get_text(char *str, uint length);
};
class Listbox : public Control { public:
GtkListStore *store;
GtkCellRenderer *renderer;
GtkTreeViewColumn *column;
array<GtkTreeViewColumn*> column_list;
GtkTreeIter iter;
  Listbox &create(const char *style, uint width, uint height, const char *columns = "", const char *data = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *data);
  int  get_selection();
  void set_selection(int index);
  void reset();
};
class Combobox : public Control { public:
uint counter;
  Combobox &create(const char *style, uint width, uint height, const char *caption = "");
  void add_item(const char *data);
  int  get_selection();
  void set_selection(int index);
  void reset();
};
class Progressbar : public Control { public:
  Progressbar &create(const char *style, uint width, uint height);
  void set_progress(uint progress);
  uint get_progress();
};
class Slider : public Control { public:
bool orientation;
  Slider &create(const char *style, uint width, uint height, uint min, uint max);
  void set_position(uint position);
  uint get_position();
};
