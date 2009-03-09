class AboutWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
    QVBoxLayout *layout;
      struct Logo : public QWidget {
        void paintEvent(QPaintEvent*);
      } *logo;
      QLabel *info;
      QWidget *spacer;

  void setup();
  void show();

public slots:
} *winAbout;
