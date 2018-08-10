#include <nall/platform.hpp>
#include <nall/chrono.hpp>
#include <nall/directory.hpp>
#include <nall/function.hpp>
#include <nall/image.hpp>
#include <nall/locale.hpp>
#include <nall/maybe.hpp>
#include <nall/path.hpp>
#include <nall/range.hpp>
#include <nall/run.hpp>
#include <nall/set.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/traits.hpp>
#include <nall/unique-pointer.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>

using nall::function;
using nall::image;
using nall::Locale;
using nall::maybe;
using nall::nothing;
using nall::set;
using nall::shared_pointer;
using nall::shared_pointer_weak;
using nall::string;
using nall::unique_pointer;
using nall::vector;

namespace hiro {

auto initialize() -> void;

struct Font;
struct Keyboard;

#define Declare(Name) \
  struct Name; \
  struct m##Name; \
  struct p##Name; \
  using s##Name = shared_pointer<m##Name>; \
  using w##Name = shared_pointer_weak<m##Name>; \

Declare(Object)
Declare(Group)
Declare(Timer)
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
Declare(Widget)
Declare(Button)
Declare(Canvas)
Declare(CheckButton)
Declare(CheckLabel)
Declare(ComboButton)
Declare(ComboButtonItem)
Declare(ComboEdit)
Declare(ComboEditItem)
Declare(Console)
Declare(Frame)
Declare(HexEdit)
Declare(HorizontalScrollBar)
Declare(HorizontalSlider)
Declare(IconView)
Declare(IconViewItem)
Declare(Label)
Declare(LineEdit)
Declare(ProgressBar)
Declare(RadioButton)
Declare(RadioLabel)
Declare(SourceEdit)
Declare(TabFrame)
Declare(TabFrameItem)
Declare(TableView)
Declare(TableViewHeader)
Declare(TableViewColumn)
Declare(TableViewItem)
Declare(TableViewCell)
Declare(TextEdit)
Declare(TreeView)
Declare(TreeViewItem)
Declare(VerticalScrollBar)
Declare(VerticalSlider)
Declare(Viewport)

#undef Declare

enum class Orientation : uint { Horizontal, Vertical };
enum class Navigation : uint { Top, Bottom, Left, Right };
enum class Sort : uint { None, Ascending, Descending };

#if defined(Hiro_Color)
struct Color {
  using type = Color;

  Color();
  Color(int red, int green, int blue, int alpha = 255);

  explicit operator bool() const;
  auto operator==(const Color& source) const -> bool;
  auto operator!=(const Color& source) const -> bool;

  auto alpha() const -> uint8_t;
  auto blue() const -> uint8_t;
  auto green() const -> uint8_t;
  auto red() const -> uint8_t;
  auto reset() -> type&;
  auto setAlpha(int alpha) -> type&;
  auto setBlue(int blue) -> type&;
  auto setColor(Color color = {}) -> type&;
  auto setColor(int red, int green, int blue, int alpha = 255) -> type&;
  auto setGreen(int green) -> type&;
  auto setRed(int red) -> type&;
  auto value() const -> uint32_t;

//private:
  struct State {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
  } state;
};
#endif

#if defined(Hiro_Gradient)
struct Gradient {
  using type = Gradient;

  Gradient();

  explicit operator bool() const;
  auto operator==(const Gradient& source) const -> bool;
  auto operator!=(const Gradient& source) const -> bool;

  auto setBilinear(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> type&;
  auto setHorizontal(Color left, Color right) -> type&;
  auto setVertical(Color top, Color bottom) -> type&;

//private:
  struct State {
    vector<Color> colors;
  } state;
};
#endif

#if defined(Hiro_Alignment)
struct Alignment {
  using type = Alignment;

  Alignment();
  Alignment(float horizontal, float vertical = 0.5);

  explicit operator bool() const;
  auto operator==(const Alignment& source) const -> bool;
  auto operator!=(const Alignment& source) const -> bool;

  auto horizontal() const -> float;
  auto reset() -> type&;
  auto setAlignment(float horizontal = -1.0, float vertical = 0.5) -> type&;
  auto setHorizontal(float horizontal) -> type&;
  auto setVertical(float vertical) -> type&;
  auto vertical() const -> float;

//private:
  struct State {
    float horizontal;
    float vertical;
  } state;
};
#endif

#if defined(Hiro_Cursor)
struct Cursor {
  using type = Cursor;

