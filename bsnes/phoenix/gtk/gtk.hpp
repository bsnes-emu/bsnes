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
  void setFont(Font &font);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
};

struct Menu : Action {
  void create(Window &parent, const char *text);
  void create(Menu &parent, const char *text);
};

struct MenuSeparator : Action {
  void create(Menu &parent);
};

struct MenuItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const char *text);
};

struct MenuCheckItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const char *text);
  bool checked();
  void setChecked(bool checked = true);
};

struct MenuRadioItem : Action {
  nall::function<void ()> onTick;
  void create(Menu &parent, const char *text);
  void create(MenuRadioItem &parent, const char *text);
  bool checked();
  void setChecked();
private:
  MenuRadioItem *first;
};

struct Widget : Object {
  virtual void setFont(Font &font);
  bool visible();
  void setVisible(bool visible = true);
  bool enabled();
  void setEnabled(bool enabled = true);
  virtual bool focused();
  virtual void setFocused();
};

struct Window : Widget {
  static Window None;
  nall::function<bool ()> onClose;
  void create(unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  bool focused();
  void setFocused();
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
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  bool checked();
  void setChecked(bool checked = true);
};

struct ComboBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void reset();
  void addItem(const char *text);
  unsigned selection();
  void setSelection(unsigned item);
  ComboBox();
private:
  unsigned counter;
};

struct EditBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setFocused();
  void setEditable(bool editable = true);
  void setWordWrap(bool wordWrap = true);
  nall::string text();
  void setText(const char *text);
};

struct HorizontalSlider : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length);
  unsigned position();
  void setPosition(unsigned position);
};

struct Label : Widget {
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setText(const char *text);
};

struct ListBox : Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setFocused();
  void setHeaderVisible(bool headerVisible = true);
  void setFont(Font &font);
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
};

struct RadioBox : Widget {
  nall::function<void ()> onTick;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  bool checked();
  void setChecked();
private:
  RadioBox *first;
};

struct TextBox : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text = "");
  void setEditable(bool editable = true);
  nall::string text();
  void setText(const char *text);
};

struct VerticalSlider : Widget {
  nall::function<void ()> onChange;
  void create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length);
  unsigned position();
  void setPosition(unsigned position);
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
