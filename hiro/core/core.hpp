#include <nall/platform.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/function.hpp>
#include <nall/image.hpp>
#include <nall/maybe.hpp>
#include <nall/range.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/traits.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>

using nall::function;
using nall::image;
using nall::lstring;
using nall::maybe;
using nall::shared_pointer;
using nall::shared_pointer_weak;
using nall::string;
using nall::vector;

namespace hiro {

#define Declare(Name) \
  struct m##Name; \
  struct p##Name; \
  using s##Name = shared_pointer<m##Name>; \
  using w##Name = shared_pointer_weak<m##Name>; \

Declare(Keyboard)
Declare(Object)
Declare(Timer)
Declare(Hotkey)
Declare(Window)
Declare(StatusBar)
Declare(MenuBar)
Declare(PopupMenu)
Declare(Action)
Declare(Menu)
Declare(MenuSeparator)
Declare(MenuItem)
Declare(MenuCheckItem)
Declare(MenuRadioItem)
Declare(Sizable)
Declare(Layout)
Declare(Widget)
Declare(Button)
Declare(Canvas)
Declare(CheckButton)
Declare(CheckLabel)
Declare(ComboButton)
Declare(ComboButtonItem)
Declare(Console)
Declare(Frame)
Declare(HexEdit)
Declare(HorizontalScroller)
Declare(HorizontalSlider)
Declare(IconView)
Declare(IconViewItem)
Declare(Label)
Declare(LineEdit)
Declare(ListView)
Declare(ListViewColumn)
Declare(ListViewItem)
Declare(ProgressBar)
Declare(RadioButton)
Declare(RadioLabel)
Declare(SourceEdit)
Declare(TabFrame)
Declare(TabFrameItem)
Declare(TextEdit)
Declare(TreeView)
Declare(TreeViewItem)
Declare(VerticalScroller)
Declare(VerticalSlider)
Declare(Viewport)

#undef Declare

enum class Edge : unsigned { Top, Bottom, Left, Right };

enum class Orientation : unsigned { Horizontal, Vertical };

struct Application {
  Application() = delete;

  static auto doMain() -> void;
  static auto font() -> string;
  static auto name() -> string;
  static auto onMain(const function<void ()>& function = {}) -> void;
  static auto run() -> void;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;
  static auto setFont(const string& font = "") -> void;
  static auto setName(const string& name = "") -> void;

  struct Windows {
    static auto doModalChange(bool modal) -> void;
    static auto onModalChange(const function<void (bool)>& function = {}) -> void;
  };

  struct Cocoa {
    static auto doAbout() -> void;
    static auto doActivate() -> void;
    static auto doPreferences() -> void;
    static auto doQuit() -> void;
    static auto onAbout(const function<void ()>& function = {}) -> void;
    static auto onActivate(const function<void ()>& function = {}) -> void;
    static auto onPreferences(const function<void ()>& function = {}) -> void;
    static auto onQuit(const function<void ()>& function = {}) -> void;
  };

//private:
  struct State {
    string font;
    string name;
    function<void ()> onMain;
    bool quit = false;

    struct Windows {
      function<void (bool)> onModalChange;
    } windows;

    struct Cocoa {
      function<void ()> onAbout;
      function<void ()> onActivate;
      function<void ()> onPreferences;
      function<void ()> onQuit;
    } cocoa;
  };
  static State state;
  static auto initialize() -> void;
};

struct Color {
  using type = Color;

  Color();
  Color(signed red, signed green, signed blue);
  Color(signed alpha, signed red, signed green, signed blue);

  explicit operator bool() const;
  auto operator==(const Color& source) const -> bool;
  auto operator!=(const Color& source) const -> bool;

  auto alpha() const -> uint8_t;
  auto blue() const -> uint8_t;
  auto empty() const -> bool;
  auto green() const -> uint8_t;
  auto red() const -> uint8_t;
  auto setAlpha(signed alpha) -> type&;
  auto setBlue(signed blue) -> type&;
  auto setColor(Color color = {}) -> type&;
  auto setColor(signed red, signed green, signed blue) -> type&;
  auto setColor(signed alpha, signed red, signed green, signed blue) -> type&;
  auto setGreen(signed green) -> type&;
  auto setRed(signed red) -> type&;
  auto value() const -> uint32_t;

//private:
  struct State {
    signed alpha;
    signed red;
    signed green;
    signed blue;
  } state;
};

struct Position {
  using type = Position;

  Position();
  Position(signed x, signed y);

  auto operator==(const Position& source) const -> bool;
  auto operator!=(const Position& source) const -> bool;

