#include "state.hpp"
#include "layout/fixed-layout.cpp"
#include "layout/horizontal-layout.cpp"
#include "layout/vertical-layout.cpp"

#if defined(PHOENIX_WINDOWS)
  #include "../windows/platform.cpp"
#elif defined(PHOENIX_QT)
  #include "../qt/platform.cpp"
#elif defined(PHOENIX_GTK)
  #include "../gtk/platform.cpp"
#elif defined(PHOENIX_REFERENCE)
  #include "../reference/platform.cpp"
#endif

static bool OS_quit = false;

//Color
//=====

uint32_t Color::rgb() const {
  return (255 << 24) + (red << 16) + (green << 8) + (blue << 0);
}

uint32_t Color::rgba() const {
  return (alpha << 24) + (red << 16) + (green << 8) + (blue << 0);
}

//Geometry
//========

Position Geometry::position() const {
  return { x, y };
}

Size Geometry::size() const {
  return { width, height };
}

string Geometry::text() const {
  return { x, ",", y, ",", width, ",", height };
}

Geometry::Geometry(const string &text) {
  lstring part = text.split(",");
  x = integer(part(0, "256"));
  y = integer(part(1, "256"));
  width = decimal(part(2, "256"));
  height = decimal(part(3, "256"));
}

//Font
//====

Geometry Font::geometry(const string &text) {
  return pFont::geometry(description, text);
}

Font::Font(const string &description):
description(description) {
}

//Desktop
//=======

Size Desktop::size() {
  return pDesktop::size();
}

Geometry Desktop::workspace() {
  return pDesktop::workspace();
}

//Keyboard
//========

bool Keyboard::pressed(Keyboard::Scancode scancode) {
  return pKeyboard::pressed(scancode);
}

bool Keyboard::released(Keyboard::Scancode scancode) {
  return !pressed(scancode);
}

vector<bool> Keyboard::state() {
  return pKeyboard::state();
}

//Mouse
//=====

Position Mouse::position() {
  return pMouse::position();
}

bool Mouse::pressed(Mouse::Button button) {
  return pMouse::pressed(button);
}

bool Mouse::released(Mouse::Button button) {
  return !pressed(button);
}

//DialogWindow
//============

string DialogWindow::fileOpen_(Window &parent, const string &path, const lstring &filter_) {
  auto filter = filter_;
  if(filter.size() == 0) filter.append("All files (*)");
  return pDialogWindow::fileOpen(parent, path, filter);
}

string DialogWindow::fileSave_(Window &parent, const string &path, const lstring &filter_) {
  auto filter = filter_;
  if(filter.size() == 0) filter.append("All files (*)");
  return pDialogWindow::fileSave(parent, path, filter);
}

string DialogWindow::folderSelect(Window &parent, const string &path) {
  return pDialogWindow::folderSelect(parent, path);
}

//MessageWindow
//=============

MessageWindow::Response MessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return pMessageWindow::information(parent, text, buttons);
}

MessageWindow::Response MessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return pMessageWindow::question(parent, text, buttons);
}

MessageWindow::Response MessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return pMessageWindow::warning(parent, text, buttons);
}

MessageWindow::Response MessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return pMessageWindow::critical(parent, text, buttons);
}

//Object
//======

Object::Object(pObject &p):
p(p) {
  OS::initialize();
  p.constructor();
}

Object::~Object() {
  p.destructor();
  delete &p;
}

//OS
//==

void OS::main() {
  return pOS::main();
}

bool OS::pendingEvents() {
  return pOS::pendingEvents();
}

void OS::processEvents() {
  return pOS::processEvents();
}

void OS::quit() {
  OS_quit = true;
  return pOS::quit();
}

void OS::initialize() {
  static bool initialized = false;
  if(initialized == false) {
    initialized = true;
    return pOS::initialize();
  }
}

//Timer
//=====

