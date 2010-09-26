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

struct Font : Object {
  enum class Style : unsigned {
    None = 0,
    Bold = 1,
    Italic = 2,
  };
  bool create(const char *name, unsigned size, Font::Style style = Style::None);
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
  void create(Window &parent, const char *text);
  void create(Menu &parent, const char *text);
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
  void create(Menu &parent, const char *text);
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
  void create(Menu &parent, const char *text);
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
  void create(Menu &parent, const char *text);
  void create(MenuRadioItem &parent, const char *text);
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
  bool focused();
  void setFocused();
  Widget();
//private:
  struct Data;
  Data *widget;
};

struct Window : Widget {
  static Window None;
  nall::function<bool ()> onClose;
  void create(unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setGeometry(unsigned x, unsigned y, unsigned width, unsigned height);
  void setDefaultFont(Font &font);
  void setFont(Font &font);
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setTitle(const char *text);
  void setStatusText(const char *text);
  void setMenuVisible(bool visible = true);
  void setStatusVisible(bool visible = true);
  Window();
//private:
  struct Data;
  Data *window;
};

struct Button : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
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
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  bool checked();
  void setChecked(bool checked = true);
  CheckBox();
//private:
  struct Data;
  Data *checkBox;
};

struct ComboBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void reset();
  void addItem(const char *text);
  unsigned selection();
  void setSelection(unsigned row);
  ComboBox();
//private:
  struct Data;
  Data *comboBox;
};

struct EditBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setEditable(bool editable = true);
  void setWordWrap(bool wordWrap = true);
  nall::string text();
  void setText(const char *text);
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
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setText(const char *text);
  Label();
//private:
  struct Data;
  Data *label;
};

struct ListBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setHeaderVisible(bool headerVisible = true);
  void reset();
  void resizeColumnsToContent();
  void addItem(const char *text);
  void setItem(unsigned row, const char *text);
  nall::optional<unsigned> selection();
  void setSelection(unsigned row);
  ListBox();
//private:
  struct Data;
  Data *listBox;
};

struct ProgressBar : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height);
  void setProgress(unsigned progress);
  ProgressBar();
//private:
  struct Data;
  Data *progressBar;
};

struct RadioBox : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  bool checked();
  void setChecked();
  RadioBox();
//private:
  struct Data;
  Data *radioBox;
};

struct TextBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setEditable(bool editable = true);
  nall::string text();
  void setText(const char *text);
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
  static Response information(Window &parent, const char *text, Buttons = Buttons::Ok);
  static Response question(Window &parent, const char *text, Buttons = Buttons::YesNo);
  static Response warning(Window &parent, const char *text, Buttons = Buttons::Ok);
  static Response critical(Window &parent, const char *text, Buttons = Buttons::Ok);
};

struct OS : Object {
  bool pending();
  void run();
  void main();
  void quit();
  unsigned desktopWidth();
  unsigned desktopHeight();
  nall::string folderSelect(Window &parent, const char *path = "");
  nall::string fileOpen(Window &parent, const char *filter, const char *path = "");
  nall::string fileSave(Window &parent, const char *filter, const char *path = "");
//private:
  static OS& handle();
  struct Data;
  Data *os;
private:
  OS();
};

extern OS &os;

}
