struct Timer::State {
  bool enabled;
  unsigned milliseconds;

  State() {
    enabled = false;
    milliseconds = 0;
  }
};

struct Window::State {
  bool backgroundColorOverride;
  Color backgroundColor;
  bool fullScreen;
  Geometry geometry;
  bool ignore;
  array<Layout&> layout;
  array<Menu&> menu;
  string menuFont;
  bool menuVisible;
  bool resizable;
  string statusFont;
  string statusText;
  bool statusVisible;
  string title;
  bool visible;
  array<Widget&> widget;
  string widgetFont;

  State() {
    backgroundColorOverride = false;
    backgroundColor = { 0, 0, 0, 255 };
    fullScreen = false;
    geometry = { 128, 128, 256, 256 };
    ignore = false;
    menuVisible = false;
    resizable = true;
    statusVisible = false;
    visible = false;
  }
};

struct Action::State {
  bool enabled;
  Menu *menu;
  bool visible;
  Window *window;

  State() {
    enabled = true;
    menu = 0;
    visible = true;
    window = 0;
  }
};

struct Menu::State {
  array<Action&> action;
  nall::image image;
  string text;

  State() : image(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0) {
  }
};

struct Item::State {
  nall::image image;
  string text;

  State() : image(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0) {
  }
};

struct CheckItem::State {
  bool checked;
  string text;

  State() {
    checked = false;
  }
};

struct RadioItem::State {
  bool checked;
  array<RadioItem&> group;
  string text;

  State() {
    checked = true;
  }
};

struct Sizable::State {
  Layout *layout;
  Window *window;

  State() {
    layout = 0;
    window = 0;
  }
};

struct Layout::State {
  State() {
  }
};

struct Widget::State {
  bool abstract;
  bool enabled;
  string font;
  Geometry geometry;
  bool visible;

  State() {
    abstract = false;
    enabled = true;
    geometry = { 0, 0, 0, 0 };
    visible = true;
  }
};

struct Button::State {
  nall::image image;
  Orientation orientation;
  string text;

  State() : image(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0) {
  }
};

struct Canvas::State {
  uint32_t *data;
  unsigned width;
  unsigned height;

  State() {
    data = nullptr;
    width = 256;
    height = 256;
  }
};

struct CheckBox::State {
  bool checked;
  string text;

  State() {
    checked = false;
  }
};

struct ComboBox::State {
  unsigned selection;
  vector<string> text;

  State() {
    selection = 0;
  }
};

struct HexEdit::State {
  unsigned columns;
  unsigned length;
  unsigned offset;
  unsigned rows;

  State() {
    columns = 16;
    length = 0;
    offset = 0;
    rows = 16;
  }
};

struct HorizontalScrollBar::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

struct HorizontalSlider::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

struct Label::State {
  string text;
};

struct LineEdit::State {
  bool editable;
  string text;

  State() {
    editable = true;
  }
};

struct ListView::State {
  bool checkable;
  array<bool> checked;
  lstring headerText;
  bool headerVisible;
  bool selected;
  unsigned selection;
  vector<lstring> text;

  State() {
    checkable = false;
    headerVisible = false;
    selected = false;
    selection = 0;
  }
};

struct ProgressBar::State {
  unsigned position;

  State() {
    position = 0;
  }
};

struct RadioBox::State {
  bool checked;
  array<RadioBox&> group;
  string text;

  State() {
    checked = true;
  }
};

struct TextEdit::State {
  unsigned cursorPosition;
  bool editable;
  string text;
  bool wordWrap;

  State() {
    cursorPosition = 0;
    editable = true;
    wordWrap = false;
  }
};

struct VerticalScrollBar::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

struct VerticalSlider::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};
