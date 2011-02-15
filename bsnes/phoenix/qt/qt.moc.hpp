struct Settings : public configuration {
  unsigned frameGeometryX;
  unsigned frameGeometryY;
  unsigned frameGeometryWidth;
  unsigned frameGeometryHeight;

  void load();
  void save();
  Settings();
};

struct pFont;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pObject {
  bool locked;

  pObject() {
    locked = false;
  }
};

struct pOS : public pObject {
  QApplication *application;

  unsigned desktopWidth();
  unsigned desktopHeight();
  string fileLoad(Window &parent, const string &path, const lstring &filter);
  string fileSave(Window &parent, const string &path, const lstring &filter);
  string folderSelect(Window &parent, const string &path);
  void main();
  bool pending();
  void process();
  void quit();
  void setDefaultFont(Font &font);

  pOS();
};

struct pFont : public pObject {
  Font &font;
  QFont *qtFont;

  void setBold(bool bold);
  void setFamily(const string &family);
  void setItalic(bool italic);
  void setSize(unsigned size);
  void setUnderline(bool underline);

  pFont(Font &font);
  void update();
};

struct pMessageWindow : public pObject {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pWindow : public QObject, public pObject {
  Q_OBJECT

public:
  Window &window;
  struct QtWindow : public QWidget {
    pWindow &self;
    void closeEvent(QCloseEvent*);
    void moveEvent(QMoveEvent*);
    void resizeEvent(QResizeEvent*);
    QSize sizeHint() const;
    QtWindow(pWindow &self) : self(self) {}
  } *qtWindow;
  QVBoxLayout *qtLayout;
  QMenuBar *qtMenu;
  QStatusBar *qtStatus;
  QWidget *qtContainer;
  Layout *layout;

  void append(Menu &menu);
  Geometry frameGeometry();
  bool focused();
  Geometry geometry();
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setFrameGeometry(const Geometry &geometry);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setLayout(Layout &layout);
  void setMenuFont(Font &font);
  void setMenuVisible(bool visible);
  void setResizable(bool resizable);
  void setStatusFont(Font &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);

  pWindow(Window &window);
  void updateFrameGeometry();
};

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action);
};

struct pMenu : public pAction {
  Menu &menu;
  QMenu *qtMenu;

  void append(Action &action);
  void setText(const string &text);

  pMenu(Menu &menu);
};

struct pMenuSeparator : public pAction {
  MenuSeparator &menuSeparator;
  QAction *qtAction;

  pMenuSeparator(MenuSeparator &menuSeparator);
};

struct pMenuItem : public QObject, public pAction {
  Q_OBJECT

public:
  MenuItem &menuItem;
  QAction *qtAction;

  void setText(const string &text);

  pMenuItem(MenuItem &menuItem);

public slots:
  void onTick();
};

struct pMenuCheckItem : public QObject, public pAction {
  Q_OBJECT

public:
  MenuCheckItem &menuCheckItem;
  QAction *qtAction;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pMenuCheckItem(MenuCheckItem &menuCheckItem);

public slots:
  void onTick();
};

struct pMenuRadioItem : public QObject, public pAction {
  Q_OBJECT

public:
  MenuRadioItem &menuRadioItem;
  QAction *qtAction;
  QActionGroup *qtGroup;

  bool checked();
  void setChecked();
  void setGroup(const array<MenuRadioItem*> &group);
  void setText(const string &text);

  pMenuRadioItem(MenuRadioItem &menuRadioitem);

public slots:
  void onTick();
};

struct pLayout : public pObject {
  Layout &layout;
  pWindow *parent;

  void append(Widget &widget);

  pLayout(Layout &layout);
};

struct pWidget : public pObject {
  Widget &widget;
  QWidget *qtWidget;

  bool enabled();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(Font &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget);
};

struct pButton : public QObject, public pWidget {
  Q_OBJECT

public:
  Button &button;
  QPushButton *qtButton;

  void setText(const string &text);

  pButton(Button &button);

public slots:
  void onTick();
};

struct pCheckBox : public QObject, public pWidget {
  Q_OBJECT

public:
  CheckBox &checkBox;
  QCheckBox *qtCheckBox;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox);

public slots:
  void onTick();
};

struct pComboBox : public QObject, public pWidget {
  Q_OBJECT

public:
  ComboBox &comboBox;
  QComboBox *qtComboBox;

  void append(const string &text);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox);

public slots:
  void onChange();
};

struct pHexEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  HexEdit &hexEdit;
  struct QtHexEdit : public QTextEdit {
    pHexEdit &self;
    void keyPressEvent(QKeyEvent*);
    void keyPressEventAcknowledge(QKeyEvent*);
    QtHexEdit(pHexEdit &self) : self(self) {}
  } *qtHexEdit;
  QHBoxLayout *qtLayout;
  QScrollBar *qtScroll;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  void keyPressEvent(QKeyEvent*);
  pHexEdit(HexEdit &hexEdit);

public slots:
  void onScroll();
};

struct pHorizontalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  HorizontalSlider &horizontalSlider;
  QSlider *qtSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider);

public slots:
  void onChange();
};

struct pLabel : public pWidget {
  Label &label;
  QLabel *qtLabel;

  void setText(const string &text);

  pLabel(Label &label);
};

struct pLineEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  LineEdit &lineEdit;
  QLineEdit *qtLineEdit;

  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit);

public slots:
  void onActivate();
  void onChange();
};

struct pListBox : public QObject, public pWidget {
  Q_OBJECT

public:
  ListBox &listBox;
  QTreeWidget *qtListBox;

  void append(const lstring &text);
  void autosizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring &text);
  void modify(unsigned row, unsigned column, const string &text);
  void reset();
  optional<unsigned> selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setSelection(unsigned row);

  pListBox(ListBox &listBox);

public slots:
  void onActivate();
  void onChange();
  void onTick(QTreeWidgetItem *item);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;
  QProgressBar *qtProgressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar);
};

struct pRadioBox : public QObject, public pWidget {
  Q_OBJECT

public:
  RadioBox &radioBox;
  QRadioButton *qtRadioBox;
  QButtonGroup *qtGroup;

  bool checked();
  void setChecked();
  void setGroup(const array<RadioBox*> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox);

public slots:
  void onTick();
};

struct pTextEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  TextEdit &textEdit;
  QTextEdit *qtTextEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit);

public slots:
  void onChange();
};

struct pVerticalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  VerticalSlider &verticalSlider;
  QSlider *qtSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider);

public slots:
  void onChange();
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport);
};