void Timer::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Timer::setInterval(unsigned milliseconds) {
  state.milliseconds = milliseconds;
  return p.setInterval(milliseconds);
}

Timer::Timer():
state(*new State),
base_from_member<pTimer&>(*new pTimer(*this)),
Object(base_from_member<pTimer&>::value),
p(base_from_member<pTimer&>::value) {
  p.constructor();
}

Timer::~Timer() {
  p.destructor();
  delete &state;
}

//Window
//======

Window& Window::none() {
  return pWindow::none();
}

void Window::append_(Layout &layout) {
  if(state.layout.append(layout)) {
    ((Sizable&)layout).state.window = this;
    ((Sizable&)layout).state.layout = 0;
    p.append(layout);
    layout.synchronizeLayout();
  }
}

void Window::append_(Menu &menu) {
  if(state.menu.append(menu)) {
    ((Action&)menu).state.window = this;
    p.append(menu);
  }
}

void Window::append_(Widget &widget) {
  if(state.widget.append(widget)) {
    ((Sizable&)widget).state.window = this;
    p.append(widget);
  }
}

Color Window::backgroundColor() {
  return p.backgroundColor();
}

Geometry Window::frameGeometry() {
  Geometry geometry = p.geometry();
  Geometry margin = p.frameMargin();
  return {
    geometry.x - margin.x, geometry.y - margin.y,
    geometry.width + margin.width, geometry.height + margin.height
  };
}

Geometry Window::frameMargin() {
  return p.frameMargin();
}

bool Window::focused() {
  return p.focused();
}

bool Window::fullScreen() {
  return state.fullScreen;
}

Geometry Window::geometry() {
  return p.geometry();
}

void Window::ignore() {
  state.ignore = true;
}

void Window::remove_(Layout &layout) {
  if(state.layout.remove(layout)) {
    p.remove(layout);
    ((Sizable&)layout).state.window = 0;
  }
}

void Window::remove_(Menu &menu) {
  if(state.menu.remove(menu)) {
    p.remove(menu);
    ((Action&)menu).state.window = 0;
  }
}

void Window::remove_(Widget &widget) {
  if(state.widget.remove(widget)) {
    p.remove(widget);
    ((Sizable&)widget).state.window = 0;
  }
}

void Window::setBackgroundColor(const Color &color) {
  state.backgroundColorOverride = true;
  state.backgroundColor = color;
  return p.setBackgroundColor(color);
}

void Window::setFrameGeometry(const Geometry &geometry) {
  Geometry margin = p.frameMargin();
  return setGeometry({
    geometry.x + margin.x, geometry.y + margin.y,
    geometry.width - margin.width, geometry.height - margin.height
  });
}

void Window::setFocused() {
  return p.setFocused();
}

void Window::setFullScreen(bool fullScreen) {
  state.fullScreen = fullScreen;
  return p.setFullScreen(fullScreen);
}

void Window::setGeometry(const Geometry &geometry) {
  state.geometry = geometry;
  return p.setGeometry(geometry);
}

void Window::setMenuFont(const string &font) {
  state.menuFont = font;
  return p.setMenuFont(font);
}

void Window::setMenuVisible(bool visible) {
  state.menuVisible = visible;
  return p.setMenuVisible(visible);
}

void Window::setModal(bool modal) {
  state.modal = modal;
  return p.setModal(modal);
}

void Window::setResizable(bool resizable) {
  state.resizable = resizable;
  return p.setResizable(resizable);
}

void Window::setSmartGeometry(const Geometry &geometry) {
  Geometry margin = p.frameMargin();
  return setGeometry({
    geometry.x + margin.x, geometry.y + margin.y,
    geometry.width, geometry.height
  });
}

void Window::setStatusFont(const string &font) {
  state.statusFont = font;
  return p.setStatusFont(font);
}

void Window::setStatusText(const string &text) {
  state.statusText = text;
  return p.setStatusText(text);
}