  auto setPosition(Position position = {}) -> type&;
  auto setPosition(signed x, signed y) -> type&;
  auto setX(signed x) -> type&;
  auto setY(signed y) -> type&;
  auto x() const -> signed;
  auto y() const -> signed;

//private:
  struct State {
    signed x;
    signed y;
  } state;
};

struct Size {
  using type = Size;

  Size();
  Size(signed width, signed height);

  auto operator==(const Size& source) const -> bool;
  auto operator!=(const Size& source) const -> bool;

  auto height() const -> signed;
  auto setHeight(signed height) -> type&;
  auto setSize(Size source = {}) -> type&;
  auto setSize(signed width, signed height) -> type&;
  auto setWidth(signed width) -> type&;
  auto width() const -> signed;

  static const signed Maximum = ~0;  //~0 == -1
  static const signed Minimum =  0;

//private:
  struct State {
    signed width;
    signed height;
  } state;
};

struct Geometry {
  using type = Geometry;

  Geometry();
  Geometry(Position position, Size size);
  Geometry(signed x, signed y, signed width, signed height);
  Geometry(const string& text);

  auto operator==(const Geometry& source) const -> bool;
  auto operator!=(const Geometry& source) const -> bool;

  auto height() const -> signed;
  auto position() const -> Position;
  auto setGeometry(Geometry geometry = {}) -> type&;
  auto setGeometry(Position position, Size size) -> type&;
  auto setGeometry(signed x, signed y, signed width, signed height) -> type&;
  auto setHeight(signed height) -> type&;
  auto setPosition(Position position = {}) -> type&;
  auto setPosition(signed x, signed y) -> type&;
  auto setSize(Size size = {}) -> type&;
  auto setSize(signed width, signed height) -> type&;
  auto setWidth(signed width) -> type&;
  auto setX(signed x) -> type&;
  auto setY(signed y) -> type&;
  auto size() const -> Size;
  auto text() const -> string;
  auto width() const -> signed;
  auto x() const -> signed;
  auto y() const -> signed;

//private:
  struct State {
    signed x;
    signed y;
    signed width;
    signed height;
  } state;
};

struct Font {
  Font() = delete;

  static auto serif(unsigned size = 0, const string& style = "") -> string;
  static auto sans(unsigned size = 0, const string& style = "") -> string;
  static auto monospace(unsigned size = 0, const string& style = "") -> string;
  static auto size(const string& font, const string& text = " ") -> Size;
};

struct Desktop {
  Desktop() = delete;

  static auto size() -> Size;
  static auto workspace() -> Geometry;
};

struct Monitor {
  Monitor() = delete;

  static auto count() -> unsigned;
  static auto geometry(unsigned monitor) -> Geometry;
  static auto primary() -> unsigned;
};

struct Keyboard {
  Keyboard() = delete;

  static auto append(sHotkey hotkey) -> void;
  static auto hotkey(unsigned position) -> sHotkey;
  static auto hotkeys() -> unsigned;
  static auto poll() -> vector<bool>;
  static auto pressed(const string& key) -> bool;
  static auto released(const string& key) -> bool;
  static auto remove(sHotkey hotkey) -> void;

  static const vector<string> keys;

//private:
  struct State {
    vector<sHotkey> hotkeys;
  };
  static State state;
};

struct Mouse {
  enum class Button : unsigned { Left, Middle, Right };

  Mouse() = delete;

  static auto position() -> Position;
  static auto pressed(Button) -> bool;
  static auto released(Button) -> bool;
};

struct BrowserWindow {
  using type = BrowserWindow;

  auto directory() -> string;
  auto open() -> string;
  auto save() -> string;
  auto setFilters(const lstring& filters = {"*"}) -> type&;
  auto setParent(sWindow parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

//private:
  struct State {
    lstring filters;
    sWindow parent;
    string path;
    string title;
  } state;
};

struct MessageWindow {
  enum class Buttons : unsigned { Ok, OkCancel, YesNo, YesNoCancel };
  enum class Response : unsigned { Ok, Cancel, Yes, No };

  using type = MessageWindow;

  MessageWindow(const string& text = "");

  auto error(Buttons = Buttons::Ok) -> Response;
  auto information(Buttons = Buttons::Ok) -> Response;
  auto question(Buttons = Buttons::YesNo) -> Response;
  auto setParent(sWindow parent) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto warning(Buttons = Buttons::Ok) -> Response;

//private:
  struct State {
    MessageWindow::Buttons buttons = MessageWindow::Buttons::Ok;
    sWindow parent;
    string text;
    string title;
  } state;
};

#define Declare(Name) \
  using type = m##Name; \
  operator s##Name() const { return instance; } \
  auto self() -> p##Name* { return (p##Name*)delegate; } \
  auto self() const -> const p##Name* { return (const p##Name*)delegate; } \
  auto bind(const s##Name& instance) -> void { \
    this->instance = instance; \
    if(!abstract()) construct(); \
  } \
  auto unbind() -> void { \
    reset(); \
    destruct(); \
    instance.reset(); \
  } \
  virtual auto allocate() -> pObject*; \

struct mObject {
  Declare(Object)

