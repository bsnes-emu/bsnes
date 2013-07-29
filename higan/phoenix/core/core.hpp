#include <nall/platform.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/function.hpp>
#include <nall/group.hpp>
#include <nall/image.hpp>
#include <nall/map.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>

namespace phoenix {

struct Application;
struct Font;
struct Window;
struct Menu;
struct Sizable;
struct Layout;
struct Widget;

struct pApplication;
struct pFont;
struct pObject;
struct pTimer;
struct pWindow;
struct pAction;
struct pMenu;
struct pSeparator;
struct pItem;
struct pCheckItem;
struct pRadioItem;
struct pSizable;
struct pLayout;
struct pWidget;
struct pButton;
struct pCanvas;
struct pCheckButton;
struct pComboButton;
struct pHexEdit;
struct pHorizontalScroller;
struct pHorizontalSlider;
struct pLabel;
struct pLineEdit;
struct pListView;
struct pProgressBar;
struct pRadioButton;
struct pTextEdit;
struct pVerticalScroller;
struct pVerticalSlider;
struct pViewport;

struct Application {
  static nall::function<void ()> main;

  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();
  static void setName(nall::string name);

  Application() = delete;
  struct State;
  static void initialize();

  struct Windows {
    static nall::function<void ()> onModalBegin;
    static nall::function<void ()> onModalEnd;
  };

  struct Cocoa {
    static nall::function<void ()> onAbout;
    static nall::function<void ()> onActivate;
    static nall::function<void ()> onPreferences;
    static nall::function<void ()> onQuit;
  };
};

enum : unsigned {
  MaximumSize = ~0u,
  MinimumSize =  0u,
};

struct Color {
  uint8_t red, green, blue, alpha;
  uint32_t rgb() const;
  uint32_t rgba() const;
  inline Color() : red(0), green(0), blue(0), alpha(255) {}
  inline Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : red(red), green(green), blue(blue), alpha(alpha) {}
};

struct Position {
  signed x, y;
  inline Position() : x(0), y(0) {}
  template<typename X, typename Y> inline Position(X x, Y y) : x(x), y(y) {}
};

struct Size {
  unsigned width, height;
  inline Size() : width(0), height(0) {}
  template<typename W, typename H> inline Size(W width, H height) : width(width), height(height) {}
};

struct Geometry {
  signed x, y;
  unsigned width, height;
  Position position() const;
  Size size() const;
  nall::string text() const;
  inline Geometry() : x(0), y(0), width(0), height(0) {}
  inline Geometry(Position position, Size size) : x(position.x), y(position.y), width(size.width), height(size.height) {}
  template<typename X, typename Y, typename W, typename H> inline Geometry(X x, Y y, W width, H height) : x(x), y(y), width(width), height(height) {}
  Geometry(nall::string text);
};

enum class Orientation : unsigned { Horizontal, Vertical };

struct Font {
  static nall::string serif(unsigned size = 0, nall::string style = "");
  static nall::string sans(unsigned size = 0, nall::string style = "");
  static nall::string monospace(unsigned size = 0, nall::string style = "");
  static Size size(nall::string font, nall::string text);
  Font() = delete;
};

struct Desktop {
  static Size size();
  static Geometry workspace();
  Desktop() = delete;
};

struct Keyboard {
  #include "keyboard.hpp"
  static bool pressed(Scancode scancode);
  static bool released(Scancode scancode);
  static nall::vector<bool> state();
  Keyboard() = delete;
};

struct Mouse {
  enum class Button : unsigned { Left, Middle, Right };
  static Position position();
  static bool pressed(Button);
  static bool released(Button);
  Mouse() = delete;
};

struct BrowserWindow {
  template<typename... Args> BrowserWindow& setFilters(Args&&... args) { return setFilters_({args...}); }

  nall::string directory();
  nall::string open();
  nall::string save();
  BrowserWindow& setFilters_(const nall::lstring& filters);
  BrowserWindow& setParent(Window& parent);
  BrowserWindow& setPath(nall::string path);
  BrowserWindow& setTitle(nall::string title);