void Window::setStatusVisible(bool visible) {
  state.statusVisible = visible;
  return p.setStatusVisible(visible);
}

void Window::setTitle(const string &text) {
  state.title = text;
  return p.setTitle(text);
}

void Window::setVisible(bool visible) {
  state.visible = visible;
  synchronizeLayout();
  return p.setVisible(visible);
}

void Window::setWidgetFont(const string &font) {
  state.widgetFont = font;
  return p.setWidgetFont(font);
}

string Window::statusText() {
  return state.statusText;
}

void Window::synchronizeLayout() {
  if(visible() && OS_quit == false) setGeometry(geometry());
}

bool Window::visible() {
  return state.visible;
}

Window::Window():
state(*new State),
base_from_member<pWindow&>(*new pWindow(*this)),
Object(base_from_member<pWindow&>::value),
p(base_from_member<pWindow&>::value) {
  p.constructor();
}

Window::~Window() {
  p.destructor();
  delete &state;
}

//Action
//======

bool Action::enabled() {
  return state.enabled;
}

void Action::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Action::setVisible(bool visible) {
  state.visible = visible;
  return p.setVisible(visible);
}

bool Action::visible() {
  return state.visible;
}

Action::Action(pAction &p):
state(*new State),
Object(p),
p(p) {
  p.constructor();
}

Action::~Action() {
  p.destructor();
  delete &state;
}

//Menu
//====

void Menu::append(const set<Action&> &list) {
  for(auto &action : list) {
    if(state.action.append(action)) {
      action.state.menu = this;
      p.append(action);
    }
  }
}

void Menu::remove(const set<Action&> &list) {
  for(auto &action : list) {
    if(state.action.remove(action)) {
      action.state.menu = 0;
      return p.remove(action);
    }
  }
}

void Menu::setImage(const image &image) {
  state.image = image;
  return p.setImage(image);
}

void Menu::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

Menu::Menu():
state(*new State),
base_from_member<pMenu&>(*new pMenu(*this)),
Action(base_from_member<pMenu&>::value),
p(base_from_member<pMenu&>::value) {
  p.constructor();
}

Menu::~Menu() {
  p.destructor();
  delete &state;
}

//Separator
//=========

Separator::Separator():
base_from_member<pSeparator&>(*new pSeparator(*this)),
Action(base_from_member<pSeparator&>::value),
p(base_from_member<pSeparator&>::value) {
  p.constructor();
}

Separator::~Separator() {
  p.destructor();
}

//Item
//====

void Item::setImage(const image &image) {
  state.image = image;
  return p.setImage(image);
}

void Item::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

Item::Item():
state(*new State),
base_from_member<pItem&>(*new pItem(*this)),
Action(base_from_member<pItem&>::value),
p(base_from_member<pItem&>::value) {
  p.constructor();
}

Item::~Item() {
  p.destructor();
  delete &state;
}

//CheckItem
//=========

bool CheckItem::checked() {
  return p.checked();
}

void CheckItem::setChecked(bool checked) {
  state.checked = checked;
  return p.setChecked(checked);
}

void CheckItem::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

CheckItem::CheckItem():
state(*new State),
base_from_member<pCheckItem&>(*new pCheckItem(*this)),
Action(base_from_member<pCheckItem&>::value),
p(base_from_member<pCheckItem&>::value) {
  p.constructor();
}

CheckItem::~CheckItem() {
  p.destructor();
  delete &state;
}

//RadioItem
//=========

void RadioItem::group(const set<RadioItem&> &list) {
  for(auto &item : list) item.p.setGroup(item.state.group = list);
  if(list.size()) list[0].setChecked();
}

bool RadioItem::checked() {
  return p.checked();
}

void RadioItem::setChecked() {
  for(auto &item : state.group) item.state.checked = false;
  state.checked = true;
  return p.setChecked();
}

void RadioItem::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

string RadioItem::text() {
  return state.text;
}

