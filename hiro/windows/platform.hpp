namespace phoenix {

struct AppMessage {
  enum : unsigned {
    ListView_onActivate = 0,
  };
};

typedef LRESULT CALLBACK (*WindowProc)(HWND, UINT, WPARAM, LPARAM);

struct pApplication {
  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

struct Settings {
  bimap<Keyboard::Scancode, unsigned> keymap;
};

struct pFont;
struct pObject;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(string font, string text);

  static HFONT create(string description);
  static void free(HFONT hfont);
  static Size size(HFONT hfont, string text);
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
  static vector<pObject*> objects;

  Object& object;
  uintptr_t id;
  bool locked;

  pObject(Object& object);
  static Object* find(unsigned id);
  virtual ~pObject() {}

  void constructor() {}
  void destructor() {}
};

struct pTimer : public pObject {
  Timer& timer;
  UINT_PTR htimer;

  void setEnabled(bool enabled);
  void setInterval(unsigned interval);

  pTimer(Timer& timer) : pObject(timer), timer(timer) {}
  void constructor();
};

struct pWindow : public pObject {
  static vector<pWindow*> modal;
  static void updateModality();

  Window& window;
  HWND hwnd;
  HMENU hmenu;
  HWND hstatus;
  HFONT hstatusfont;
  HBRUSH brush;
  COLORREF brushColor;

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
  void destructor();
  void updateMenu();

  void onClose();
  void onDrop(WPARAM wparam);
  bool onEraseBackground();
  void onModalBegin();
  void onModalEnd();
  void onMove();
  void onSize();
};

struct pAction : public pObject {
  Action& action;
  Menu* parentMenu;
  Window* parentWindow;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action& action) : pObject(action), action(action) {}
  void constructor();
};

struct pMenu : public pAction {
  Menu& menu;
  HMENU hmenu;
  HBITMAP hbitmap;

  void append(Action& action);
  void remove(Action& action);
  void setImage(const image& image);
  void setText(string text);

  pMenu(Menu& menu) : pAction(menu), menu(menu), hbitmap(0) {}
  void constructor();
  void destructor();
  void createBitmap();
  void update(Window& parentWindow, Menu* parentMenu = nullptr);
};

struct pSeparator : public pAction {
  Separator& separator;

