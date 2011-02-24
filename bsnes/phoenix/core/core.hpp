struct Font;
struct Window;
struct Menu;
struct Layout;
struct Widget;

struct pOS;
struct pFont;
struct pWindow;
struct pAction;
struct pMenu;
struct pMenuSeparator;
struct pMenuItem;
struct pMenuCheckItem;
struct pMenuRadioItem;
struct pLayout;
struct pWidget;
struct pButton;
struct pCheckBox;
struct pComboBox;
struct pHexEdit;
struct pHorizontalSlider;
struct pLabel;
struct pLineEdit;
struct pListView;
struct pProgressBar;
struct pRadioBox;
struct pTextEdit;
struct pVerticalSlider;
struct pViewport;

struct Geometry {
  signed x, y;
  unsigned width, height;
  inline Geometry() : x(0), y(0), width(0), height(0) {}
  inline Geometry(signed x, signed y, unsigned width, unsigned height) : x(x), y(y), width(width), height(height) {}
};

struct Object {
  Object();
  Object& operator=(const Object&) = delete;
  Object(const Object&) = delete;
  virtual void unused() {}  //allows dynamic_cast<> on Object
};

struct OS : Object {
  static unsigned desktopWidth();
  static unsigned desktopHeight();
  template<typename... Args> static nall::string fileLoad(Window &parent, const nall::string &path, const Args&... args) { return fileLoad_(parent, path, { args... }); }
  template<typename... Args> static nall::string fileSave(Window &parent, const nall::string &path, const Args&... args) { return fileSave_(parent, path, { args... }); }
  static nall::string folderSelect(Window &parent, const nall::string &path);
  static void main();
  static bool pending();
  static void process();
  static void quit();

  OS();
  static void initialize();

private:
  static nall::string fileLoad_(Window &parent, const nall::string &path, const nall::lstring& filter);
  static nall::string fileSave_(Window &parent, const nall::string &path, const nall::lstring& filter);
};

struct Font : Object {
  void setBold(bool bold = true);
  void setFamily(const nall::string &family);
  void setItalic(bool italic = true);
  void setSize(unsigned size);
  void setUnderline(bool underline = true);

  Font();
  struct State;
  State &state;
  pFont &p;
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

struct Window : Object {
  static Window None;
  nall::function<void ()> onClose;
  nall::function<void ()> onMove;
  nall::function<void ()> onSize;

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Geometry frameGeometry();
  bool focused();
  Geometry geometry();
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setFrameGeometry(const Geometry &geometry);
  void setFocused();
  void setFullScreen(bool fullScreen = true);
  void setGeometry(const Geometry &geometry);
  void setMenuFont(Font &font);
  void setMenuVisible(bool visible = true);
  void setResizable(bool resizable = true);
  void setStatusFont(Font &font);
  void setStatusText(const nall::string &text);
  void setStatusVisible(bool visible = true);
  void setTitle(const nall::string &text);
  void setVisible(bool visible = true);
  void setWidgetFont(Font &font);

  Window();
  struct State;
  State &state;
  pWindow &p;
};

struct Action : Object {
  void setEnabled(bool enabled = true);
  void setVisible(bool visible = true);

  Action(pAction &p);
  struct State;
  State &state;
  pAction &p;
};

struct Menu : private nall::base_from_member<pMenu&>, Action {
  void append(Action &action);
  void setText(const nall::string &text);

  Menu();
  struct State;
  State &state;
  pMenu &p;
};

struct MenuSeparator : private nall::base_from_member<pMenuSeparator&>, Action {
  MenuSeparator();
  pMenuSeparator &p;
};

struct MenuItem : private nall::base_from_member<pMenuItem&>, Action {
  nall::function<void ()> onTick;

  void setText(const nall::string &text);

  MenuItem();
  struct State;
  State &state;
  pMenuItem &p;
};

struct MenuCheckItem : private nall::base_from_member<pMenuCheckItem&>, Action {
  nall::function<void ()> onTick;

  bool checked();
  void setChecked(bool checked = true);
  void setText(const nall::string &text);

  MenuCheckItem();
  struct State;
  State &state;
  pMenuCheckItem &p;
};

struct MenuRadioItem : private nall::base_from_member<pMenuRadioItem&>, Action {
  template<typename... Args> static void group(Args&... args) { group_({ args... }); }

  nall::function<void ()> onTick;

  bool checked();
  void setChecked();
  void setText(const nall::string &text);