RadioItem::RadioItem():
state(*new State),
base_from_member<pRadioItem&>(*new pRadioItem(*this)),
Action(base_from_member<pRadioItem&>::value),
p(base_from_member<pRadioItem&>::value) {
  p.constructor();
}

RadioItem::~RadioItem() {
  for(auto &item : state.group) {
    if(&item != this) item.state.group.remove(*this);
  }
  p.destructor();
  delete &state;
}

//Sizable
//=======

Layout* Sizable::layout() {
  return state.layout;
}

Window* Sizable::window() {
  if(state.layout) return state.layout->window();
  return state.window;
}

Sizable::Sizable(pSizable &p):
state(*new State),
Object(p),
p(p) {
  p.constructor();
}

Sizable::~Sizable() {
  if(layout()) layout()->remove(*this);
  p.destructor();
  delete &state;
}

//Layout
//======

void Layout::append(Sizable &sizable) {
  sizable.state.layout = this;
  sizable.state.window = 0;

  if(dynamic_cast<Layout*>(&sizable)) {
    Layout &layout = (Layout&)sizable;
    layout.synchronizeLayout();
  }

  if(dynamic_cast<Widget*>(&sizable)) {
    Widget &widget = (Widget&)sizable;
    if(sizable.window()) sizable.window()->append(widget);
  }
}

void Layout::remove(Sizable &sizable) {
  if(dynamic_cast<Widget*>(&sizable)) {
    Widget &widget = (Widget&)sizable;
    if(sizable.window()) sizable.window()->remove(widget);
  }

  sizable.state.layout = 0;
  sizable.state.window = 0;
}

Layout::Layout():
state(*new State),
base_from_member<pLayout&>(*new pLayout(*this)),
Sizable(base_from_member<pLayout&>::value),
p(base_from_member<pLayout&>::value) {
}

Layout::Layout(pLayout &p):
state(*new State),
base_from_member<pLayout&>(p),
Sizable(p),
p(p) {
}

Layout::~Layout() {
  if(layout()) layout()->remove(*this);
  else if(window()) window()->remove(*this);
  p.destructor();
  delete &state;
}

//Widget
//======

bool Widget::enabled() {
  return state.enabled;
}

bool Widget::focused() {
  return p.focused();
}

string Widget::font() {
  return state.font;
}

Geometry Widget::geometry() {
  return state.geometry;
}

Geometry Widget::minimumGeometry() {
  return p.minimumGeometry();
}

void Widget::setEnabled(bool enabled) {
  state.enabled = enabled;
  return p.setEnabled(enabled);
}

void Widget::setFocused() {
  return p.setFocused();
}

void Widget::setFont(const string &font) {
  state.font = font;
  return p.setFont(font);
}

void Widget::setGeometry(const Geometry &geometry) {
  state.geometry = geometry;
  return p.setGeometry(geometry);
}

void Widget::setVisible(bool visible) {
  state.visible = visible;
  return p.setVisible(visible);
}

bool Widget::visible() {
  return state.visible;
}

Widget::Widget():
state(*new State),
base_from_member<pWidget&>(*new pWidget(*this)),
Sizable(base_from_member<pWidget&>::value),
p(base_from_member<pWidget&>::value) {
  state.abstract = true;
  p.constructor();
}

Widget::Widget(pWidget &p):
state(*new State),
base_from_member<pWidget&>(p),
Sizable(base_from_member<pWidget&>::value),
p(base_from_member<pWidget&>::value) {
  p.constructor();
}

Widget::~Widget() {
  p.destructor();
  delete &state;
}

//Button
//======

void Button::setImage(const image &image, Orientation orientation) {
  state.image = image;
  state.orientation = orientation;
  return p.setImage(image, orientation);
}

void Button::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

Button::Button():
state(*new State),
base_from_member<pButton&>(*new pButton(*this)),
Widget(base_from_member<pButton&>::value),
p(base_from_member<pButton&>::value) {
  p.constructor();
}