  BrowserWindow();
  ~BrowserWindow();
  struct State;
  State& state;
};

struct MessageWindow {
  enum class Buttons : unsigned {
    Ok,
    OkCancel,
    YesNo,
    YesNoCancel,
  };

  enum class Response : unsigned {
    Ok,
    Cancel,
    Yes,
    No,
  };

  Response error(Buttons = Buttons::Ok);
  Response information(Buttons = Buttons::Ok);
  Response question(Buttons = Buttons::YesNo);
  MessageWindow& setParent(Window& parent);
  MessageWindow& setText(nall::string text);
  MessageWindow& setTitle(nall::string title);
  Response warning(Buttons = Buttons::Ok);

  MessageWindow(nall::string text = "");
  ~MessageWindow();
  struct State;
  State& state;
};

struct Object {
  Object(pObject& p);
  Object& operator=(const Object&) = delete;
  Object(const Object&) = delete;
  virtual ~Object();
  pObject& p;
};

struct Timer : private nall::base_from_member<pTimer&>, Object {
  nall::function<void ()> onActivate;

  void setEnabled(bool enabled = true);
  void setInterval(unsigned milliseconds);

  Timer();
  ~Timer();
  struct State;
  State& state;
  pTimer& p;
};

struct Window : private nall::base_from_member<pWindow&>, Object {
  nall::function<void ()> onClose;
  nall::function<void (nall::lstring)> onDrop;
  nall::function<void (Keyboard::Keycode)> onKeyPress;
  nall::function<void (Keyboard::Keycode)> onKeyRelease;
  nall::function<void ()> onMove;
  nall::function<void ()> onSize;

  static Window& none();

  inline void append() {}
  inline void remove() {}
  template<typename T, typename... Args> void append(T& arg, Args&&... args) { append_(arg); append(args...); }
  template<typename T, typename... Args> void remove(T& arg, Args&&... args) { remove_(arg); remove(args...); }

  void append_(Layout& layout);
  void append_(Menu& menu);
  void append_(Widget& widget);
  Color backgroundColor();
  Geometry frameGeometry();
  Geometry frameMargin();
  bool focused();
  bool fullScreen();
  Geometry geometry();
  void remove_(Layout& layout);
  void remove_(Menu& menu);
  void remove_(Widget& widget);
  void setBackgroundColor(Color color);
  void setDroppable(bool droppable = true);
  void setFrameGeometry(Geometry geometry);
  void setFocused();
  void setFullScreen(bool fullScreen = true);
  void setGeometry(Geometry geometry);
  void setMenuFont(nall::string font);
  void setMenuVisible(bool visible = true);
  void setModal(bool modal = true);
  void setResizable(bool resizable = true);
  void setSmartGeometry(Geometry geometry);
  void setStatusFont(nall::string font);
  void setStatusText(nall::string text);
  void setStatusVisible(bool visible = true);
  void setTitle(nall::string text);
  void setVisible(bool visible = true);
  void setWidgetFont(nall::string font);
  nall::string statusText();
  void synchronizeLayout();
  bool visible();

  Window();
  ~Window();
  struct State;
  State& state;
  pWindow& p;
};

struct Action : Object {
  bool enabled();
  void setEnabled(bool enabled = true);
  void setVisible(bool visible = true);
  bool visible();

  Action(pAction& p);
  ~Action();
  struct State;
  State& state;
  pAction& p;
};

struct Menu : private nall::base_from_member<pMenu&>, Action {
  template<typename... Args> void append(Args&&... args) { append({std::forward<Args>(args)...}); }
  template<typename... Args> void remove(Args&&... args) { remove({std::forward<Args>(args)...}); }

  void append(const nall::group<Action>& list);
  void remove(const nall::group<Action>& list);
  void setImage(const nall::image& image = nall::image{});
  void setText(nall::string text);

  Menu();
  ~Menu();
  struct State;
  State& state;
  pMenu& p;
};

struct Separator : private nall::base_from_member<pSeparator&>, Action {
  Separator();
  ~Separator();
  pSeparator& p;
};

struct Item : private nall::base_from_member<pItem&>, Action {
  nall::function<void ()> onActivate;

  void setImage(const nall::image& image = nall::image{});
  void setText(nall::string text);

