namespace phoenix {

struct pApplication {
  static XlibDisplay* display;

  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

struct Settings : Configuration::Document {
  bimap<Keyboard::Scancode, unsigned> keymap;

  struct Geometry : Configuration::Node {
    unsigned frameX;
    unsigned frameY;
    unsigned frameWidth;
    unsigned frameHeight;
    unsigned menuHeight;
    unsigned statusHeight;
  } geometry;

  struct Window : Configuration::Node {
    unsigned backgroundColor;
  } window;

  void load();
  void save();
  Settings();
};

struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(string font, string text);

  static PangoFontDescription* create(string description);
  static void free(PangoFontDescription* font);
  static Size size(PangoFontDescription* font, string text);
  static void setFont(GtkWidget* widget, string font);
  static void setFont(GtkWidget* widget, gpointer font);
};

struct pDesktop {
  static Size size();
  static Geometry workspace();
};

struct pMonitor {
  static unsigned count();
  static Geometry geometry(unsigned monitor);
  static unsigned primary();
};

struct pKeyboard {
  static bool pressed(Keyboard::Scancode scancode);
  static vector<bool> state();

  static void initialize();
};

struct pMouse {
  static Position position();
  static bool pressed(Mouse::Button button);
};

struct pBrowserWindow {
  static string directory(BrowserWindow::State& state);
  static string open(BrowserWindow::State& state);
  static string save(BrowserWindow::State& state);
};

struct pMessageWindow {
  static MessageWindow::Response error(MessageWindow::State& state);
  static MessageWindow::Response information(MessageWindow::State& state);
  static MessageWindow::Response question(MessageWindow::State& state);
  static MessageWindow::Response warning(MessageWindow::State& state);
};

struct pObject {
  Object& object;
  bool locked;

  pObject(Object& object) : object(object), locked(false) {}
  virtual ~pObject() {}

  void constructor() {}
  void destructor() {}
};

struct pTimer : public pObject {
  Timer& timer;

  void setEnabled(bool enabled);
  void setInterval(unsigned interval);

  pTimer(Timer& timer) : pObject(timer), timer(timer) {}
  void constructor();
};

struct pWindow : public pObject {
  Window& window;
  GtkWidget* widget;
  GtkWidget* menuContainer;
  GtkWidget* formContainer;
  GtkWidget* statusContainer;
  GtkWidget* menu;
  GtkWidget* status;
  GtkAllocation lastAllocation;
  bool onSizePending;

  static Window& none();

  void append(Layout& layout);
  void append(Menu& menu);
  void append(Widget& widget);
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout& layout);
  void remove(Menu& menu);
  void remove(Widget& widget);
  void setBackgroundColor(Color color);
  void setDroppable(bool droppable);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(Geometry geometry);
  void setMenuFont(string font);
  void setMenuVisible(bool visible);
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(string font);
  void setStatusText(string text);
  void setStatusVisible(bool visible);
  void setTitle(string text);
  void setVisible(bool visible);
  void setWidgetFont(string font);

  pWindow(Window& window) : pObject(window), window(window) {}
  void constructor();
  unsigned menuHeight();
  unsigned statusHeight();
};

struct pAction : public pObject {
  Action& action;
  GtkWidget* widget;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action& action) : pObject(action), action(action) {}
  void constructor();
  virtual void orphan();
  string mnemonic(string text);
  virtual void setFont(string font);
};

struct pMenu : public pAction {
  Menu& menu;
  GtkWidget* gtkMenu;

  void append(Action& action);
  void remove(Action& action);
  void setImage(const image& image);
  void setText(string text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
  void orphan();
  void setFont(string font);
};

struct pSeparator : public pAction {
  Separator& separator;