  Cursor(int offset = 0, int length = 0);

  explicit operator bool() const;
  auto operator==(const Cursor& source) const -> bool;
  auto operator!=(const Cursor& source) const -> bool;

  auto length() const -> int;
  auto offset() const -> int;
  auto setCursor(int offset = 0, int length = 0) -> type&;
  auto setLength(int length = 0) -> type&;
  auto setOffset(int offset = 0) -> type&;

//private:
  struct State {
    int offset;
    int length;
  } state;
};
#endif

#if defined(Hiro_Position)
struct Position {
  using type = Position;

  Position();
  Position(float x, float y);
  template<typename X, typename Y>
  Position(X x, Y y) : Position((float)x, (float)y) {}

  explicit operator bool() const;
  auto operator==(const Position& source) const -> bool;
  auto operator!=(const Position& source) const -> bool;

  auto reset() -> type&;
  auto setPosition(Position position = {}) -> type&;
  auto setPosition(float x, float y) -> type&;
  auto setX(float x) -> type&;
  auto setY(float y) -> type&;
  auto x() const -> float;
  auto y() const -> float;

//private:
  struct State {
    float x;
    float y;
  } state;
};
#endif

#if defined(Hiro_Size)
struct Size {
  using type = Size;

  Size();
  Size(float width, float height);
  template<typename W, typename H>
  Size(W width, H height) : Size((float)width, (float)height) {}

  explicit operator bool() const;
  auto operator==(const Size& source) const -> bool;
  auto operator!=(const Size& source) const -> bool;

  auto height() const -> float;
  auto reset() -> type&;
  auto setHeight(float height) -> type&;
  auto setSize(Size source = {}) -> type&;
  auto setSize(float width, float height) -> type&;
  auto setWidth(float width) -> type&;
  auto width() const -> float;

  static constexpr float Maximum = -1.0;
  static constexpr float Minimum = +0.0;

//private:
  struct State {
    float width;
    float height;
  } state;
};
#endif

#if defined(Hiro_Geometry)
struct Geometry {
  using type = Geometry;

  Geometry();
  Geometry(Position position, Size size);
  Geometry(float x, float y, float width, float height);
  template<typename X, typename Y, typename W, typename H>
  Geometry(X x, Y y, W width, H height) : Geometry((float)x, (float)y, (float)width, (float)height) {}

  explicit operator bool() const;
  auto operator==(const Geometry& source) const -> bool;
  auto operator!=(const Geometry& source) const -> bool;

  auto height() const -> float;
  auto position() const -> Position;
  auto reset() -> type&;
  auto setGeometry(Geometry geometry = {}) -> type&;
  auto setGeometry(Position position, Size size) -> type&;
  auto setGeometry(float x, float y, float width, float height) -> type&;
  auto setHeight(float height) -> type&;
  auto setPosition(Position position = {}) -> type&;
  auto setPosition(float x, float y) -> type&;
  auto setSize(Size size = {}) -> type&;
  auto setSize(float width, float height) -> type&;
  auto setWidth(float width) -> type&;
  auto setX(float x) -> type&;
  auto setY(float y) -> type&;
  auto size() const -> Size;
  auto width() const -> float;
  auto x() const -> float;
  auto y() const -> float;

//private:
  struct State {
    float x;
    float y;
    float width;
    float height;
  } state;
};
#endif

#include "font.hpp"

#if defined(Hiro_Hotkey)
struct Hotkey {
  using type = Hotkey;

  Hotkey();
  Hotkey(const string& sequence);

  explicit operator bool() const;
  auto operator==(const Hotkey& source) const -> bool;
  auto operator!=(const Hotkey& source) const -> bool;

  auto doPress() const -> void;
  auto doRelease() const -> void;
  auto onPress(const function<void ()>& callback = {}) -> type&;
  auto onRelease(const function<void ()>& callback = {}) -> type&;
  auto reset() -> type&;
  auto sequence() const -> string;
  auto setSequence(const string& sequence = "") -> type&;

//private:
  struct State {
    bool active = false;
    vector<uint> keys;
    function<void ()> onPress;
    function<void ()> onRelease;
    string sequence;
  };
  shared_pointer<State> state;
};
#endif

#include "application.hpp"
#include "desktop.hpp"
#include "monitor.hpp"

#if defined(Hiro_Keyboard)
struct Keyboard {
  Keyboard() = delete;