  Item();
  ~Item();
  struct State;
  State& state;
  pItem& p;
};

struct CheckItem : private nall::base_from_member<pCheckItem&>, Action {
  nall::function<void ()> onToggle;

  bool checked();
  void setChecked(bool checked = true);
  void setText(nall::string text);

  CheckItem();
  ~CheckItem();
  struct State;
  State& state;
  pCheckItem& p;
};

struct RadioItem : private nall::base_from_member<pRadioItem&>, Action {
  template<typename... Args> static void group(Args&&... args) { group({std::forward<Args>(args)...}); }
  static void group(const nall::group<RadioItem>& list);

  nall::function<void ()> onActivate;

  bool checked();
  void setChecked();
  void setText(nall::string text);
  nall::string text();

  RadioItem();
  ~RadioItem();
  struct State;
  State& state;
  pRadioItem& p;
};

struct Sizable : Object {
  virtual bool enabled() = 0;
  Layout* layout();
  virtual Size minimumSize() = 0;
  virtual void setEnabled(bool enabled = true) = 0;
  virtual void setGeometry(Geometry geometry) = 0;
  virtual void setVisible(bool visible = true) = 0;
  virtual bool visible() = 0;
  Window* window();

  Sizable(pSizable& p);
  ~Sizable();
  struct State;
  State& state;
  pSizable& p;
};

struct Layout : private nall::base_from_member<pLayout&>, Sizable {
  virtual void append(Sizable& sizable);
  virtual void remove(Sizable& sizable);
  virtual void reset() {}
  virtual void synchronizeLayout() = 0;

  Layout();
  Layout(pLayout& p);
  ~Layout();
  struct State;
  State& state;
  pLayout& p;
};

struct Widget : private nall::base_from_member<pWidget&>, Sizable {
  bool enabled();
  bool focused();
  nall::string font();
  Geometry geometry();
  Size minimumSize();
  void setEnabled(bool enabled = true);
  void setFocused();
  void setFont(nall::string font);
  void setGeometry(Geometry geometry);
  void setVisible(bool visible = true);
  bool visible();

  Widget();
  Widget(pWidget& p);
  ~Widget();
  struct State;
  State& state;
  pWidget& p;
};

struct Button : private nall::base_from_member<pButton&>, Widget {
  nall::function<void ()> onActivate;

  void setImage(const nall::image& image = nall::image{}, Orientation = Orientation::Horizontal);
  void setText(nall::string text);

  Button();
  ~Button();
  struct State;
  State& state;
  pButton& p;
};

struct Canvas : private nall::base_from_member<pCanvas&>, Widget {
  nall::function<void (nall::lstring)> onDrop;
  nall::function<void ()> onMouseLeave;
  nall::function<void (Position)> onMouseMove;
  nall::function<void (Mouse::Button)> onMousePress;
  nall::function<void (Mouse::Button)> onMouseRelease;

  uint32_t* data();
  void setDroppable(bool droppable = true);
  bool setImage(const nall::image& image);
  void setSize(Size size);
  Size size();
  void update();

  Canvas();
  ~Canvas();
  struct State;
  State& state;
  pCanvas& p;
};

struct CheckButton : private nall::base_from_member<pCheckButton&>, Widget {
  nall::function<void ()> onToggle;

  bool checked();
  void setChecked(bool checked = true);
  void setText(nall::string text);

  CheckButton();
  ~CheckButton();
  struct State;
  State& state;
  pCheckButton& p;
};

struct ComboButton : private nall::base_from_member<pComboButton&>, Widget {
  nall::function<void ()> onChange;

  template<typename... Args> void append(Args&&... args) { append_({args...}); }

  void append_(const nall::lstring& list);
  void modify(unsigned row, nall::string text);
  void remove(unsigned row);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);
  nall::string text();
  nall::string text(unsigned row);

  ComboButton();
  ~ComboButton();
  struct State;
  State& state;
  pComboButton& p;
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
  ~HexEdit();
  struct State;
  State& state;
  pHexEdit& p;
};

struct HorizontalScroller : private nall::base_from_member<pHorizontalScroller&>, Widget {
  nall::function<void ()> onChange;