  mObject();
  virtual ~mObject();
  mObject(const mObject&) = delete;
  mObject& operator=(const mObject&) = delete;

  auto abstract() const -> bool;
  auto enabled(bool recursive = false) const -> bool;
  virtual auto focused() const -> bool;
  auto font(bool recursive = false) const -> string;
  auto offset() const -> signed;
  auto offset(signed displacement) -> type&;
  auto parent() const -> mObject*;
  auto parentComboButton(bool recursive = false) const -> mComboButton*;
  auto parentFrame(bool recursive = false) const -> mFrame*;
  auto parentIconView(bool recursive = false) const -> mIconView*;
  auto parentLayout(bool recursive = false) const -> mLayout*;
  auto parentListView(bool recursive = false) const -> mListView*;
  auto parentMenu(bool recursive = false) const -> mMenu*;
  auto parentMenuBar(bool recursive = false) const -> mMenuBar*;
  auto parentPopupMenu(bool recursive = false) const -> mPopupMenu*;
  auto parentSizable(bool recursive = false) const -> mSizable*;
  auto parentTabFrame(bool recursive = false) const -> mTabFrame*;
  auto parentTabFrameItem(bool recursive = false) const -> mTabFrameItem*;
  auto parentTreeView(bool recursive = false) const -> mTreeView*;
  auto parentTreeViewItem(bool recursive = false) const -> mTreeViewItem*;
  auto parentWidget(bool recursive = false) const -> mWidget*;
  auto parentWindow(bool recursive = false) const -> mWindow*;
  virtual auto remove() -> type&;
  virtual auto reset() -> type&;
  virtual auto setEnabled(bool enabled = true) -> type&;
  virtual auto setFocused() -> type&;
  virtual auto setFont(const string& font = "") -> type&;
  virtual auto setParent(mObject* parent = nullptr, signed offset = -1) -> type&;
  virtual auto setVisible(bool visible = true) -> type&;
  auto visible(bool recursive = false) const -> bool;

//private:
  struct State {
    bool enabled = true;
    string font;
    signed offset = -1;
    mObject* parent = nullptr;
    bool visible = true;
  } state;

  wObject instance;
  pObject* delegate = nullptr;

  virtual auto construct() -> void;
  virtual auto destruct() -> void;
};

struct mHotkey : mObject {
  Declare(Hotkey)

  auto doPress() const -> void;
  auto doRelease() const -> void;
  auto onPress(const function<void ()>& function = {}) -> type&;
  auto onRelease(const function<void ()>& function = {}) -> type&;
  auto parent() const -> wObject;
  auto remove() -> type& override;
  auto sequence() const -> string;
  auto setParent(sObject parent) -> type&;
  auto setSequence(const string& sequence = "") -> type&;

//private:
  struct State {
    bool active = false;
    vector<unsigned> keys;
    function<void ()> onPress;
    function<void ()> onRelease;
    wObject parent;
    string sequence;
  } state;
};

struct mTimer : mObject {
  Declare(Timer)

  auto doActivate() const -> void;
  auto interval() const -> unsigned;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto setInterval(unsigned interval = 0) -> type&;

//private:
  struct State {
    unsigned interval = 0;
    function<void ()> onActivate;
  } state;
};

struct mWindow : mObject {
  Declare(Window)
  using mObject::remove;