  static auto append(Hotkey hotkey) -> void;
  static auto hotkey(uint position) -> Hotkey;
  static auto hotkeyCount() -> uint;
  static auto hotkeys() -> vector<Hotkey>;
  static auto poll() -> vector<bool>;
  static auto pressed(const string& key) -> bool;
  static auto released(const string& key) -> bool;
  static auto remove(Hotkey hotkey) -> void;

  static const vector<string> keys;

//private:
  struct State {
    vector<Hotkey> hotkeys;
  };
  static State state;
};
#endif

#if defined(Hiro_Mouse)
struct Mouse {
  enum class Button : uint { Left, Middle, Right };

  Mouse() = delete;

  static auto position() -> Position;
  static auto pressed(Button) -> bool;
  static auto released(Button) -> bool;
};
#endif

#if defined(Hiro_BrowserWindow)
struct BrowserWindow {
  using type = BrowserWindow;

  auto directory() -> string;
  auto open() -> string;
  auto save() -> string;
  auto setFilters(const vector<string>& filters = {"*"}) -> type&;
  auto setParent(sWindow parent) -> type&;
  auto setPath(const string& path = "") -> type&;
  auto setTitle(const string& title = "") -> type&;

//private:
  struct State {
    vector<string> filters;
    sWindow parent;
    string path;
    string title;
  } state;
};
#endif

#if defined(Hiro_MessageWindow)
struct MessageWindow {
  enum class Buttons : uint { Ok, OkCancel, YesNo, YesNoCancel };
  enum class Response : uint { Ok, Cancel, Yes, No };

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
#endif

struct Property {
  using type = Property;

  Property(const string& name, const string& value = "");

  auto operator==(const Property& source) const -> bool;
  auto operator< (const Property& source) const -> bool;

  auto name() const -> string;
  auto setValue(const string& value = "") -> type&;
  auto value() const -> string;

private:
  struct State {
    string name;
    string value;
  } state;
};

#define Declare(Name) \
  using type = m##Name; \
  operator s##Name() const { return instance; } \
  auto self() -> p##Name* { return (p##Name*)delegate; } \
  auto self() const -> const p##Name* { return (const p##Name*)delegate; } \
  auto bind(const s##Name& instance) -> void { \
    this->instance = instance; \
    setGroup(); \
    if(!abstract()) construct(); \
  } \
  auto unbind() -> void { \
    reset(); \
    destruct(); \
    instance.reset(); \
  } \
  virtual auto allocate() -> pObject*; \

#include "object.hpp"
#include "group.hpp"
#include "timer.hpp"
#include "window.hpp"

#if defined(Hiro_StatusBar)
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
#endif

#if defined(Hiro_MenuBar)
struct mMenuBar : mObject {
  Declare(MenuBar)

  auto append(sMenu menu) -> type&;
  auto menu(uint position) const -> Menu;
  auto menuCount() const -> uint;
  auto menus() const -> vector<Menu>;
  auto remove() -> type& override;
  auto remove(sMenu menu) -> type&;
  auto reset() -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;

//private:
  struct State {
    vector<sMenu> menus;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_PopupMenu)
struct mPopupMenu : mObject {
  Declare(PopupMenu)
  using mObject::remove;

  auto action(uint position) const -> Action;
  auto actionCount() const -> uint;
  auto actions() const -> vector<Action>;
  auto append(sAction action) -> type&;
  auto remove(sAction action) -> type&;
  auto reset() -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setVisible(bool visible = true) -> type& override;

//private:
  struct State {
    vector<sAction> actions;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_Action)
struct mAction : mObject {
  Declare(Action)

  auto remove() -> type& override;

//private:
  struct State {
  } state;
};
#endif

#if defined(Hiro_Menu)
struct mMenu : mAction {
  Declare(Menu)
  using mObject::remove;

  auto action(uint position) const -> Action;
  auto actionCount() const -> uint;
  auto actions() const -> vector<Action>;
  auto append(sAction action) -> type&;
  auto icon() const -> image;
  auto remove(sAction action) -> type&;
  auto reset() -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
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
#endif

#if defined(Hiro_MenuSeparator)
struct mMenuSeparator : mAction {
  Declare(MenuSeparator)

//private:
  struct State {
  } state;
};
#endif

#if defined(Hiro_MenuItem)
struct mMenuItem : mAction {
  Declare(MenuItem)