Button::~Button() {
  p.destructor();
  delete &state;
}

//Canvas
//======

uint32_t* Canvas::data() {
  return state.data;
}

bool Canvas::setImage(const nall::image &image) {
  if(image.data == nullptr || image.width == 0 || image.height == 0) return false;
  state.width = image.width;
  state.height = image.height;
  setSize({ state.width, state.height });
  memcpy(state.data, image.data, state.width * state.height * sizeof(uint32_t));
  return true;
}

void Canvas::setSize(const Size &size) {
  state.width = size.width;
  state.height = size.height;
  delete[] state.data;
  state.data = new uint32_t[size.width * size.height];
  return p.setSize(size);
}

Size Canvas::size() {
  return { state.width, state.height };
}

void Canvas::update() {
  return p.update();
}

Canvas::Canvas():
state(*new State),
base_from_member<pCanvas&>(*new pCanvas(*this)),
Widget(base_from_member<pCanvas&>::value),
p(base_from_member<pCanvas&>::value) {
  state.data = new uint32_t[state.width * state.height];
  p.constructor();
}

Canvas::~Canvas() {
  p.destructor();
  delete[] state.data;
  delete &state;
}

//CheckBox
//========

bool CheckBox::checked() {
  return p.checked();
}

void CheckBox::setChecked(bool checked) {
  state.checked = checked;
  return p.setChecked(checked);
}

void CheckBox::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

CheckBox::CheckBox():
state(*new State),
base_from_member<pCheckBox&>(*new pCheckBox(*this)),
Widget(base_from_member<pCheckBox&>::value),
p(base_from_member<pCheckBox&>::value) {
  p.constructor();
}

CheckBox::~CheckBox() {
  p.destructor();
  delete &state;
}

//ComboBox
//========

void ComboBox::append_(const lstring &list) {
  for(auto &text : list) {
    state.text.append(text);
    p.append(text);
  }
}

void ComboBox::modify(unsigned row, const string &text) {
  state.text(row) = text;
  p.modify(row, text);
}

void ComboBox::remove(unsigned row) {
  state.text.remove(row);
  p.remove(row);
}

void ComboBox::reset() {
  state.selection = 0;
  state.text.reset();
  return p.reset();
}

unsigned ComboBox::selection() {
  return p.selection();
}

void ComboBox::setSelection(unsigned row) {
  state.selection = row;
  return p.setSelection(row);
}

string ComboBox::text() {
  return state.text(selection());
}

string ComboBox::text(unsigned row) {
  return state.text(row);
}

ComboBox::ComboBox():
state(*new State),
base_from_member<pComboBox&>(*new pComboBox(*this)),
Widget(base_from_member<pComboBox&>::value),
p(base_from_member<pComboBox&>::value) {
  p.constructor();
}

ComboBox::~ComboBox() {
  p.destructor();
  delete &state;
}

//HexEdit
//=======

void HexEdit::setColumns(unsigned columns) {
  state.columns = columns;
  return p.setColumns(columns);
}

void HexEdit::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HexEdit::setOffset(unsigned offset) {
  state.offset = offset;
  return p.setOffset(offset);
}

void HexEdit::setRows(unsigned rows) {
  state.rows = rows;
  return p.setRows(rows);
}

void HexEdit::update() {
  return p.update();
}

HexEdit::HexEdit():
state(*new State),
base_from_member<pHexEdit&>(*new pHexEdit(*this)),
Widget(base_from_member<pHexEdit&>::value),
p(base_from_member<pHexEdit&>::value) {
  p.constructor();
}

HexEdit::~HexEdit() {
  p.destructor();
  delete &state;
}

//HorizontalScrollBar
//===================

unsigned HorizontalScrollBar::length() {
  return state.length;
}

unsigned HorizontalScrollBar::position() {
  return p.position();
}

void HorizontalScrollBar::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HorizontalScrollBar::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

