class AboutWindow : public QObject {
  Q_OBJECT

public:
  QbWindow *window;
  QVBoxLayout *layout;
    struct Logo : public QWidget {
      void paintEvent(QPaintEvent*);
    } *logo;
    QLabel *info;

  AboutWindow();

public slots:
} *aboutWindow;