  auto doActivate() const -> void;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_MenuCheckItem)
struct mMenuCheckItem : mAction {
  Declare(MenuCheckItem)

  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto onToggle(const function<void ()>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_MenuRadioItem)
struct mMenuRadioItem : mAction {
  Declare(MenuRadioItem)

  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto group() const -> Group override;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto setChecked() -> type&;
  auto setGroup(sGroup group = {}) -> type& override;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool checked = false;
    sGroup group;
    function<void ()> onActivate;
    string text;
  } state;
};
#endif

#include "sizable.hpp"

#include "widget/widget.hpp"

#if defined(Hiro_Button)
struct mButton : mWidget {
  Declare(Button)

  auto bordered() const -> bool;
  auto doActivate() const -> void;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_Canvas)
struct mCanvas : mWidget {
  Declare(Canvas)

  auto color() const -> Color;
  auto data() -> uint32_t*;
  auto droppable() const -> bool;
  auto doDrop(vector<string> names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto gradient() const -> Gradient;
  auto icon() const -> image;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onMouseLeave(const function<void ()>& callback = {}) -> type&;
  auto onMouseMove(const function<void (Position)>& callback = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto setColor(Color color = {}) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setGradient(Gradient gradient = {}) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setSize(Size size = {}) -> type&;
  auto size() const -> Size;
  auto update() -> type&;

//private:
  struct State {
    Color color;
    bool droppable = false;
    Gradient gradient;
    image icon;
    function<void (vector<string>)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
  } state;
};
#endif

#if defined(Hiro_CheckButton)
struct mCheckButton : mWidget {
  Declare(CheckButton)

  auto bordered() const -> bool;
  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto icon() const -> image;
  auto onToggle(const function<void ()>& callback = {}) -> type&;
  auto orientation() const -> Orientation;
  auto setBordered(bool bordered = true) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool bordered = true;
    bool checked = false;
    image icon;
    function<void ()> onToggle;
    Orientation orientation = Orientation::Horizontal;
    string text;
  } state;
};
#endif

#if defined(Hiro_CheckLabel)
struct mCheckLabel : mWidget {
  Declare(CheckLabel)

  auto checked() const -> bool;
  auto doToggle() const -> void;
  auto onToggle(const function<void ()>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_ComboButton)
struct mComboButton : mWidget {
  Declare(ComboButton)
  using mObject::remove;

  auto append(sComboButtonItem item) -> type&;
  auto doChange() const -> void;
  auto item(uint position) const -> ComboButtonItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<ComboButtonItem>;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto remove(sComboButtonItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> ComboButtonItem;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;

//private:
  struct State {
    vector<sComboButtonItem> items;
    function<void ()> onChange;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_ComboButton)
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
    bool selected = false;
    string text;
  } state;
};
#endif

#if defined(Hiro_ComboEdit)
struct mComboEdit : mWidget {
  Declare(ComboEdit)
  using mObject::remove;

  auto append(sComboEditItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto item(uint position) const -> ComboEditItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<ComboEditItem>;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto remove(sComboEditItem item) -> type&;
  auto reset() -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool editable = true;
    Color foregroundColor;
    vector<sComboEditItem> items;
    function<void ()> onActivate;
    function<void ()> onChange;
    string text;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_ComboEdit)
struct mComboEditItem : mObject {
  Declare(ComboEditItem)

  auto icon() const -> image;
  auto remove() -> type& override;
  auto setIcon(const image& icon = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    image icon;
    string text;
  } state;
};
#endif

#if defined(Hiro_Console)
struct mConsole : mWidget {
  Declare(Console)

  auto backgroundColor() const -> Color;
  auto doActivate(string) const -> void;
  auto foregroundColor() const -> Color;
  auto onActivate(const function<void (string)>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_Frame)
struct mFrame : mWidget {
  Declare(Frame)
  using mObject::remove;

  auto append(sSizable sizable) -> type&;
  auto remove(sSizable sizable) -> type&;
  auto reset() -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setText(const string& text = "") -> type&;
  auto sizable() const -> Sizable;
  auto text() const -> string;

//private:
  struct State {
    sSizable sizable;
    string text;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_HexEdit)
struct mHexEdit : mWidget {
  Declare(HexEdit)