HorizontalScrollBar::HorizontalScrollBar():
state(*new State),
base_from_member<pHorizontalScrollBar&>(*new pHorizontalScrollBar(*this)),
Widget(base_from_member<pHorizontalScrollBar&>::value),
p(base_from_member<pHorizontalScrollBar&>::value) {
  p.constructor();
}

HorizontalScrollBar::~HorizontalScrollBar() {
  p.destructor();
  delete &state;
}

//HorizontalSlider
//================

unsigned HorizontalSlider::length() {
  return state.length;
}

unsigned HorizontalSlider::position() {
  return p.position();
}

void HorizontalSlider::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void HorizontalSlider::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

HorizontalSlider::HorizontalSlider():
state(*new State),
base_from_member<pHorizontalSlider&>(*new pHorizontalSlider(*this)),
Widget(base_from_member<pHorizontalSlider&>::value),
p(base_from_member<pHorizontalSlider&>::value) {
  p.constructor();
}

HorizontalSlider::~HorizontalSlider() {
  p.destructor();
  delete &state;
}

//Label
//=====

void Label::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

Label::Label():
state(*new State),
base_from_member<pLabel&>(*new pLabel(*this)),
Widget(base_from_member<pLabel&>::value),
p(base_from_member<pLabel&>::value) {
  p.constructor();
}

Label::~Label() {
  p.destructor();
  delete &state;
}

//LineEdit
//========

void LineEdit::setEditable(bool editable) {
  state.editable = editable;
  return p.setEditable(editable);
}

void LineEdit::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

string LineEdit::text() {
  return p.text();
}

LineEdit::LineEdit():
state(*new State),
base_from_member<pLineEdit&>(*new pLineEdit(*this)),
Widget(base_from_member<pLineEdit&>::value),
p(base_from_member<pLineEdit&>::value) {
  p.constructor();
}

LineEdit::~LineEdit() {
  p.destructor();
  delete &state;
}

//ListView
//========

void ListView::append_(const lstring &text) {
  state.checked.append(false);
  state.text.append(text);
  return p.append(text);
}

void ListView::autoSizeColumns() {
  return p.autoSizeColumns();
}

bool ListView::checked(unsigned row) {
  return p.checked(row);
}

void ListView::modify_(unsigned row, const lstring &text) {
  state.text[row] = text;
  return p.modify(row, text);
}

void ListView::remove(unsigned row) {
  state.text.remove(row);
  state.image.remove(row);
  return p.remove(row);
}

void ListView::reset() {
  state.checked.reset();
  state.image.reset();
  state.text.reset();
  return p.reset();
}

bool ListView::selected() {
  return p.selected();
}

unsigned ListView::selection() {
  return p.selection();
}

void ListView::setCheckable(bool checkable) {
  state.checkable = checkable;
  return p.setCheckable(checkable);
}

void ListView::setChecked(unsigned row, bool checked) {
  state.checked[row] = checked;
  return p.setChecked(row, checked);
}

void ListView::setHeaderText_(const lstring &text) {
  state.headerText = text;
  return p.setHeaderText(text);
}

void ListView::setHeaderVisible(bool visible) {
  state.headerVisible = visible;
  return p.setHeaderVisible(visible);
}

void ListView::setImage(unsigned row, unsigned column, const nall::image &image) {
  state.image(row)(column) = image;
  return p.setImage(row, column, image);
}

void ListView::setSelected(bool selected) {
  state.selected = selected;
  return p.setSelected(selected);
}

void ListView::setSelection(unsigned row) {
  state.selected = true;
  state.selection = row;
  return p.setSelection(row);
}

ListView::ListView():
state(*new State),
base_from_member<pListView&>(*new pListView(*this)),
Widget(base_from_member<pListView&>::value),
p(base_from_member<pListView&>::value) {
  p.constructor();
}

ListView::~ListView() {
  p.destructor();
  delete &state;
}

//ProgressBar
//===========

