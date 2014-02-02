struct Application::State {
  string name;
  bool quit = false;
} applicationState;

struct Timer::State {
  bool enabled = false;
  unsigned interval = 0;
};

struct BrowserWindow::State {
  lstring filters;
  Window* parent = nullptr;
  string path;
  string title;
};

struct MessageWindow::State {
  MessageWindow::Buttons buttons = MessageWindow::Buttons::Ok;
  Window* parent = nullptr;
  string text;
  string title;
};

struct Window::State {
  bool backgroundColorOverride = false;
  Color backgroundColor = {0, 0, 0, 255};
  bool droppable = false;
  bool fullScreen = false;
  Geometry geometry = {128, 128, 256, 256};
  group<Layout> layout;
  group<Menu> menu;
  string menuFont;
  bool menuVisible = false;
  bool modal = false;
  bool resizable = true;
  string statusFont;
  string statusText;
  bool statusVisible = false;
  string title;
  bool visible = false;
  group<Widget> widget;
  string widgetFont;
};

struct Action::State {
  bool enabled = true;
  Menu* menu = nullptr;
  bool visible = true;
  Window* window = nullptr;
};

struct Menu::State {
  group<Action> action;
  nall::image image;
  string text;
};

struct Item::State {
  nall::image image;
  string text;
};

struct CheckItem::State {
  bool checked = false;
  string text;
};

struct RadioItem::State {
  bool checked = true;
  nall::group<RadioItem> group;
  string text;
};

struct Sizable::State {
  bool enabled = true;
  Sizable* parent = nullptr;
  bool visible = true;
  Window* window = nullptr;
};

struct Layout::State {
  Widget* widget = nullptr;
  unsigned widgetSelection = 0;
};

struct Widget::State {
  bool abstract = false;
  string font;
  Geometry geometry = {0, 0, 0, 0};
};

struct Button::State {
  nall::image image;
  Orientation orientation = Orientation::Horizontal;
  string text;
};

struct Canvas::State {
  Color color;
  uint32_t* data = nullptr;
  bool droppable = false;
  vector<Color> gradient = {{}, {}, {}, {}};
  nall::image image;
  Canvas::Mode mode = Canvas::Mode::Color;
  unsigned width = 0;
  unsigned height = 0;
};

struct CheckButton::State {
  bool checked = false;
  nall::image image;
  Orientation orientation = Orientation::Horizontal;
  string text;
};

struct CheckLabel::State {
  bool checked = false;
  string text;
};

struct ComboButton::State {
  unsigned selection = 0;
  vector<string> text;
};

struct Console::State {
  string prompt;
};

struct Frame::State {
  Layout* layout = nullptr;
  string text;
};

struct HexEdit::State {
  unsigned columns = 16;
  unsigned length = 0;
  unsigned offset = 0;
  unsigned rows = 16;
};

struct HorizontalScroller::State {
  unsigned length = 101;
  unsigned position = 0;
};

struct HorizontalSlider::State {
  unsigned length = 101;
  unsigned position = 0;
};

struct Label::State {
  string text;
};

struct LineEdit::State {
  bool editable = true;
  string text;
};

struct ListView::State {
  bool checkable = false;
  vector<bool> checked;
  lstring headerText;
  bool headerVisible = false;
  vector<vector<nall::image>> image;
  bool selected = false;
  unsigned selection = 0;
  vector<lstring> text;
};

struct ProgressBar::State {
  unsigned position = 0;
};

struct RadioButton::State {
  bool checked = true;
  nall::group<RadioButton> group;
  nall::image image;
  Orientation orientation = Orientation::Horizontal;
  string text;
};

struct RadioLabel::State {
  bool checked = true;
  nall::group<RadioLabel> group;
  string text;
};

struct TabFrame::State {
  vector<nall::image> image;
  vector<Layout*> layout;
  unsigned selection = 0;
  lstring text;
};

struct TextEdit::State {
  unsigned cursorPosition = 0;
  bool editable = true;
  string text;
  bool wordWrap = true;
};

struct VerticalScroller::State {
  unsigned length = 101;
  unsigned position = 0;
};

struct VerticalSlider::State {
  unsigned length = 101;
  unsigned position = 0;
};

struct Viewport::State {
  bool droppable = false;
};
