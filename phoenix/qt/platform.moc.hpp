namespace phoenix {

struct pApplication {
  static XlibDisplay* display;

  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
  static void syncX();
};

static QApplication* qtApplication = nullptr;

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

  static QFont create(string description);
  static Size size(const QFont& qtFont, string text);
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

struct pTimer : public QObject, public pObject {
  Q_OBJECT

public:
  Timer& timer;
  QTimer* qtTimer;

  void setEnabled(bool enabled);
  void setInterval(unsigned interval);

  pTimer(Timer& timer) : pObject(timer), timer(timer) {}
  void constructor();
  void destructor();

public slots:
  void onActivate();
};

struct pWindow : public QObject, public pObject {
  Q_OBJECT

public:
  Window& window;
  struct QtWindow : public QWidget {
    pWindow& self;
    void closeEvent(QCloseEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void moveEvent(QMoveEvent*);
    void resizeEvent(QResizeEvent*);
    QSize sizeHint() const;
    QtWindow(pWindow& self) : self(self) {}
  };
  QtWindow* qtWindow;
  QVBoxLayout* qtLayout;
  QMenuBar* qtMenu;
  QStatusBar* qtStatus;
  QWidget* qtContainer;

  void append(Layout& layout);
  void append(Menu& menu);
  void append(Widget& widget);
  Geometry frameMargin();
  bool focused();
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
  void updateFrameGeometry();
};

struct pAction : public pObject {
  Action& action;

  void setEnabled(bool enabled);
  void setFont(string font);
  void setVisible(bool visible);

  pAction(Action& action) : pObject(action), action(action) {}
  void constructor();
  void destructor();
};

struct pMenu : public pAction {
  Menu& menu;
  QMenu* qtMenu;

  void append(Action& action);
  void remove(Action& action);
  void setFont(string font);
  void setImage(const image& image);
  void setText(string text);

  pMenu(Menu& menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
};

struct pSeparator : public pAction {
  Separator& separator;
  QAction* qtAction;

  pSeparator(Separator& separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

struct pItem : public QObject, public pAction {
  Q_OBJECT

public:
  Item& item;
  QAction* qtAction;

  void setImage(const image& image);
  void setText(string text);

  pItem(Item& item) : pAction(item), item(item) {}
  void constructor();
  void destructor();

public slots:
  void onActivate();
};

struct pCheckItem : public QObject, public pAction {
  Q_OBJECT

public:
  CheckItem& checkItem;
  QAction* qtAction;

  void setChecked(bool checked);
  void setText(string text);

  pCheckItem(CheckItem& checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();

public slots:
  void onToggle();
};

struct pRadioItem : public QObject, public pAction {
  Q_OBJECT

public:
  RadioItem& radioItem;
  QAction* qtAction;
  QActionGroup* qtGroup;

  void setChecked();
  void setGroup(const group<RadioItem>& group);
  void setText(string text);

  pRadioItem(RadioItem& radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();

public slots:
  void onActivate();
};

struct pSizable : public pObject {
  Sizable& sizable;

  virtual Position displacement() { return {0, 0}; }

  pSizable(Sizable& sizable) : pObject(sizable), sizable(sizable) {}

  void constructor() {}
  void destructor() {}
};

struct pLayout : public pSizable {
  Layout& layout;

  pLayout(Layout& layout) : pSizable(layout), layout(layout) {}

  void constructor() {}
  void destructor() {}
};

struct pWidget : public pSizable {
  Widget& widget;
  QWidget* qtWidget;

  virtual QWidget* container(Widget& widget);
  bool focused();
  virtual Size minimumSize();
  virtual void setEnabled(bool enabled);
  void setFocused();
  void setFont(string font);
  virtual void setGeometry(Geometry geometry);
  virtual void setVisible(bool visible);

  pWidget(Widget& widget) : pSizable(widget), widget(widget) {}
  void constructor();
  void synchronizeState();
  void destructor();
  virtual void orphan();
};

struct pButton : public QObject, public pWidget {
  Q_OBJECT

public:
  Button& button;
  QToolButton* qtButton;

  Size minimumSize();
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pButton(Button& button) : pWidget(button), button(button) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
};

struct pCanvas : public QObject, public pWidget {
  Q_OBJECT

public:
  Canvas& canvas;
  QImage* surface = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;
  struct QtCanvas : public QWidget {
    pCanvas& self;
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void leaveEvent(QEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    QtCanvas(pCanvas& self);
  };
  QtCanvas* qtCanvas;

  void setDroppable(bool droppable);
  void setGeometry(Geometry geometry);
  void setMode(Canvas::Mode mode);
  void setSize(Size size);

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void orphan();
  void rasterize();
  void release();

public slots:
};

struct pCheckButton : public QObject, public pWidget {
  Q_OBJECT

public:
  CheckButton& checkButton;
  QToolButton* qtCheckButton;

  Size minimumSize();
  void setChecked(bool checked);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pCheckButton(CheckButton& checkButton) : pWidget(checkButton), checkButton(checkButton) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onToggle(bool checked);
};

struct pCheckLabel : public QObject, public pWidget {
  Q_OBJECT

public:
  CheckLabel& checkLabel;
  QCheckBox* qtCheckLabel;

  Size minimumSize();
  void setChecked(bool checked);
  void setText(string text);

  pCheckLabel(CheckLabel& checkLabel) : pWidget(checkLabel), checkLabel(checkLabel) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onToggle();
};

struct pComboButton : public QObject, public pWidget {
  Q_OBJECT

public:
  ComboButton& comboButton;
  QComboBox* qtComboButton;

  void append(string text);
  Size minimumSize();
  void remove(unsigned selection);
  void reset();
  unsigned selection();
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);

  pComboButton(ComboButton& comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pConsole : public QObject, public pWidget {
  Q_OBJECT

public:
  Console& console;
  struct QtConsole : public QTextEdit {
    pConsole& self;
    void keyPressEvent(QKeyEvent*);
    void keyPressEventAcknowledge(QKeyEvent*);
    QtConsole(pConsole& self) : self(self) {}
  };
  QtConsole* qtConsole;

  void print(string text);
  void reset();

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
  void orphan();
  void keyPressEvent(QKeyEvent*);
};

struct pFrame : public QObject, public pWidget {
  Q_OBJECT

public:
  Frame& frame;
  QGroupBox* qtFrame;

  void setEnabled(bool enabled);
  void setGeometry(Geometry geometry);
  void setText(string text);
  void setVisible(bool visible);

  pFrame(Frame& frame) : pWidget(frame), frame(frame) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pHexEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  HexEdit& hexEdit;
  struct QtHexEdit : public QTextEdit {
    pHexEdit& self;
    void keyPressEvent(QKeyEvent*);
    void keyPressEventAcknowledge(QKeyEvent*);
    void wheelEvent(QWheelEvent*);
    QtHexEdit(pHexEdit& self) : self(self) {}
  };
  struct QtHexEditScrollBar : public QScrollBar {
    pHexEdit& self;
    bool event(QEvent*);
    QtHexEditScrollBar(pHexEdit& self) : QScrollBar(Qt::Vertical), self(self) {}
  };
  QtHexEdit* qtHexEdit;
  QHBoxLayout* qtLayout;
  QtHexEditScrollBar* qtScroll;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit& hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
  void orphan();
  void keyPressEvent(QKeyEvent*);
  signed rows();
  signed rowsScrollable();
  void scrollTo(signed position);

public slots:
  void onScroll();
};

struct pHorizontalScroller : public QObject, public pWidget {
  Q_OBJECT

public:
  HorizontalScroller& horizontalScroller;
  QScrollBar* qtScroller;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScroller(HorizontalScroller& horizontalScroller) : pWidget(horizontalScroller), horizontalScroller(horizontalScroller) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pHorizontalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  HorizontalSlider& horizontalSlider;
  QSlider* qtSlider;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider& horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pLabel : public pWidget {
  Label& label;
  QLabel* qtLabel;

  Size minimumSize();
  void setText(string text);

  pLabel(Label& label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pLineEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  LineEdit& lineEdit;
  QLineEdit* qtLineEdit;

  Size minimumSize();
  void setEditable(bool editable);
  void setText(string text);
  string text();

  pLineEdit(LineEdit& lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
  void onChange();
};

struct pListView : public QObject, public pWidget {
  Q_OBJECT

public:
  ListView& listView;
  QTreeWidget* qtListView;

  void append(const lstring& text);
  void autoSizeColumns();
  void remove(unsigned selection);
  void reset();
  void setCheckable(bool checkable);
  void setChecked(unsigned selection, bool checked);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned selection, unsigned position, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned selection);
  void setText(unsigned selection, unsigned position, string text);

  pListView(ListView& listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
  void onChange(QTreeWidgetItem* item);
  void onToggle(QTreeWidgetItem* item);
};

struct pProgressBar : public pWidget {
  ProgressBar& progressBar;
  QProgressBar* qtProgressBar;

  Size minimumSize();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar& progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pRadioLabel : public QObject, public pWidget {
  Q_OBJECT

public:
  RadioLabel& radioLabel;
  QRadioButton* qtRadioLabel;

  bool checked();
  Size minimumSize();
  void setChecked();
  void setGroup(const group<RadioLabel>& group);
  void setText(string text);

  pRadioLabel(RadioLabel& radioLabel) : pWidget(radioLabel), radioLabel(radioLabel) {}
  pRadioLabel& parent();
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
};

struct pRadioButton : public QObject, public pWidget {
  Q_OBJECT

public:
  RadioButton& radioButton;
  QToolButton* qtRadioButton;

  Size minimumSize();
  void setChecked();
  void setGroup(const group<RadioButton>& group);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pRadioButton(RadioButton& radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  pRadioButton& parent();
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
};

struct pTabFrame : public QObject, public pWidget {
  Q_OBJECT

public:
  TabFrame& tabFrame;
  QTabWidget* qtTabFrame;

  void append(string text, const image& image);
  QWidget* container(Widget& widget);
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
  void synchronizeLayout();

public slots:
  void onChange(int selection);
};

struct pTextEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  TextEdit& textEdit;
  QTextEdit* qtTextEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(string text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit& textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pVerticalScroller : public QObject, public pWidget {
  Q_OBJECT

public:
  VerticalScroller& verticalScroller;
  QScrollBar* qtScroller;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller& verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pVerticalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  VerticalSlider& verticalSlider;
  QSlider* qtSlider;

  Size minimumSize();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider& verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pViewport : public pWidget {
  Viewport& viewport;
  struct QtViewport : public QWidget {
    pViewport& self;
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void leaveEvent(QEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    QtViewport(pViewport& self);
  };
  QtViewport* qtViewport;

  uintptr_t handle();
  void setDroppable(bool droppable);

  pViewport(Viewport& viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
  void orphan();
};

}