  unsigned length();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  HorizontalScroller();
  ~HorizontalScroller();
  struct State;
  State& state;
  pHorizontalScroller& p;
};

struct HorizontalSlider : private nall::base_from_member<pHorizontalSlider&>, Widget {
  nall::function<void ()> onChange;

  unsigned length();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  HorizontalSlider();
  ~HorizontalSlider();
  struct State;
  State& state;
  pHorizontalSlider& p;
};

struct Label : private nall::base_from_member<pLabel&>, Widget {
  void setText(nall::string text);

  Label();
  ~Label();
  struct State;
  State& state;
  pLabel& p;
};

struct LineEdit : private nall::base_from_member<pLineEdit&>, Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;

  void setEditable(bool editable = true);
  void setText(nall::string text);
  nall::string text();

  LineEdit();
  ~LineEdit();
  struct State;
  State& state;
  pLineEdit& p;
};

struct ListView : private nall::base_from_member<pListView&>, Widget {
  nall::function<void ()> onActivate;
  nall::function<void ()> onChange;
  nall::function<void (unsigned)> onToggle;

  template<typename... Args> void append(Args&&... args) { append_({args...}); }
  template<typename... Args> void modify(unsigned row, Args&&... args) { modify_(row, {args...}); }
  template<typename... Args> void setHeaderText(Args&&... args) { setHeaderText_({args...}); }

  void append_(const nall::lstring& list);
  void autoSizeColumns();
  bool checked(unsigned row);
  void modify_(unsigned row, const nall::lstring& list);
  void remove(unsigned row);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable = true);
  void setChecked(unsigned row, bool checked = true);
  void setHeaderText_(const nall::lstring& list);
  void setHeaderVisible(bool visible = true);
  void setImage(unsigned row, unsigned column, const nall::image& image = nall::image{});
  void setSelected(bool selected = true);
  void setSelection(unsigned row);

  ListView();
  ~ListView();
  struct State;
  State& state;
  pListView& p;
};

struct ProgressBar : private nall::base_from_member<pProgressBar&>, Widget {
  void setPosition(unsigned position);

  ProgressBar();
  ~ProgressBar();
  struct State;
  State& state;
  pProgressBar& p;
};

struct RadioButton : private nall::base_from_member<pRadioButton&>, Widget {
  template<typename... Args> static void group(Args&&... args) { group({std::forward<Args>(args)...}); }
  static void group(const nall::group<RadioButton>& list);

  nall::function<void ()> onActivate;

  bool checked();
  void setChecked();
  void setText(nall::string text);

  RadioButton();
  ~RadioButton();
  struct State;
  State& state;
  pRadioButton& p;
};

struct TextEdit : private nall::base_from_member<pTextEdit&>, Widget {
  nall::function<void ()> onChange;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable = true);
  void setText(nall::string text);
  void setWordWrap(bool wordWrap = true);
  nall::string text();
  bool wordWrap();

  TextEdit();
  ~TextEdit();
  struct State;
  State& state;
  pTextEdit& p;
};

struct VerticalScroller : private nall::base_from_member<pVerticalScroller&>, Widget {
  nall::function<void ()> onChange;

  unsigned length();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  VerticalScroller();
  ~VerticalScroller();
  struct State;
  State& state;
  pVerticalScroller& p;
};

struct VerticalSlider : private nall::base_from_member<pVerticalSlider&>, Widget {
  nall::function<void ()> onChange;

  unsigned length();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  VerticalSlider();
  ~VerticalSlider();
  struct State;
  State& state;
  pVerticalSlider& p;
};

struct Viewport : private nall::base_from_member<pViewport&>, Widget {
  nall::function<void (nall::lstring)> onDrop;
  nall::function<void ()> onMouseLeave;
  nall::function<void (Position)> onMouseMove;
  nall::function<void (Mouse::Button)> onMousePress;
  nall::function<void (Mouse::Button)> onMouseRelease;

  uintptr_t handle();
  void setDroppable(bool droppable = true);

  Viewport();
  ~Viewport();
  struct State;
  State& state;
  pViewport& p;
};

#include "layout/fixed-layout.hpp"
#include "layout/horizontal-layout.hpp"
#include "layout/vertical-layout.hpp"

}
