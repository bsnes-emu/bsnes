namespace phoenix {

struct Window;

struct Object {
  Object();
  Object& operator=(const Object&) = delete;
  Object(const Object&) = delete;
//private:
  struct Data;
  Data *object;
private:
  virtual void unused();
};

struct Geometry {
  unsigned x, y;
  unsigned width, height;
  inline Geometry() : x(0), y(0), width(0), height(0) {}
  inline Geometry(unsigned x, unsigned y, unsigned width, unsigned height) : x(x), y(y), width(width), height(height) {}
};

struct Font : Object {
  enum class Style : unsigned {
    None = 0,
    Bold = 1,
    Italic = 2,
  };
  bool create(const nall::string &name, unsigned size, Font::Style style = Style::None);
  Font();
  ~Font();
//private:
  struct Data;
  Data *font;
};

inline Font::Style operator|(Font::Style a, Font::Style b) { return (Font::Style)((unsigned)a | (unsigned)b); }
inline Font::Style operator&(Font::Style a, Font::Style b) { return (Font::Style)((unsigned)a & (unsigned)b); }

struct Action : Object {
  virtual bool enabled() = 0;
  virtual void setEnabled(bool enabled = true) = 0;
  Action();
//private:
  struct Data;
  Data *action;
};

struct Menu : Action {
  void create(Window &parent, const nall::string &text);
  void create(Menu &parent, const nall::string &text);
  bool enabled();
  void setEnabled(bool enabled = true);
};

struct MenuSeparator : Action {
  void create(Menu &parent);
  bool enabled();
  void setEnabled(bool enabled = true);
};

struct MenuItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  bool enabled();
  void setEnabled(bool enabled = true);
};

struct MenuCheckItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  bool enabled();
  void setEnabled(bool enabled = true);
  bool checked();
  void setChecked(bool checked = true);
};

struct MenuRadioItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  void create(MenuRadioItem &parent, const nall::string &text);
  bool enabled();
  void setEnabled(bool enabled = true);
  bool checked();
  void setChecked();
};

struct Window;
struct Layout;

struct Widget : Object {
  virtual void setParent(Layout &parent) {}

  virtual void setFont(Font &font);
  virtual void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  bool focused();
  void setFocused();
  Widget();
//private:
  struct Data;
  Data *widget;
};

struct Window : Widget {
  nall::function<bool ()> onClose;
  void create(unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void setLayout(Layout &layout);
  void setDefaultFont(Font &font);
  void setFont(Font &font);
  Geometry geometry();
  void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setTitle(const nall::string &text);
  void setStatusText(const nall::string &text);
  void setMenuVisible(bool visible = true);
  void setStatusVisible(bool visible = true);
  bool fullscreen();
  void setFullscreen(bool fullscreen = true);
  Window();
//private:
  struct Data;
  Data *window;
  static Window None;
  void resize(unsigned width, unsigned height);
};

struct Layout : Widget {
  virtual void create(Window &parent) = 0;
  Layout();
//private:
  struct Data;
  Data *layout;
};

struct FixedLayout : Layout {
  void append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height);
  void create(Window &parent);
  FixedLayout();
//private:
  struct Data;
  Data *fixedLayout;
};

struct Button : Widget {
  nall::function<void ()> onTick;
  void setParent(Layout &parent);
  void setText(const nall::string &text);
  Button();
};

struct Canvas : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
  uint32_t* buffer();
  void redraw();
  Canvas();
  ~Canvas();
//private:
  struct Data;
  Data *canvas;
};

struct CheckBox : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  bool checked();
  void setChecked(bool checked = true);
};

struct ComboBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void reset();
  void addItem(const nall::string &text);
  unsigned selection();
  void setSelection(unsigned item);
  ComboBox();
//private:
  struct Data;
  Data *comboBox;
};

struct EditBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  nall::string getText();
  void setText(const nall::string &text);
  void setCursorPosition(unsigned position);
  void setEditable(bool editable = true);
  void setWordWrap(bool wordWrap = true);
  EditBox();
//private:
  struct Data;
  Data *editBox;
};

struct HexEditor : Widget {
  nall::function<uint8_t (unsigned)> onRead;
  nall::function<void (unsigned, uint8_t)> onWrite;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
  void setSize(unsigned size);
  void setOffset(unsigned offset);
  void setColumns(unsigned columns);
  void setRows(unsigned rows);
  void update();
  HexEditor();
  ~HexEditor();
//private:
  struct Data;
  Data *hexEditor;
  bool keyPress(unsigned scancode);
};

struct HorizontalSlider : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length);
  unsigned position();
  void setPosition(unsigned position);
  HorizontalSlider();
//private:
  struct Data;
  Data *horizontalSlider;
};

struct Label : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void setText(const nall::string &text);
};

struct ListBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  nall::function<void (unsigned)> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void setHeaderVisible(bool headerVisible = true);
  void setCheckable(bool checkable = true);
  void reset();
  void resizeColumnsToContent();
  void addItem(const nall::string &text);
  void setItem(unsigned row, const nall::string &text);
  bool checked(unsigned row);
  void setChecked(unsigned row, bool checked = true);
  nall::optional<unsigned> selection();
  void setSelection(unsigned row);
  ListBox();
//private:
  struct Data;
  Data *listBox;
};

struct ProgressBar : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
  unsigned position();
  void setPosition(unsigned position);
};

struct RadioBox : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  bool checked();
  void setChecked();
  RadioBox();
//private:
  struct Data;
  Data *radioBox;
};

struct TextBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  nall::string text();
  void setText(const nall::string &text);
  void setEditable(bool editable = true);
};

struct VerticalSlider : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length);
  unsigned position();
  void setPosition(unsigned position);
  VerticalSlider();
//private:
  struct Data;
  Data *verticalSlider;
};

struct Viewport : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
  uintptr_t handle();
};

struct MessageWindow : Object {
  enum class Buttons : unsigned {
    Ok,
    OkCancel,
    YesNo,
  };
  enum class Response : unsigned {
    Ok,
    Cancel,
    Yes,
    No,
  };
  static Response information(Window &parent, const nall::string &text, Buttons = Buttons::Ok);
  static Response question(Window &parent, const nall::string &text, Buttons = Buttons::YesNo);
  static Response warning(Window &parent, const nall::string &text, Buttons = Buttons::Ok);
  static Response critical(Window &parent, const nall::string &text, Buttons = Buttons::Ok);
};

struct OS : Object {
  static bool pending();
  static void run();
  static void main();
  static void quit();
  static unsigned desktopWidth();
  static unsigned desktopHeight();
  static nall::string folderSelect(Window &parent, const nall::string &path = "");
  static nall::string fileOpen(Window &parent, const nall::string &filter, const nall::string &path = "");
  static nall::string fileSave(Window &parent, const nall::string &filter, const nall::string &path = "");
//private:
  static void initialize();
  struct Data;
  static Data *os;
  static Object* findObject(unsigned id);
  friend class Object;
};

};
