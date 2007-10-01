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

  virtual void show();
  virtual void hide();
  virtual void show(bool state);
  virtual bool visible();

  void enable();
  void disable();
  void enable(bool state);
  bool enabled();

//private:
Window *owner;
HWND hwnd;
uint id;

public:
  Control() : owner(0), hwnd(0), id(0), type(ControlType::Invalid) {}
};

class ControlGroup { public:
  uint count() { return list.size(); }
  void add(Control &control) { list[list.size()] = &control; }
  void reset() { list.reset(); }
  ControlGroup &operator=(ControlGroup &source) { list = source.list; return *this; }
  Control &operator[](int index) { return *list[index]; }

//private:
array<Control*> list;
};

//

class MenuBar : public Control { public:
  void create(Window &owner);
  void finish();

  void show();
  void hide();
  void show(bool state);
  bool visible();

//private:
HMENU group;
};

class MenuGroup : public Control { public:
  void create(MenuBar &owner, const char *caption);
  void create(MenuGroup &owner, const char *caption);
  void finish();

//private:
HMENU parent, group;
char *caption;
  ~MenuGroup() { safe_free(caption); }
};

class MenuItem : public Control { public:
  void create(MenuGroup &owner, const char *caption);
};

class MenuCheckItem : public Control { public:
  void create(MenuGroup &owner, const char *caption);
  void check();
  void uncheck();
  void check(bool state);
  bool checked();

//private:
HMENU parent;
};

class MenuRadioItem : public Control { public:
  void create(MenuGroup &owner, ControlGroup &list, const char *caption);
  void check();
  bool checked();

//private:
HMENU parent;
ControlGroup group;
};

class MenuSeparator : public Control { public:
  void create(MenuGroup &owner);
};

//

class Panel : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void attach(Window &window);
  void detach();

//private:
Window *attached;
  Panel() : attached(0) {}
};

class Container : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void set_background_color(uint8 r, uint8 g, uint8 b);

//platform-dependent:
  HWND handle();

//private:
HBRUSH background;
  Container();
};

class Canvas : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void redraw();

//private:
BITMAPINFO bmi;
uint32 *buffer;
uint pitch, _w, _h;
  void blit();
  Canvas();
  ~Canvas();
};

class Frame : public Control { public:
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
};

class Label : public Control { public:
enum { ideal_height = 16 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void set_text(const char *str, ...);
};

class Button : public Control { public:
enum { ideal_height = 21 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
};

class Checkbox : public Control { public:
enum { ideal_height = 15 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void check();
  void uncheck();
  void check(bool state);
  bool checked();
};

class Radiobox : public Control { public:
enum { ideal_height = 15 };
  void create(Window &owner, ControlGroup &list, uint style, uint x, uint y, uint width, uint height, const char *caption = "");
  void check();
  bool checked();

//private:
ControlGroup group;
};

class Editbox : public Control { public:
enum { ideal_height = 21 };
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
  void set_text(const char *str, ...);
  uint get_text(char *str, uint length);

//private:
bool multiline;
bool readonly;
bool vscroll;
bool hscroll;
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
  void add_item(const char *data, ...);
  void set_item(uint index, const char *data, ...);
  int  get_selection();
  void set_selection(int index);
  void reset();

//private:
bool header;
uint column_count;
};

class Combobox : public Control { public:
enum { ideal_height = 21 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void add_item(const char *data);
  void set_selection(int index);
  int  get_selection();
  void reset();
};

class Progressbar : public Control { public:
enum { ideal_height = 30 };
  void create(Window &owner, uint style, uint x, uint y, uint width, uint height);
  void set_progress(uint progress);
  uint get_progress();
};

class Slider : public Control { public:
enum { ideal_height = 25 };
enum Style {
  Horizontal = 0,
  Vertical   = 1,
};

  void create(Window &owner, uint style, uint x, uint y, uint width, uint height, uint range);
  void set_position(uint position);
  uint get_position();

//private:
bool orientation;
};