  auto append(sLayout layout) -> type&;
  auto append(sMenuBar menuBar) -> type&;
  auto append(sStatusBar statusBar) -> type&;
  auto backgroundColor() const -> Color;
  auto doClose() const -> void;
  auto doDrop(lstring) const -> void;
  auto doKeyPress(signed) const -> void;
  auto doKeyRelease(signed) const -> void;
  auto doMove() const -> void;
  auto doSize() const -> void;
  auto droppable() const -> bool;
  auto frameGeometry() const -> Geometry;
  auto fullScreen() const -> bool;
  auto geometry() const -> Geometry;
  auto layout() const -> sLayout;
  auto menuBar() const -> sMenuBar;
  auto modal() const -> bool;
  auto onClose(const function<void ()>& function = {}) -> type&;
  auto onDrop(const function<void (lstring)>& function = {}) -> type&;
  auto onKeyPress(const function<void (signed)>& function = {}) -> type&;
  auto onKeyRelease(const function<void (signed)>& function = {}) -> type&;
  auto onMove(const function<void ()>& function = {}) -> type&;
  auto onSize(const function<void ()>& function = {}) -> type&;
  auto remove(sLayout layout) -> type&;
  auto remove(sMenuBar menuBar) -> type&;
  auto remove(sStatusBar statusBar) -> type&;
  auto reset() -> type& override;
  auto resizable() const -> bool;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCentered() -> type&;
  auto setCentered(sWindow parent) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setFrameGeometry(Geometry geometry) -> type&;
  auto setFramePosition(Position position) -> type&;
  auto setFrameSize(Size size) -> type&;
  auto setFullScreen(bool fullScreen = true) -> type&;
  auto setGeometry(Geometry geometry) -> type&;
  auto setModal(bool modal = true) -> type&;
  auto setPosition(Position position) -> type&;
  auto setResizable(bool resizable = true) -> type&;
  auto setSize(Size size) -> type&;
  auto setTitle(const string& title = "") -> type&;
  auto statusBar() const -> sStatusBar;
  auto title() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool droppable = false;
    bool fullScreen = false;
    Geometry geometry = {128, 128, 256, 256};
    sLayout layout;
    sMenuBar menuBar;
    bool modal = false;
    function<void ()> onClose;
    function<void (lstring)> onDrop;
    function<void (signed)> onKeyPress;
    function<void (signed)> onKeyRelease;
    function<void ()> onMove;
    function<void ()> onSize;
    bool resizable = true;
    sStatusBar statusBar;
    string title;
  } state;

  auto destruct() -> void;
};

struct mStatusBar : mObject {
  Declare(StatusBar)

  auto remove() -> type& override;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    string text;
  } state;
};

struct mMenuBar : mObject {
  Declare(MenuBar)

  auto append(sMenu menu) -> type&;
  auto menu(unsigned position) const -> sMenu;
  auto menus() const -> unsigned;
  auto remove() -> type& override;
  auto remove(sMenu menu) -> type&;
  auto reset() -> type&;

//private:
  struct State {
    vector<sMenu> menus;
  } state;

  auto destruct() -> void override;
};

struct mPopupMenu : mObject {
  Declare(PopupMenu)
  using mObject::remove;

  auto action(unsigned position) const -> sAction;
  auto actions() const -> unsigned;
  auto append(sAction action) -> type&;
  auto remove(sAction action) -> type&;
  auto reset() -> type&;
  auto setVisible(bool visible = true) -> type& override;

//private:
  struct State {
    vector<sAction> actions;
  } state;

  auto destruct() -> void override;
};

struct mAction : mObject {
  Declare(Action)

  auto remove() -> type& override;

//private:
  struct State {
  } state;
};

struct mMenu : mAction {
  Declare(Menu)
  using mObject::remove;

  auto action(unsigned position) const -> sAction;
  auto actions() const -> unsigned;
  auto append(sAction action) -> type&;
  auto icon() const -> image;
  auto remove(sAction action) -> type&;
  auto reset() -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    vector<sAction> actions;
    image icon;
    string text;
  } state;

  auto destruct() -> void override;
};

struct mMenuSeparator : mAction {
  Declare(MenuSeparator)

//private:
  struct State {
  } state;
};

struct mMenuItem : mAction {
  Declare(MenuItem)

  auto doActivate() const -> void;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    image icon;
    function<void ()> onActivate;
    string text;
  } state;
};

struct mMenuCheckItem : mAction {
  Declare(MenuCheckItem)

  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto onToggle(const function<void ()>& function = {}) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool checked = false;
    function<void ()> onToggle;
    string text;
  } state;
};

struct mMenuRadioItem : mAction {
  Declare(MenuRadioItem)

  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto setChecked() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

  static auto group(const vector<wMenuRadioItem>& group) -> void;

//private:
  struct State {
    bool checked = true;
    vector<wMenuRadioItem> group;
    function<void ()> onActivate;
    string text;
  } state;
};

struct mSizable : mObject {
  Declare(Sizable)

  auto geometry() const -> Geometry;
  virtual auto minimumSize() const -> Size;
  virtual auto setGeometry(Geometry geometry) -> type&;

//private:
  struct State {
    Geometry geometry;
  } state;
};

struct mLayout : mSizable {
  Declare(Layout)

  virtual auto append(sSizable sizable) -> type&;
  virtual auto remove() -> type& override;
  virtual auto remove(sSizable sizable) -> type&;
  virtual auto reset() -> type&;
  auto setParent(mObject* parent = nullptr, signed offset = -1) -> type& override;
  auto sizable(unsigned position) const -> sSizable;
  auto sizables() const -> unsigned;

//private:
  struct State {
    vector<sSizable> sizables;
  } state;

  auto destruct() -> void override;
};

struct mWidget : mSizable {
  Declare(Widget)

  auto doSize() const -> void;
  auto onSize(const function<void ()>& function = {}) -> type&;
  auto remove() -> type& override;

//private:
  struct State {
    function<void ()> onSize;
  } state;
};