void ProgressBar::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

ProgressBar::ProgressBar():
state(*new State),
base_from_member<pProgressBar&>(*new pProgressBar(*this)),
Widget(base_from_member<pProgressBar&>::value),
p(base_from_member<pProgressBar&>::value) {
  p.constructor();
}

ProgressBar::~ProgressBar() {
  p.destructor();
  delete &state;
}

//RadioBox
//========

void RadioBox::group(const set<RadioBox&> &list) {
  for(auto &item : list) item.p.setGroup(item.state.group = list);
  if(list.size()) list[0].setChecked();
}

bool RadioBox::checked() {
  return p.checked();
}

void RadioBox::setChecked() {
  for(auto &item : state.group) item.state.checked = false;
  state.checked = true;
  return p.setChecked();
}

void RadioBox::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

RadioBox::RadioBox():
state(*new State),
base_from_member<pRadioBox&>(*new pRadioBox(*this)),
Widget(base_from_member<pRadioBox&>::value),
p(base_from_member<pRadioBox&>::value) {
  p.constructor();
}

RadioBox::~RadioBox() {
  for(auto &item : state.group) {
    if(&item != this) item.state.group.remove(*this);
  }
  p.destructor();
  delete &state;
}

//TextEdit
//========

void TextEdit::setCursorPosition(unsigned position) {
  state.cursorPosition = position;
  return p.setCursorPosition(position);
}

void TextEdit::setEditable(bool editable) {
  state.editable = editable;
  return p.setEditable(editable);
}

void TextEdit::setText(const string &text) {
  state.text = text;
  return p.setText(text);
}

void TextEdit::setWordWrap(bool wordWrap) {
  state.wordWrap = wordWrap;
  return p.setWordWrap(wordWrap);
}

string TextEdit::text() {
  return p.text();
}

TextEdit::TextEdit():
state(*new State),
base_from_member<pTextEdit&>(*new pTextEdit(*this)),
Widget(base_from_member<pTextEdit&>::value),
p(base_from_member<pTextEdit&>::value) {
  p.constructor();
}

TextEdit::~TextEdit() {
  p.destructor();
  delete &state;
}

//VerticalScrollBar
//=================

unsigned VerticalScrollBar::length() {
  return state.length;
}

unsigned VerticalScrollBar::position() {
  return p.position();
}

void VerticalScrollBar::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void VerticalScrollBar::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

VerticalScrollBar::VerticalScrollBar():
state(*new State),
base_from_member<pVerticalScrollBar&>(*new pVerticalScrollBar(*this)),
Widget(base_from_member<pVerticalScrollBar&>::value),
p(base_from_member<pVerticalScrollBar&>::value) {
  p.constructor();
}

VerticalScrollBar::~VerticalScrollBar() {
  p.destructor();
  delete &state;
}

//VerticalSlider
//==============

unsigned VerticalSlider::length() {
  return state.length;
}

unsigned VerticalSlider::position() {
  return p.position();
}

void VerticalSlider::setLength(unsigned length) {
  state.length = length;
  return p.setLength(length);
}

void VerticalSlider::setPosition(unsigned position) {
  state.position = position;
  return p.setPosition(position);
}

VerticalSlider::VerticalSlider():
state(*new State),
base_from_member<pVerticalSlider&>(*new pVerticalSlider(*this)),
Widget(base_from_member<pVerticalSlider&>::value),
p(base_from_member<pVerticalSlider&>::value) {
  p.constructor();
}

VerticalSlider::~VerticalSlider() {
  p.destructor();
  delete &state;
}

//Viewport
//========

uintptr_t Viewport::handle() {
  return p.handle();
}

Viewport::Viewport():
base_from_member<pViewport&>(*new pViewport(*this)),
Widget(base_from_member<pViewport&>::value),
p(base_from_member<pViewport&>::value) {
  p.constructor();
}

Viewport::~Viewport() {
  p.destructor();
}
