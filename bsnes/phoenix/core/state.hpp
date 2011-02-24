struct Font::State {
  bool bold;
  nall::string family;
  bool italic;
  unsigned size;
  bool underline;

  State() {
    bold = false;
    italic = false;
    size = 8;
    underline = false;
  }
};

struct Window::State {
  bool backgroundColor;
  unsigned backgroundColorRed, backgroundColorGreen, backgroundColorBlue;
  bool fullScreen;
  Geometry geometry;
  nall::reference_array<Layout&> layout;
  nall::reference_array<Menu&> menu;
  Font *menuFont;
  bool menuVisible;
  bool resizable;
  Font *statusFont;
  nall::string statusText;
  bool statusVisible;
  nall::string title;
  bool visible;
  nall::reference_array<Widget&> widget;
  Font *widgetFont;

  State() {
    backgroundColor = false;
    backgroundColorRed = 0;
    backgroundColorGreen = 0;
    backgroundColorBlue = 0;
    fullScreen = false;
    geometry = { 128, 128, 256, 256 };
    menuFont = 0;
    menuVisible = false;
    resizable = true;
    statusVisible = false;
    visible = false;
    widgetFont = 0;
  }
};

struct Action::State {
  bool enabled;
  Window *parent;
  bool visible;

  State() {
    enabled = true;
    parent = 0;
    visible = true;
  }
};

struct Menu::State {
  nall::reference_array<Action&> action;
  nall::string text;
};

struct MenuItem::State {
  nall::string text;
};

struct MenuCheckItem::State {
  bool checked;
  nall::string text;

  State() {
    checked = false;
  }
};

struct MenuRadioItem::State {
  bool checked;
  nall::reference_array<MenuRadioItem&> group;
  nall::string text;

  State() {
    checked = true;
  }
};

struct Widget::State {
  bool abstract;
  bool enabled;
  Font *font;
  Geometry geometry;
  bool visible;

  State() {
    abstract = false;
    enabled = true;
    font = 0;
    geometry = { 0, 0, 0, 0 };
    visible = true;
  }
};

struct Button::State {
  nall::string text;

  State() {
  }
};

struct CheckBox::State {
  bool checked;
  nall::string text;

  State() {
    checked = false;
  }
};

struct ComboBox::State {
  unsigned selection;
  nall::linear_vector<nall::string> text;

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

struct HorizontalSlider::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

struct Label::State {
  nall::string text;
};

struct LineEdit::State {
  bool editable;
  nall::string text;

  State() {
    editable = true;
  }
};

struct ListView::State {
  bool checkable;
  nall::array<bool> checked;
  nall::lstring headerText;
  bool headerVisible;
  nall::optional<unsigned> selection;
  nall::linear_vector<nall::lstring> text;

  State() : selection(false, 0) {
    checkable = false;
    headerVisible = false;
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
  nall::reference_array<RadioBox&> group;
  nall::string text;

  State() {
    checked = true;
  }
};

struct TextEdit::State {
  unsigned cursorPosition;
  bool editable;
  nall::string text;
  bool wordWrap;

  State() {
    cursorPosition = 0;
    editable = true;
    wordWrap = false;
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