struct mButton : mWidget {
  Declare(Button)

  auto bordered() const -> bool;
  auto doActivate() const -> void;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto orientation() const -> Orientation;
  auto setBordered(bool bordered = true) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool bordered = true;
    image icon;
    function<void ()> onActivate;
    Orientation orientation = Orientation::Horizontal;
    string text;
  } state;
};

struct mCanvas : mWidget {
  Declare(Canvas)

  auto color() const -> Color;
  auto data() -> uint32_t*;
  auto droppable() const -> bool;
  auto doDrop(lstring names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto gradient() const -> vector<Color>;
  auto icon() const -> image;
  auto onDrop(const function<void (lstring)>& function = {}) -> type&;
  auto onMouseLeave(const function<void ()>& function = {}) -> type&;
  auto onMouseMove(const function<void (Position)>& function = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& function = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& function = {}) -> type&;
  auto setColor(Color color) -> type&;
  auto setData(Size size) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> type&;
  auto setHorizontalGradient(Color left, Color right) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setVerticalGradient(Color top, Color bottom) -> type&;
  auto size() const -> Size;
  auto update() -> type&;

//private:
  struct State {
    Color color;
    vector<uint32_t> data;
    bool droppable = false;
    vector<Color> gradient = {{}, {}, {}, {}};
    image icon;
    function<void (lstring)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
    Size size;
  } state;
};

struct mCheckButton : mWidget {
  Declare(CheckButton)

  auto bordered() const -> bool;
  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto icon() const -> image;
  auto onToggle(const function<void ()>& function = {}) -> type&;
  auto orientation() const -> Orientation;
  auto setBordered(bool bordered = true) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool bordered = false;
    bool checked = false;
    image icon;
    function<void ()> onToggle;
    Orientation orientation = Orientation::Horizontal;
    string text;
  } state;
};

struct mCheckLabel : mWidget {
  Declare(CheckLabel)

  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto onToggle(const function<void ()>& function = {}) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool checked = false;
    function<void ()> onToggle;
    string text;
  } state;
};

struct mComboButton : mWidget {
  Declare(ComboButton)
  using mObject::remove;

  auto append(sComboButtonItem item) -> type&;
  auto doChange() const -> void;
  auto item(unsigned position) const -> sComboButtonItem;
  auto items() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto remove(sComboButtonItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> sComboButtonItem;

//private:
  struct State {
    vector<sComboButtonItem> items;
    function<void ()> onChange;
    signed selected = -1;
  } state;

  auto destruct() -> void override;
};

struct mComboButtonItem : mObject {
  Declare(ComboButtonItem)

  auto icon() const -> image;
  auto remove() -> type& override;
  auto selected() const -> bool;
  auto setIcon(const image& icon = {}) -> type&;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    image icon;
    string text;
  } state;
};

struct mConsole : mWidget {
  Declare(Console)

  auto backgroundColor() const -> Color;
  auto doActivate(string) const -> void;
  auto foregroundColor() const -> Color;
  auto onActivate(const function<void (string)>& function = {}) -> type&;
  auto print(const string& text) -> type&;
  auto prompt() const -> string;
  auto reset() -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setPrompt(const string& prompt = "") -> type&;

//private:
  struct State {
    Color backgroundColor;
    Color foregroundColor;
    function<void (string)> onActivate;
    string prompt;
  } state;
};

struct mFrame : mWidget {
  Declare(Frame)
  using mObject::remove;

  auto append(sLayout layout) -> type&;
  auto layout() const -> sLayout;
  auto remove(sLayout layout) -> type&;
  auto reset() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    sLayout layout;
    string text;
  } state;

  auto destruct() -> void override;
};

struct mHexEdit : mWidget {
  Declare(HexEdit)

  auto backgroundColor() const -> Color;
  auto columns() const -> unsigned;
  auto doRead(unsigned offset) const -> uint8_t;
  auto doWrite(unsigned offset, uint8_t data) const -> void;
  auto foregroundColor() const -> Color;
  auto length() const -> unsigned;
  auto offset() const -> unsigned;
  auto onRead(const function<uint8_t (unsigned)>& function = {}) -> type&;
  auto onWrite(const function<void (unsigned, uint8_t)>& function = {}) -> type&;
  auto rows() const -> unsigned;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setColumns(unsigned columns = 16) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setLength(unsigned length) -> type&;
  auto setOffset(unsigned offset) -> type&;
  auto setRows(unsigned rows = 16) -> type&;
  auto update() -> type&;

//private:
  struct State {
    Color backgroundColor;
    unsigned columns = 16;
    Color foregroundColor;
    unsigned length = 0;
    unsigned offset = 0;
    function<uint8_t (unsigned)> onRead;
    function<void (unsigned, uint8_t)> onWrite;
    unsigned rows = 16;
  } state;
};