  pSeparator(Separator& separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

struct pItem : public pAction {
  Item& item;
  HBITMAP hbitmap;

  void setImage(const image& image);
  void setText(string text);

  pItem(Item& item) : pAction(item), item(item), hbitmap(0) {}
  void constructor();
  void destructor();
  void createBitmap();

  void onActivate();
};

struct pCheckItem : public pAction {
  CheckItem& checkItem;

  void setChecked(bool checked);
  void setText(string text);

  pCheckItem(CheckItem& checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();

  void onToggle();
};

struct pRadioItem : public pAction {
  RadioItem& radioItem;

  void setChecked();
  void setGroup(const group<RadioItem>& group);
  void setText(string text);

  pRadioItem(RadioItem& radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();

  void onActivate();
};

struct pSizable : public pObject {
  Sizable& sizable;

  pSizable(Sizable& sizable) : pObject(sizable), sizable(sizable) {}
};

struct pLayout : public pSizable {
  Layout& layout;

  pLayout(Layout& layout) : pSizable(layout), layout(layout) {}
};

struct pWidget : public pSizable {
  Widget& widget;
  HWND parentHwnd;
  HWND hwnd;
  HFONT hfont;

  bool focused();
  virtual Size minimumSize();
  virtual void setEnabled(bool enabled);
  void setFocused();
  void setFont(string font);
  virtual void setGeometry(Geometry geometry);
  virtual void setVisible(bool visible);

  pWidget(Widget& widget) : pSizable(widget), widget(widget) { parentHwnd = pWindow::none().p.hwnd; }
  void constructor();
  void destructor();
  virtual void orphan();
  void setDefaultFont();
  void synchronize();
};

struct pButton : public pWidget {
  Button& button;
  HBITMAP hbitmap;
  HIMAGELIST himagelist;

  Size minimumSize();
  void setBordered(bool bordered);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pButton(Button& button) : pWidget(button), button(button), hbitmap(0), himagelist(0) {}
  void constructor();
  void destructor();
  void orphan();

  void onActivate();
};

struct pCanvas : public pWidget {
  Canvas& canvas;
  uint32_t* surface = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;

  void setDroppable(bool droppable);
  void setGeometry(Geometry geometry);
  void setMode(Canvas::Mode mode);
  void setSize(Size size);

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void orphan();
  void paint();
  void rasterize();
  void redraw();
  void release();
};

struct pCheckButton : public pWidget {
  CheckButton& checkButton;
  HBITMAP hbitmap;
  HIMAGELIST himagelist;

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

  void onToggle();
};

struct pComboButton : public pWidget {
  ComboButton& comboButton;

  void append(string text);
  void remove(unsigned selection);
  Size minimumSize();
  void reset();
  void setGeometry(Geometry geometry);
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);

  pComboButton(ComboButton& comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
  void orphan();

  void onChange();
};

struct pConsole : public pWidget {
  Console& console;
  LRESULT CALLBACK (*windowProc)(HWND, UINT, LPARAM, WPARAM);
  HBRUSH backgroundBrush = nullptr;

  void print(string text);
  void reset();
  void setBackgroundColor(Color color);
  void setForegroundColor(Color color);
  void setPrompt(string prompt);

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
  void orphan();
  bool keyPress(unsigned key);
};

struct pFrame : public pWidget {
  Frame& frame;

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
  WindowProc windowProc = nullptr;
  HWND scrollBar = nullptr;
  HBRUSH backgroundBrush = nullptr;

  void setBackgroundColor(Color color);
  void setColumns(unsigned columns);
  void setForegroundColor(Color color);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit& hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
  void orphan();
  bool keyPress(unsigned key);
  signed rows();
  signed rowsScrollable();
  signed scrollPosition();
  void scrollTo(signed position);
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

  void onChange(WPARAM wparam);
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

  void onChange();
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
  HBRUSH backgroundBrush = nullptr;

  Size minimumSize();
  void setBackgroundColor(Color color);
  void setEditable(bool editable);
  void setForegroundColor(Color color);
  void setText(string text);
  string text();

  pLineEdit(LineEdit& lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
  void orphan();

  void onChange();
};

struct pListView : public pWidget {
  ListView& listView;
  HIMAGELIST imageList;
  vector<vector<unsigned>> imageMap;
  vector<image> images;
  bool lostFocus;

  void append(const lstring& text);
  void autoSizeColumns();
  void remove(unsigned selection);
  void reset();
  void setBackgroundColor(Color color);
  void setCheckable(bool checkable);
  void setChecked(unsigned selection, bool checked);
  void setForegroundColor(Color color);
  void setGeometry(Geometry geometry);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned selection, unsigned position, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned selection);
  void setText(unsigned selection, unsigned position, string text);

  pListView(ListView& listView) : pWidget(listView), listView(listView), imageList(nullptr) {}
  void constructor();
  void destructor();
  void orphan();
  void buildImageList();

  void onActivate(LPARAM lparam);
  void onChange(LPARAM lparam);
  LRESULT onCustomDraw(LPARAM lparam);
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
  HBITMAP hbitmap;
  HIMAGELIST himagelist;

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
};

struct pRadioLabel : public pWidget {
  RadioLabel& radioLabel;

  Size minimumSize();
  void setChecked();
  void setGroup(const group<RadioLabel>& group);
  void setText(string text);

  pRadioLabel(RadioLabel& radioLabel) : pWidget(radioLabel), radioLabel(radioLabel) {}
  void constructor();
  void destructor();
  void orphan();

  void onActivate();
};

struct pTabFrame : public pWidget {
  TabFrame& tabFrame;
  WindowProc windowProc = nullptr;
  HIMAGELIST imageList = nullptr;

  void append(string text, const image& image);
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
  void buildImageList();
  void synchronizeLayout();

  void onChange();
  void onDrawItem(LPARAM lparam);
};

struct pTextEdit : public pWidget {
  TextEdit& textEdit;
  HBRUSH backgroundBrush = nullptr;

  void setBackgroundColor(Color color);
  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setForegroundColor(Color color);
  void setText(string text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit& textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
  void orphan();

  void onChange();
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

  void onChange(WPARAM wparam);
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

  void onChange();
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