  auto address() const -> uint;
  auto backgroundColor() const -> Color;
  auto columns() const -> uint;
  auto doRead(uint offset) const -> uint8_t;
  auto doWrite(uint offset, uint8_t data) const -> void;
  auto foregroundColor() const -> Color;
  auto length() const -> uint;
  auto onRead(const function<uint8_t (uint)>& callback = {}) -> type&;
  auto onWrite(const function<void (uint, uint8_t)>& callback = {}) -> type&;
  auto rows() const -> uint;
  auto setAddress(uint address = 0) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setColumns(uint columns = 16) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setLength(uint length) -> type&;
  auto setRows(uint rows = 16) -> type&;
  auto update() -> type&;

//private:
  struct State {
    uint address = 0;
    Color backgroundColor;
    uint columns = 16;
    Color foregroundColor;
    uint length = 0;
    function<uint8_t (uint)> onRead;
    function<void (uint, uint8_t)> onWrite;
    uint rows = 16;
  } state;
};
#endif

#if defined(Hiro_HorizontalScrollBar)
struct mHorizontalScrollBar : mWidget {
  Declare(HorizontalScrollBar)

  auto doChange() const -> void;
  auto length() const -> uint;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto position() const -> uint;
  auto setLength(uint length = 101) -> type&;
  auto setPosition(uint position = 0) -> type&;

//private:
  struct State {
    uint length = 101;
    function<void ()> onChange;
    uint position = 0;
  } state;
};
#endif

#if defined(Hiro_HorizontalSlider)
struct mHorizontalSlider : mWidget {
  Declare(HorizontalSlider)

  auto doChange() const -> void;
  auto length() const -> uint;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto position() const -> uint;
  auto setLength(uint length = 101) -> type&;
  auto setPosition(uint position = 0) -> type&;

//private:
  struct State {
    uint length = 101;
    function<void ()> onChange;
    uint position = 0;
  } state;
};
#endif

#if defined(Hiro_IconView)
struct mIconView : mWidget {
  Declare(IconView)
  using mObject::remove;

  auto append(sIconViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto batchable() const -> bool;
  auto batched() const -> vector<IconViewItem>;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto flow() const -> Orientation;
  auto foregroundColor() const -> Color;
  auto item(uint position) const -> IconViewItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<IconViewItem>;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onContext(const function<void ()>& callback = {}) -> type&;
  auto orientation() const -> Orientation;
  auto remove(sIconViewItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> IconViewItem;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setBatchable(bool batchable = true) -> type&;
  auto setFlow(Orientation flow = Orientation::Vertical) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setParent(mObject* object = nullptr, int offset = -1) -> type& override;
  auto setSelected(const vector<int>& selections) -> type&;

//private:
  struct State {
    Color backgroundColor;
    bool batchable = false;
    Color foregroundColor;
    Orientation flow = Orientation::Vertical;
    vector<sIconViewItem> items;
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    Orientation orientation = Orientation::Horizontal;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_IconView)
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
#endif

#if defined(Hiro_Label)
struct mLabel : mWidget {
  Declare(Label)

  auto alignment() const -> Alignment;
  auto backgroundColor() const -> Color;
  auto foregroundColor() const -> Color;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Alignment alignment;
    Color backgroundColor;
    Color foregroundColor;
    string text;
  } state;
};
#endif

#if defined(Hiro_LineEdit)
struct mLineEdit : mWidget {
  Declare(LineEdit)

  auto backgroundColor() const -> Color;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto onChange(const function<void ()>& callback = {}) -> type&;
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
#endif

#if defined(Hiro_ProgressBar)
struct mProgressBar : mWidget {
  Declare(ProgressBar)

  auto position() const -> uint;
  auto setPosition(uint position) -> type&;

//private:
  struct State {
    uint position = 0;
  } state;
};
#endif

#if defined(Hiro_RadioButton)
struct mRadioButton : mWidget {
  Declare(RadioButton)

  auto bordered() const -> bool;
  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto group() const -> Group override;
  auto icon() const -> image;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto orientation() const -> Orientation;
  auto setBordered(bool bordered = true) -> type&;
  auto setChecked() -> type&;
  auto setGroup(sGroup group = {}) -> type& override;
  auto setIcon(const image& icon = {}) -> type&;
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool bordered = true;
    bool checked = false;
    sGroup group;
    image icon;
    function<void ()> onActivate;
    Orientation orientation = Orientation::Horizontal;
    string text;
  } state;
};
#endif

#if defined(Hiro_RadioLabel)
struct mRadioLabel : mWidget {
  Declare(RadioLabel)

  auto checked() const -> bool;
  auto doActivate() const -> void;
  auto group() const -> Group override;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto setChecked() -> type&;
  auto setGroup(sGroup group = {}) -> type& override;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    bool checked = false;
    sGroup group;
    function<void ()> onActivate;
    string text;
  } state;
};
#endif

#include "widget/source-edit.hpp"
#include "widget/tab-frame.hpp"
#include "widget/tab-frame-item.hpp"
#include "widget/table-view.hpp"
#include "widget/table-view-column.hpp"
#include "widget/table-view-item.hpp"
#include "widget/table-view-cell.hpp"

#if defined(Hiro_TextEdit)
struct mTextEdit : mWidget {
  Declare(TextEdit)

