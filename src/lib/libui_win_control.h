class Control { public:
Window *owner;
HWND hwnd;
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

  void resize(uint width, uint height);

  void show();
  void hide();
  void show(bool state);
  bool visible();

  void enable();
  void disable();
  void enable(bool state);
  bool enabled();

  Control() : owner(0), hwnd(0), id(0), type(Invalid) {}
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
HMENU group;
char *caption;
  MenuGroup &create(const char *caption);
  MenuGroup() : master(false) {}
  ~MenuGroup() { safe_free(caption); }
};

class MenuItem : public Control { public:
char *caption;
  MenuItem &create(const char *caption);
  ~MenuItem() { safe_free(caption); }
};
class MenuCheckItem : public Control { public:
char *caption;
  MenuCheckItem &create(const char *caption);
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
  ~MenuCheckItem() { safe_free(caption); }
};
class MenuRadioItem : public Control { public:
ControlGroup group;
char *caption;
  MenuRadioItem &create(ControlGroup &list, const char *caption);
  void check();
  bool checked();
  ~MenuRadioItem() { safe_free(caption); }
};
class Container : public Control { public:
  Container &create(const char *style, uint width, uint height);
  void attach(Control &control, uint x, uint y);
  void move(Control &control, uint x, uint y);
};
class Canvas : public Control { public:
HBRUSH background;
  Canvas &create(const char *style, uint width, uint height);
  WindowHandle handle();
  void set_background_color(uint8 r, uint8 g, uint8 b);
  Canvas() : background(0) {}
};
class Frame : public Control { public:
  Frame &create(const char *style, uint width, uint height, const char *caption = "");
};
class Label : public Control { public:
  Label &create(const char *style, uint width, uint height, const char *caption = "");
};
class Button : public Control { public:
  Button &create(const char *style, uint width, uint height, const char *caption = "");
};
class Checkbox : public Control { public:
  Checkbox &create(const char *style, uint width, uint height, const char *caption = "");
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
};
class Radiobox : public Control { public:
ControlGroup group;
  Radiobox &create(ControlGroup &list, const char *style, uint width, uint height, const char *caption = "");
  void check();
  bool checked();
};
class Editbox : public Control { public:
bool multiline;
bool readonly;
bool vscroll;
bool hscroll;
  Editbox &create(const char *style, uint width, uint height, const char *caption = "");
};
class Listbox : public Control { public:
bool header;
uint column_count;
  Listbox &create(const char *style, uint width, uint height, const char *columns = "", const char *data = "");
  void autosize_columns();
  void set_column_width(uint column, uint width);
  void add_item(const char *data);
  int  get_selection();
  void set_selection(int index);
  void reset();
};
class Combobox : public Control { public:
  Combobox &create(const char *style, uint width, uint height, const char *caption = "");
  void add_item(const char *data);
  void set_selection(int index);
  int  get_selection();
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