struct mHorizontalScroller : mWidget {
  Declare(HorizontalScroller)

  auto doChange() const -> void;
  auto length() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto position() const -> unsigned;
  auto setLength(unsigned length = 101) -> type&;
  auto setPosition(unsigned position = 0) -> type&;

//private:
  struct State {
    unsigned length = 101;
    function<void ()> onChange;
    unsigned position = 0;
  } state;
};

struct mHorizontalSlider : mWidget {
  Declare(HorizontalSlider)

  auto doChange() const -> void;
  auto length() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto position() const -> unsigned;
  auto setLength(unsigned length = 101) -> type&;
  auto setPosition(unsigned position = 0) -> type&;

//private:
  struct State {
    unsigned length = 101;
    function<void ()> onChange;
    unsigned position = 0;
  } state;
};

struct mIconView : mWidget {
  Declare(IconView)
  using mObject::remove;

  auto append(sIconViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto flow() const -> Orientation;
  auto foregroundColor() const -> Color;
  auto item(unsigned position) const -> sIconViewItem;
  auto items() const -> unsigned;
  auto multiSelect() const -> bool;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onContext(const function<void ()>& function = {}) -> type&;
  auto orientation() const -> Orientation;
  auto remove(sIconViewItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> maybe<unsigned>;
  auto selectedItems() const -> vector<unsigned>;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setFlow(Orientation flow = Orientation::Vertical) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setMultiSelect(bool multipleSelections = true) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setSelected(const vector<signed>& selections) -> type&;

//private:
  struct State {
    Color backgroundColor;
    Color foregroundColor;
    Orientation flow = Orientation::Vertical;
    vector<sIconViewItem> items;
    bool multiSelect = false;
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    Orientation orientation = Orientation::Horizontal;
  } state;

  auto destruct() -> void override;
};

struct mIconViewItem : mObject {
  Declare(IconViewItem)

  auto icon() const -> image;
  auto remove() -> type& override;
  auto selected() const -> bool;
  auto setIcon(const image& icon = {}) -> type&;
  auto setSelected(bool selected = true) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    image icon;
    bool selected = false;
    string text;
  } state;
};

struct mLabel : mWidget {
  Declare(Label)

  auto horizontalAlignment() const -> double;
  auto setHorizontalAlignment(double alignment = 0.0) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setVerticalAlignment(double alignment = 0.5) -> type&;
  auto text() const -> string;
  auto verticalAlignment() const -> double;

//private:
  struct State {
    double horizontalAlignment = 0.0;
    string text;
    double verticalAlignment = 0.5;
  } state;
};

struct mLineEdit : mWidget {
  Declare(LineEdit)

  auto backgroundColor() const -> Color;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool editable = true;
    Color foregroundColor;
    function<void ()> onActivate;
    function<void ()> onChange;
    string text;
  } state;
};

struct mListView : mWidget {
  Declare(ListView)
  using mObject::remove;

  auto append(sListViewColumn column) -> type&;
  auto append(sListViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto checkable() const -> bool;
  auto checked() const -> vector<sListViewItem>;
  auto column(unsigned position) const -> sListViewColumn;
  auto columns() const -> unsigned;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto doEdit(sListViewItem item, sListViewColumn column) const -> void;
  auto doSort(sListViewColumn column) const -> void;
  auto doToggle(sListViewItem item) const -> void;
  auto foregroundColor() const -> Color;
  auto gridVisible() const -> bool;
  auto headerVisible() const -> bool;
  auto item(unsigned position) const -> sListViewItem;
  auto items() const -> unsigned;
  auto multiSelect() const -> bool;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onContext(const function<void ()>& function = {}) -> type&;
  auto onEdit(const function<void (sListViewItem, sListViewColumn)>& function = {}) -> type&;
  auto onSort(const function<void (sListViewColumn)>& function = {}) -> type&;
  auto onToggle(const function<void (sListViewItem)>& function = {}) -> type&;
  auto remove(sListViewColumn column) -> type&;
  auto remove(sListViewItem item) -> type&;
  auto reset() -> type&;
  auto resizeColumns() -> type&;
  auto selected() const -> sListViewItem;
  auto selectedItems() const -> vector<sListViewItem>;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCheckable(bool checkable = true) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setGridVisible(bool visible = true) -> type&;
  auto setHeaderVisible(bool visible = true) -> type&;
  auto setMultiSelect(bool multiSelect = true) -> type&;
  auto setSelected(bool selected = true) -> type&;

//private:
  struct State {
    unsigned activeColumn = 0;
    Color backgroundColor;
    bool checkable = false;
    vector<sListViewColumn> columns;
    Color foregroundColor;
    bool gridVisible = false;
    bool headerVisible = false;
    vector<sListViewItem> items;
    bool multiSelect = false;
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    function<void (sListViewItem, sListViewColumn)> onEdit;
    function<void (sListViewColumn)> onSort;
    function<void (sListViewItem)> onToggle;
  } state;

  auto destruct() -> void override;
};

struct mListViewColumn : mObject {
  Declare(ListViewColumn)

