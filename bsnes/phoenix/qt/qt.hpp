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
  virtual bool visible() = 0;
  virtual void setVisible(bool visible = true) = 0;
  virtual bool enabled() = 0;
  virtual void setEnabled(bool enabled = true) = 0;
};

struct Menu : Action {
  void create(Window &parent, const nall::string &text);
  void create(Menu &parent, const nall::string &text);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  Menu();
//private:
  struct Data;
  Data *menu;
};

struct MenuSeparator : Action {
  void create(Menu &parent);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  MenuSeparator();
//private:
  struct Data;
  Data *menuSeparator;
};

struct MenuItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  MenuItem();
//private:
  struct Data;
  Data *menuItem;
};

struct MenuCheckItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  bool checked();
  void setChecked(bool checked = true);
  MenuCheckItem();
//private:
  struct Data;
  Data *menuCheckItem;
};

struct MenuRadioItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const nall::string &text);
  void create(MenuRadioItem &parent, const nall::string &text);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  bool checked();
  void setChecked();
  MenuRadioItem();
//private:
  struct Data;
  Data *menuRadioItem;
};

struct Widget : Object {
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

struct Window : Widget {
  nall::function<bool ()> onClose;
  void create(unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  Geometry geometry();
  void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
  void setDefaultFont(Font &font);
  void setFont(Font &font);
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setTitle(const nall::string &text);
  void setStatusText(const nall::string &text);
  void setMenuVisible(bool visible = true);
  void setStatusVisible(bool visible = true);
  bool focused();
  Window();
//private:
  struct Data;
  Data *window;
  static Window None;
};

struct Button : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  Button();
//private:
  struct Data;
  Data *button;
};

struct Canvas : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
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
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  bool checked();
  void setChecked(bool checked = true);
  CheckBox();
//private:
  struct Data;
  Data *checkBox;
};

struct ComboBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void reset();
  void addItem(const nall::string &text);
  unsigned selection();
  void setSelection(unsigned row);
  ComboBox();
//private:
  struct Data;
  Data *comboBox;
};

struct EditBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const nall::string &text = "");
  void setEditable(bool editable = true);
  void setWordWrap(bool wordWrap = true);
  nall::string text();
  void setText(const nall::string &text);
  void setCursorPosition(unsigned position);
  EditBox();
//private:
  struct Data;
  Data *editBox;
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
  Label();
//private:
  struct Data;
  Data *label;
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
  void setPosition(unsigned position);
  ProgressBar();
//private:
  struct Data;
  Data *progressBar;
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
  void setEditable(bool editable = true);
  nall::string text();
  void setText(const nall::string &text);
  TextBox();
//private:
  struct Data;
  Data *textBox;
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
  Viewport();
//private:
  struct Data;
  Data *viewport;
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
  struct Data;
  static Data *os;
  static void initialize();
};

}
