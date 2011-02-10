namespace phoenix {

struct Window;

struct Object {
  Object();
  Object& operator=(const Object&) = delete;
  Object(const Object&) = delete;
//private:
  virtual void unused();
  struct Data;
  Data *object;
};

struct Geometry {
  signed x, y;
  unsigned width, height;
  inline Geometry() : x(0), y(0), width(0), height(0) {}
  inline Geometry(signed x, signed y, unsigned width, unsigned height) : x(x), y(y), width(width), height(height) {}
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

struct Menu;
struct Layout;
struct Widget;

struct Window : Object {
  static Window None;
  nall::function<bool ()> onClose;
  nall::function<void ()> onMove;
  nall::function<void ()> onSize;
  void append(Menu &menu);
  void setLayout(Layout &layout);
  void setResizable(bool resizable = true);
  bool focused();
  void setFocused();
  Geometry frameGeometry();
  Geometry geometry();
  void setFrameGeometry(signed x, signed y, unsigned width, unsigned height);
  void setGeometry(signed x, signed y, unsigned width, unsigned height);
  void setDefaultFont(Font &font);
  void setFont(Font &font);
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setTitle(const nall::string &text);
  void setStatusText(const nall::string &text);
  void setVisible(bool visible = true);
  void setMenuVisible(bool visible = true);
  void setStatusVisible(bool visible = true);
  bool fullscreen();
  void setFullscreen(bool fullscreen = true);
  Window();
//private:
  struct Data;
  Data *window;
  void updateFrameGeometry();
};

struct Action : Object {
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  Action();
//private:
  struct Data;
  Data *action;
};

struct Menu : Action {
  void append(Action &action);
  void setText(const nall::string &text);
  Menu();
};

struct MenuSeparator : Action {
  MenuSeparator();
};

struct MenuItem : Action {
  nall::function<void ()> onTick;
  void setText(const nall::string &text);
  MenuItem();
};

struct MenuCheckItem : Action {
  nall::function<void ()> onTick;
  void setText(const nall::string &text);
  bool checked();
  void setChecked(bool checked = true);
  MenuCheckItem();
};

struct MenuRadioItem : Action {
  nall::function<void ()> onTick;
  void setParent(MenuRadioItem &parent);
  void setText(const nall::string &text);
  bool checked();
  void setChecked();
  MenuRadioItem();
};

struct Layout : Object {
  virtual void setParent(Window &parent);
  virtual void setGeometry(Geometry &geometry) = 0;
  virtual void append(Widget &widget);
  Layout();
//private:
  struct Data;
  Data *layout;
};

struct Widget : Object {
  virtual Geometry geometry();
  virtual void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
  virtual void setFont(Font &font);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  virtual bool focused();
  virtual void setFocused();
  Widget();
//private:
  struct Data;
  Data *widget;
};

struct Button : Widget {
  nall::function<void ()> onTick;
  void setText(const nall::string &text);
  Button();
};

struct Canvas : Widget {
  void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
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
  void setText(const nall::string &text);
  bool checked();
  void setChecked(bool checked = true);
  CheckBox();
};

struct ComboBox : Widget {
  nall::function<void ()> onChange;
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
  void setFocused();
  void setEditable(bool editable = true);
  void setWordWrap(bool wordWrap = true);
  nall::string text();
  void setText(const nall::string &text);
  void setCursorPosition(unsigned position);
  EditBox();
};

struct HexEditor : Widget {
  nall::function<uint8_t (unsigned)> onRead;
  nall::function<void (unsigned, uint8_t)> onWrite;
  void setSize(unsigned size);
  void setOffset(unsigned offset);
  void setColumns(unsigned columns);
  void setRows(unsigned rows);
  void update();
  HexEditor();
//private:
  struct Data;
  Data *hexEditor;
  bool keyPress(unsigned scancode);
  void scroll(unsigned position);
  void setScroll();
  void updateScroll();
  unsigned cursorPosition();
  void setCursorPosition(unsigned position);
};

struct HorizontalSlider : Widget {
  nall::function<void ()> onChange;
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);
  HorizontalSlider();
};

struct Label : Widget {
  void setText(const nall::string &text);
  Label();
};

struct ListBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  nall::function<void (unsigned)> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void setFocused();
  void setHeaderText(const nall::string &text);
  void setHeaderVisible(bool headerVisible = true);
  void setCheckable(bool checkable = true);
  void setFont(Font &font);
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
private:
  void create(const nall::string &text);
};

struct ProgressBar : Widget {
  void setPosition(unsigned position);
  ProgressBar();
};

struct RadioBox : Widget {
  nall::function<void ()> onTick;
  void setParent(RadioBox &parent);
  void setText(const nall::string &text);
  bool checked();
  void setChecked();
  RadioBox();
};

struct TextBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  void setEditable(bool editable = true);
  nall::string text();
  void setText(const nall::string &text);
  TextBox();
};

struct VerticalSlider : Widget {
  nall::function<void ()> onChange;
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);
  VerticalSlider();
};

struct Viewport : Widget {
  uintptr_t handle();
  Viewport();
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
};

}