  auto backgroundColor() const -> Color;
  auto cursor() const -> Cursor;
  auto doChange() const -> void;
  auto doMove() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onMove(const function<void ()>& callback = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCursor(Cursor cursor = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setWordWrap(bool wordWrap = true) -> type&;
  auto text() const -> string;
  auto wordWrap() const -> bool;

//private:
  struct State {
    Color backgroundColor;
    Cursor cursor;
    bool editable = true;
    Color foregroundColor;
    function<void ()> onChange;
    function<void ()> onMove;
    string text;
    bool wordWrap = true;
  } state;
};
#endif

#if defined(Hiro_TreeView)
struct mTreeView : mWidget {
  Declare(TreeView)
  using mObject::remove;

  auto append(sTreeViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto doToggle(sTreeViewItem item) const -> void;
  auto foregroundColor() const -> Color;
  auto item(const string& path) const -> TreeViewItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<TreeViewItem>;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onContext(const function<void ()>& callback = {}) -> type&;
  auto onToggle(const function<void (sTreeViewItem)>& callback = {}) -> type&;
  auto remove(sTreeViewItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> TreeViewItem;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;

//private:
  struct State {
    Color backgroundColor;
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
#endif

#if defined(Hiro_TreeView)
struct mTreeViewItem : mObject {
  Declare(TreeViewItem)

  auto append(sTreeViewItem item) -> type&;
  auto backgroundColor(bool recursive = false) const -> Color;
  auto checkable() const -> bool;
  auto checked() const -> bool;
  auto foregroundColor(bool recursive = false) const -> Color;
  auto icon() const -> image;
  auto item(const string& path) const -> TreeViewItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<TreeViewItem>;
  auto path() const -> string;
  auto remove() -> type& override;
  auto remove(sTreeViewItem item) -> type&;
  auto selected() const -> bool;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCheckable(bool checkable = true) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setExpanded(bool expanded = true) -> type&;
  auto setFocused() -> type& override;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool checkable = false;
    bool checked = false;
    Color foregroundColor;
    image icon;
    vector<sTreeViewItem> items;
    string text;
  } state;

  auto destruct() -> void override;
};
#endif

#if defined(Hiro_VerticalScrollBar)
struct mVerticalScrollBar : mWidget {
  Declare(VerticalScrollBar)

  auto doChange() const -> void;
  auto length() const -> uint;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto position() const -> uint;
  auto setLength(uint length = 101) -> type&;
  auto setPosition(uint position = 0) -> type&;

//private:
  struct State {
    uint length = 101;
    function<void ()> onChange;
    uint position = 0;
  } state;
};
#endif

#if defined(Hiro_VerticalSlider)
struct mVerticalSlider : mWidget {
  Declare(VerticalSlider)

  auto doChange() const -> void;
  auto length() const -> uint;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto position() const -> uint;
  auto setLength(uint length = 101) -> type&;
  auto setPosition(uint position = 0) -> type&;

//private:
  struct State {
    uint length = 101;
    function<void ()> onChange;
    uint position = 0;
  } state;
};
#endif

#if defined(Hiro_Viewport)
struct mViewport : mWidget {
  Declare(Viewport)

  auto doDrop(vector<string> names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto droppable() const -> bool;
  auto handle() const -> uintptr_t;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onMouseLeave(const function<void ()>& callback = {}) -> type&;
  auto onMouseMove(const function<void (Position position)>& callback = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto setDroppable(bool droppable = true) -> type&;

//private:
  struct State {
    bool droppable = false;
    function<void (vector<string>)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
  } state;
};
#endif

#undef Declare

#include "lock.hpp"
#include "shared.hpp"

}