  pSeparator(Separator& separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pItem : public pAction {
  Item& item;

  void setImage(const image& image);
  void setText(string text);

  pItem(Item& item) : pAction(item), item(item) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pCheckItem : public pAction {
  CheckItem& checkItem;

  void setChecked(bool checked);
  void setText(string text);

  pCheckItem(CheckItem& checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pRadioItem : public pAction {
  RadioItem& radioItem;

  void setChecked();
  void setGroup(const group<RadioItem>& group);
  void setText(string text);

  pRadioItem(RadioItem& radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
  void orphan();
  void onActivate();
  pRadioItem& parent();
};

struct pSizable : public pObject {
  Sizable& sizable;

  virtual Position displacement() { return {0, 0}; }

  pSizable(Sizable &sizable) : pObject(sizable), sizable(sizable) {}
};

struct pLayout : public pSizable {
  Layout& layout;

  pLayout(Layout& layout) : pSizable(layout), layout(layout) {}
};

struct pWidget : public pSizable {
  Widget& widget;
  GtkWidget* gtkWidget = nullptr;
  GtkWidget* gtkParent = nullptr;

  virtual GtkWidget* container(Widget& widget);
  virtual bool focused();
  virtual Size minimumSize();
  virtual void setEnabled(bool enabled);
  virtual void setFocused();
  virtual void setFont(string font);
  virtual void setGeometry(Geometry geometry);
  virtual void setVisible(bool visible);

  pWidget(Widget& widget) : pSizable(widget), widget(widget) {}
  void constructor();
  void destructor();
  virtual void orphan();
};

struct pButton : public pWidget {
  Button& button;

  Size minimumSize();
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pButton(Button& button) : pWidget(button), button(button) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pCanvas : public pWidget {
  Canvas& canvas;
  GdkPixbuf* surface = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;

  Size minimumSize();
  void setDroppable(bool droppable);
  void setGeometry(Geometry geometry);
  void setMode(Canvas::Mode mode);
  void setSize(Size size);

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void orphan();
  void onExpose(GdkEventExpose* event);
  void rasterize();
  void redraw();
  void release();
};

struct pCheckButton : public pWidget {
  CheckButton& checkButton;

  Size minimumSize();
  void setChecked(bool checked);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pCheckButton(CheckButton& checkButton) : pWidget(checkButton), checkButton(checkButton) {}
  void constructor();
  void destructor();
  void orphan();
  void onToggle();
};

struct pCheckLabel : public pWidget {
  CheckLabel& checkLabel;

  Size minimumSize();
  void setChecked(bool checked);
  void setText(string text);

  pCheckLabel(CheckLabel& checkLabel) : pWidget(checkLabel), checkLabel(checkLabel) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pComboButton : public pWidget {
  ComboButton& comboButton;
  unsigned itemCounter;

  void append(string text);
  Size minimumSize();
  void remove(unsigned selection);
  void reset();
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);

  pComboButton(ComboButton& comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pConsole : public pWidget {
  Console& console;
  GtkWidget* subWidget;
  GtkTextBuffer* textBuffer;
  string previousPrompt;
  lstring history;
  unsigned historyOffset = 0;

  void print(string text);
  void reset();
  void setPrompt(string prompt);

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
  void orphan();
  bool keyPress(unsigned scancode, unsigned mask);
  void seekToEnd();
  void seekToMark();
};

struct pFrame : public pWidget {
  Frame& frame;

  GtkWidget* container(Widget& widget);
  Position containerOffset();
  void setEnabled(bool enabled);
  void setGeometry(Geometry geometry);
  void setText(string text);
  void setVisible(bool visible);

  pFrame(Frame& frame) : pWidget(frame), frame(frame) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pHexEdit : public pWidget {
  HexEdit& hexEdit;
  GtkWidget* container;
  GtkWidget* subWidget;
  GtkWidget* scrollBar;
  GtkTextBuffer* textBuffer;
  GtkTextMark* textCursor;

  bool focused();
  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit& hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
  void orphan();
  unsigned cursorPosition();
  bool keyPress(unsigned scancode, unsigned mask);
  signed rows();
  signed rowsScrollable();
  void scroll(signed position);
  void setCursorPosition(unsigned position);
  void setScroll();
  void updateScroll();
};

struct pHorizontalScroller : public pWidget {
  HorizontalScroller& horizontalScroller;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScroller(HorizontalScroller& horizontalScroller) : pWidget(horizontalScroller), horizontalScroller(horizontalScroller) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider& horizontalSlider;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider& horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pLabel : public pWidget {
  Label& label;

  Size minimumSize();
  void setText(string text);

  pLabel(Label& label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pLineEdit : public pWidget {
  LineEdit& lineEdit;

  Size minimumSize();
  void setEditable(bool editable);
  void setText(string text);
  string text();

  pLineEdit(LineEdit& lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pListView : public pWidget {
  ListView& listView;
  GtkWidget* subWidget;
  GtkListStore* store;
  struct GtkColumn {
    GtkTreeViewColumn* column;
    GtkCellRenderer* checkbutton;
    GtkCellRenderer* icon;
    GtkCellRenderer* text;
    GtkWidget *label;
  };
  vector<GtkColumn> column;

  void append(const lstring& text);
  void autoSizeColumns();
  bool focused();
  void remove(unsigned selection);
  void reset();
  void setCheckable(bool checkable);
  void setChecked(unsigned selection, bool checked);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned selection, unsigned position, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned row);
  void setText(unsigned selection, unsigned position, string text);

  pListView(ListView& listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
  void orphan();
  void setFocused();
  void setFont(string font);
};

struct pProgressBar : public pWidget {
  ProgressBar& progressBar;

  Size minimumSize();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar& progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pRadioButton : public pWidget {
  RadioButton& radioButton;

  Size minimumSize();
  void setChecked();
  void setGroup(const group<RadioButton>& group);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pRadioButton(RadioButton& radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  void constructor();
  void destructor();
  void orphan();
  void onActivate();
  pRadioButton& parent();
};

struct pRadioLabel : public pWidget {
  RadioLabel& radioLabel;

  Size minimumSize();
  void setChecked();
  void setGroup(const group<RadioLabel>& group);
  void setText(string text);

  pRadioLabel(RadioLabel& radioLabel) : pWidget(radioLabel), radioLabel(radioLabel) {}
  void onActivate();
  pRadioLabel& parent();
  void constructor();
  void destructor();
  void orphan();
};

struct pTabFrame : public pWidget {
  TabFrame& tabFrame;
  struct Tab {
    GtkWidget* child;
    GtkWidget* container;
    GtkWidget* layout;
    GtkWidget* image;
    GtkWidget* title;
  };
  vector<Tab> tabs;

  void append(string text, const image& image);
  GtkWidget* container(Widget& widget);
  Position displacement();
  void remove(unsigned selection);
  void setEnabled(bool enabled);
  void setGeometry(Geometry geometry);
  void setImage(unsigned selection, const image& image);
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);
  void setVisible(bool visible);

  pTabFrame(TabFrame& tabFrame) : pWidget(tabFrame), tabFrame(tabFrame) {}
  void constructor();
  void destructor();
  void orphan();
  void setFont(string font);
  void synchronizeLayout();
};

struct pTextEdit : public pWidget {
  TextEdit& textEdit;
  GtkWidget* subWidget;
  GtkTextBuffer* textBuffer;

  bool focused();
  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(string text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit& textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pVerticalScroller : public pWidget {
  VerticalScroller& verticalScroller;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller& verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pVerticalSlider : public pWidget {
  VerticalSlider& verticalSlider;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider& verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pViewport : public pWidget {
  Viewport& viewport;

  uintptr_t handle();
  void setDroppable(bool droppable);

  pViewport(Viewport& viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
  void orphan();
};

}