  MenuRadioItem();
  struct State;
  State &state;
  pMenuRadioItem &p;

private:
  static void group_(const nall::reference_array<MenuRadioItem&> &list);
};

struct Layout : Object {
  virtual void setGeometry(Geometry &geometry) = 0;
  virtual void setParent(Window &parent) = 0;
  virtual void setVisible(bool visible = true) = 0;
};

struct Widget : Object {
  bool enabled();
  void setEnabled(bool enabled = true);
  void setFocused();
  void setFont(Font &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible = true);

  Widget();
  Widget(pWidget &p);
  struct State;
  State &state;
  pWidget &p;
};

struct Button : private nall::base_from_member<pButton&>, Widget {
  nall::function<void ()> onTick;

  void setText(const nall::string &text);

  Button();
  struct State;
  State &state;
  pButton &p;
};

struct CheckBox : private nall::base_from_member<pCheckBox&>, Widget {
  nall::function<void ()> onTick;

  bool checked();
  void setChecked(bool checked = true);
  void setText(const nall::string &text);

  CheckBox();
  struct State;
  State &state;
  pCheckBox &p;
};

struct ComboBox : private nall::base_from_member<pComboBox&>, Widget {
  nall::function<void ()> onChange;

  void append(const nall::string &text);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  ComboBox();
  struct State;
  State &state;
  pComboBox &p;
};

struct HexEdit : private nall::base_from_member<pHexEdit&>, Widget {
  nall::function<uint8_t (unsigned)> onRead;
  nall::function<void (unsigned, uint8_t)> onWrite;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  HexEdit();
  struct State;
  State &state;
  pHexEdit &p;
};

struct HorizontalSlider : private nall::base_from_member<pHorizontalSlider&>, Widget {
  nall::function<void ()> onChange;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  HorizontalSlider();
  struct State;
  State &state;
  pHorizontalSlider &p;
};

struct Label : private nall::base_from_member<pLabel&>, Widget {
  void setText(const nall::string &text);

  Label();
  struct State;
  State &state;
  pLabel &p;
};

struct LineEdit : private nall::base_from_member<pLineEdit&>, Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;

  void setEditable(bool editable = true);
  void setText(const nall::string &text);
  nall::string text();

  LineEdit();
  struct State;
  State &state;
  pLineEdit &p;
};

struct ListView : private nall::base_from_member<pListView&>, Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  nall::function<void (unsigned)> onTick;

  template<typename... Args> void append(const Args&... args) { append_({ args... }); }
  void autosizeColumns();
  bool checked(unsigned row);
  template<typename... Args> void modify(unsigned row, const Args&... args) { modify_(row, { args... }); }
  void modify(unsigned row, unsigned column, const nall::string &text);
  void reset();
  nall::optional<unsigned> selection();
  void setCheckable(bool checkable = true);
  void setChecked(unsigned row, bool checked = true);
  template<typename... Args> void setHeaderText(const Args&... args) { setHeaderText_({ args... }); }
  void setHeaderVisible(bool visible = true);
  void setSelection(unsigned row);

  ListView();
  struct State;
  State &state;
  pListView &p;

private:
  void append_(const nall::lstring &list);
  void modify_(unsigned row, const nall::lstring &list);
  void setHeaderText_(const nall::lstring &list);
};

struct ProgressBar : private nall::base_from_member<pProgressBar&>, Widget {
  void setPosition(unsigned position);

  ProgressBar();
  struct State;
  State &state;
  pProgressBar &p;
};

struct RadioBox : private nall::base_from_member<pRadioBox&>, Widget {
  template<typename... Args> static void group(Args&... args) { group_({ args... }); }

  nall::function<void ()> onTick;

  bool checked();
  void setChecked();
  void setText(const nall::string &text);

  RadioBox();
  struct State;
  State &state;
  pRadioBox &p;

private:
  static void group_(const nall::reference_array<RadioBox&> &list);
};

struct TextEdit : private nall::base_from_member<pTextEdit&>, Widget {
  nall::function<void ()> onChange;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable = true);
  void setText(const nall::string &text);
  void setWordWrap(bool wordWrap = true);
  nall::string text();

  TextEdit();
  struct State;
  State &state;
  pTextEdit &p;
};

struct VerticalSlider : private nall::base_from_member<pVerticalSlider&>, Widget {
  nall::function<void ()> onChange;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  VerticalSlider();
  struct State;
  State &state;
  pVerticalSlider &p;
};

struct Viewport : private nall::base_from_member<pViewport&>, Widget {
  uintptr_t handle();

  Viewport();
  pViewport &p;
};

#include "layout/fixed-layout.hpp"
#include "layout/horizontal-layout.hpp"
#include "layout/vertical-layout.hpp"
