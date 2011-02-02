struct Object::Data {
public:
  Object &self;
  bool locked;

  Data(Object &self) : self(self) {
    locked = false;
  }
};

struct Font::Data : public QFont {
public:
  Font &self;

  Data(Font &self) : self(self) {
  }
};

struct Menu::Data : public QMenu {
public:
  Menu &self;
  Window *parent;

  Data(Menu &self) : self(self), parent(0) {
  }
};

struct MenuSeparator::Data {
public:
  MenuSeparator &self;
  QAction *action;

  Data(MenuSeparator &self) : self(self) {
  }
};

struct MenuItem::Data : public QAction {
  Q_OBJECT

public:
  MenuItem &self;

  Data(MenuItem &self) : self(self), QAction(0) {
  }

public slots:
  void onTick() {
    if(self.onTick) self.onTick();
  }
};

struct MenuCheckItem::Data : public QAction {
  Q_OBJECT

public:
  MenuCheckItem &self;

  Data(MenuCheckItem &self) : self(self), QAction(0) {
  }

public slots:
  void onTick() {
    if(self.onTick) self.onTick();
  }
};

struct MenuRadioItem::Data : public QAction {
  Q_OBJECT

public:
  MenuRadioItem &self;
  Menu *parent;
  QActionGroup *actionGroup;

  Data(MenuRadioItem &self) : self(self), QAction(0) {
  }

public slots:
  void onTick() {
    if(self.object->locked == false && self.onTick && self.checked()) self.onTick();
  }
};

struct Widget::Data {
public:
  Widget &self;
  QWidget *widget;

  Data(Widget &self) : self(self) {
  }
};

struct Window::Data : public QWidget {
  Q_OBJECT

public:
  Window &self;
  QFont *defaultFont;
  QVBoxLayout *layout;
  QMenuBar *menuBar;
  QWidget *container;
  QStatusBar *statusBar;

  void closeEvent(QCloseEvent *event) {
    if(self.onClose) {
      bool result = self.onClose();
      if(result == false) event->ignore();
    }
  }

  Data(Window &self) : self(self) {
  }
};

struct Button::Data : public QPushButton {
  Q_OBJECT

public:
  Button &self;

  Data(Button &self) : self(self) {
  }

public slots:
  void onTick() {
    if(self.onTick) self.onTick();
  }
};

struct Canvas::Data : public QWidget {
  Q_OBJECT

public:
  Canvas &self;
  QImage *image;
  void paintEvent(QPaintEvent*);

  Data(Canvas &self) : self(self) {
  }
};

struct CheckBox::Data : public QCheckBox {
  Q_OBJECT

public:
  CheckBox &self;

  Data(CheckBox &self) : self(self) {
  }

public slots:
  void onTick() {
    if(self.onTick) self.onTick();
  }
};

struct ComboBox::Data : public QComboBox {
  Q_OBJECT

public:
  ComboBox &self;

  Data(ComboBox &self) : self(self) {
  }

public slots:
  void onChange() {
    if(self.object->locked == false && self.onChange) self.onChange();
  }
};

struct EditBox::Data : public QTextEdit {
  Q_OBJECT

public:
  EditBox &self;

  Data(EditBox &self) : self(self) {
  }

public slots:
  void onChange() {
    if(self.onChange) self.onChange();
  }
};

struct HexEditor::Data : public QTextEdit {
  Q_OBJECT

public:
  HexEditor &self;
  QHBoxLayout *layout;
  QScrollBar *scrollBar;
  unsigned size;
  unsigned offset;
  unsigned columns;
  unsigned rows;

  void keyPressEvent(QKeyEvent*);

  Data(HexEditor &self) : self(self) {
  }

public slots:
  void scrollEvent();
};

struct HorizontalSlider::Data : public QSlider {
  Q_OBJECT

public:
  HorizontalSlider &self;

  Data(HorizontalSlider &self) : self(self), QSlider(Qt::Horizontal) {
  }

public slots:
  void onChange() {
    if(self.onChange) self.onChange();
  }
};

struct Label::Data : public QLabel {
  Q_OBJECT

public:
  Label &self;

  Data(Label &self) : self(self) {
  }
};

struct ListBox::Data : public QTreeWidget {
  Q_OBJECT

public:
  ListBox &self;
  bool checkable;

  Data(ListBox &self) : self(self) {
    checkable = false;
  }

public slots:
  void onActivate() {
    if(self.object->locked == false && self.onActivate) self.onActivate();
  }

  void onChange() {
    if(self.object->locked == false && self.onChange) self.onChange();
  }

  void onTick(QTreeWidgetItem *item) {
    if(self.object->locked == false && self.onTick) self.onTick(item->data(0, Qt::UserRole).toUInt());
  }
};

struct ProgressBar::Data : public QProgressBar {
public:
  ProgressBar &self;

  Data(ProgressBar &self) : self(self) {
  }
};

struct RadioBox::Data : public QRadioButton {
  Q_OBJECT

public:
  RadioBox &self;
  Window *parent;
  QButtonGroup *buttonGroup;

  Data(RadioBox &self) : self(self) {
  }

public slots:
  void onTick() {
    if(self.onTick && self.checked()) self.onTick();
  }
};

struct TextBox::Data : public QLineEdit {
  Q_OBJECT

public:
  TextBox &self;

  Data(TextBox &self) : self(self) {
  }

public slots:
  void onActivate() {
    if(self.onActivate) self.onActivate();
  }

  void onChange() {
    if(self.onChange) self.onChange();
  }
};

struct VerticalSlider::Data : public QSlider {
  Q_OBJECT

public:
  VerticalSlider &self;

  Data(VerticalSlider &self) : self(self), QSlider(Qt::Vertical) {
  }

public slots:
  void onChange() {
    if(self.onChange) self.onChange();
  }
};

struct Viewport::Data : public QWidget {
public:
  Viewport &self;

  Data(Viewport &self) : self(self) {
  }
};

struct OS::Data : public QObject {
  Q_OBJECT

public:
  QApplication *application;

public slots:
};