  auto active() const -> bool;
  auto backgroundColor() const -> Color;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto horizontalAlignment() const -> double;
  auto icon() const -> image;
  auto remove() -> type& override;
  auto resizable() const -> bool;
  auto setActive() -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setFont(const string& font = "") -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setHorizontalAlignment(double alignment = 0.0) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setResizable(bool resizable = true) -> type&;
  auto setSortable(bool sortable = true) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setVerticalAlignment(double alignment = 0.5) -> type&;
  auto setVisible(bool visible = true) -> type&;
  auto setWidth(signed width = 0) -> type&;
  auto sortable() const -> bool;
  auto text() const -> string;
  auto verticalAlignment() const -> double;
  auto width() const -> signed;

//private:
  struct State {
    Color backgroundColor;
    bool editable = false;
    string font;
    Color foregroundColor;
    double horizontalAlignment = 0.0;
    image icon;
    bool resizable = true;
    bool sortable = false;
    string text;
    double verticalAlignment = 0.5;
    bool visible = true;
    signed width = 0;
  } state;
};

struct mListViewItem : mObject {
  Declare(ListViewItem)

  auto checked() const -> bool;
  auto icon(unsigned column = 0) const -> image;
  auto remove() -> type& override;
  auto selected() const -> bool;
  auto setChecked(bool checked = true) -> type&;
  auto setFocused() -> type& override;
  auto setIcon(unsigned column, const image& icon = {}) -> type&;
  auto setSelected(bool selected = true) -> type&;
  auto setText(const lstring& text) -> type&;
  auto setText(unsigned column, const string& text = "") -> type&;
  auto text(unsigned column = 0) const -> string;

//private:
  struct State {
    bool checked = false;
    vector<image> icon;
    bool selected = false;
    lstring text;
  } state;
};

struct mProgressBar : mWidget {
  Declare(ProgressBar)

  auto position() const -> unsigned;
  auto setPosition(unsigned position) -> type&;

//private:
  struct State {
    unsigned position = 0;
  } state;
};

struct mRadioButton : mWidget {
  Declare(RadioButton)

  auto bordered() const -> bool;
  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto orientation() const -> Orientation;
  auto setBordered(bool bordered = true) -> type&;
  auto setChecked() -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

  static auto group(const vector<wRadioButton>& group) -> void;

//private:
  struct State {
    bool bordered = true;
    bool checked = true;
    vector<wRadioButton> group;
    image icon;
    function<void ()> onActivate;
    Orientation orientation = Orientation::Horizontal;
    string text;
  } state;
};

struct mRadioLabel : mWidget {
  Declare(RadioLabel)

  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto setChecked() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

  static auto group(const vector<wRadioLabel>& group) -> void;

//private:
  struct State {
    bool checked = true;
    vector<wRadioLabel> group;
    function<void ()> onActivate;
    string text;
  } state;
};

struct mSourceEdit : mWidget {
  Declare(SourceEdit)

  auto doChange() const -> void;
  auto doMove() const -> void;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onMove(const function<void ()>& function = {}) -> type&;
  auto position() const -> unsigned;
  auto setPosition(signed position) -> type&;
  auto setSelected(Position selected) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    function<void ()> onChange;
    function<void ()> onMove;
    unsigned position = 0;
    Position selected;
    string text;
  } state;
};

struct mTabFrame : mWidget {
  Declare(TabFrame)
  using mObject::remove;
  friend class mTabFrameItem;

  auto append(sTabFrameItem item) -> type&;
  auto doChange() const -> void;
  auto doClose(sTabFrameItem item) const -> void;
  auto doMove(sTabFrameItem from, sTabFrameItem to) const -> void;
  auto edge() const -> Edge;
  auto item(unsigned position) const -> sTabFrameItem;
  auto items() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onClose(const function<void (sTabFrameItem)>& function = {}) -> type&;
  auto onMove(const function<void (sTabFrameItem, sTabFrameItem)>& function = {}) -> type&;
  auto remove(sTabFrameItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> sTabFrameItem;
  auto setEdge(Edge edge = Edge::Top) -> type&;
  auto setParent(mObject* object = nullptr, signed offset = -1) -> type& override;

//private:
  struct State {
    Edge edge = Edge::Top;
    vector<sTabFrameItem> items;
    function<void ()> onChange;
    function<void (sTabFrameItem)> onClose;
    function<void (sTabFrameItem, sTabFrameItem)> onMove;
    unsigned selected = 0;
  } state;

  auto destruct() -> void override;
};

struct mTabFrameItem : mObject {
  Declare(TabFrameItem)

  auto append(sLayout layout) -> type&;
  auto closable() const -> bool;
  auto icon() const -> image;
  auto layout() const -> sLayout;
  auto movable() const -> bool;
  auto remove() -> type& override;
  auto remove(sLayout layout) -> type&;
  auto reset() -> type&;
  auto selected() const -> bool;
  auto setClosable(bool closable = true) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setMovable(bool movable = true) -> type&;
  auto setParent(mObject* object = nullptr, signed offset = -1) -> type& override;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool closable = false;
    image icon;
    sLayout layout;
    bool movable = false;
    string text;
  } state;

  auto destruct() -> void override;
};

struct mTextEdit : mWidget {
  Declare(TextEdit)

  auto backgroundColor() const -> Color;
  auto cursorPosition() const -> unsigned;
  auto doChange() const -> void;
  auto doMove() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onMove(const function<void ()>& function = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCursorPosition(unsigned position) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setWordWrap(bool wordWrap = true) -> type&;
  auto text() const -> string;
  auto wordWrap() const -> bool;

//private:
  struct State {
    Color backgroundColor;
    unsigned cursorPosition = 0;
    bool editable = true;
    Color foregroundColor;
    function<void ()> onChange;
    function<void ()> onMove;
    string text;
    bool wordWrap = true;
  } state;
};

struct mTreeView : mWidget {
  Declare(TreeView)
  using mObject::remove;

  auto append(sTreeViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto checkable() const -> bool;
  auto collapse() -> type&;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto doToggle(sTreeViewItem item) const -> void;
  auto expand() -> type&;
  auto foregroundColor() const -> Color;
  auto item(const string& path) const -> sTreeViewItem;
  auto items() const -> unsigned;
  auto onActivate(const function<void ()>& function = {}) -> type&;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto onContext(const function<void ()>& function = {}) -> type&;
  auto onToggle(const function<void (sTreeViewItem)>& function = {}) -> type&;
  auto remove(sTreeViewItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> sTreeViewItem;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCheckable(bool checkable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;

//private:
  struct State {
    Color backgroundColor;
    bool checkable = false;
    Color foregroundColor;
    vector<sTreeViewItem> items;
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    function<void (sTreeViewItem)> onToggle;
    string selectedPath;
  } state;

  auto destruct() -> void override;
};

struct mTreeViewItem : mObject {
  Declare(TreeViewItem)

  auto append(sTreeViewItem item) -> type&;
  auto checked() const -> bool;
  auto icon() const -> image;
  auto item(const string& path) const -> sTreeViewItem;
  auto items() const -> unsigned;
  auto path() const -> string;
  auto remove() -> type& override;
  auto remove(sTreeViewItem item) -> type&;
  auto selected() const -> bool;
  auto setChecked(bool checked = true) -> type&;
  auto setFocused() -> type& override;
  auto setIcon(const image& icon = {}) -> type&;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool checked = false;
    image icon;
    vector<sTreeViewItem> items;
    string text;
  } state;

  auto destruct() -> void override;
};

struct mVerticalScroller : mWidget {
  Declare(VerticalScroller)

  auto doChange() const -> void;
  auto length() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto position() const -> unsigned;
  auto setLength(unsigned length = 101) -> type&;
  auto setPosition(unsigned position = 0) -> type&;

//private:
  struct State {
    unsigned length = 101;
    function<void ()> onChange;
    unsigned position = 0;
  } state;
};

struct mVerticalSlider : mWidget {
  Declare(VerticalSlider)

  auto doChange() const -> void;
  auto length() const -> unsigned;
  auto onChange(const function<void ()>& function = {}) -> type&;
  auto position() const -> unsigned;
  auto setLength(unsigned length = 101) -> type&;
  auto setPosition(unsigned position = 0) -> type&;

//private:
  struct State {
    unsigned length = 101;
    function<void ()> onChange;
    unsigned position = 0;
  } state;
};

struct mViewport : mWidget {
  Declare(Viewport)

  auto doDrop(lstring names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto droppable() const -> bool;
  auto handle() const -> uintptr_t;
  auto onDrop(const function<void (lstring)>& function = {}) -> type&;
  auto onMouseLeave(const function<void ()>& function = {}) -> type&;
  auto onMouseMove(const function<void (Position position)>& function = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& function = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& function = {}) -> type&;
  auto setDroppable(bool droppable = true) -> type&;

//private:
  struct State {
    bool droppable = false;
    function<void (lstring)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
  } state;
};

#undef Declare

}
