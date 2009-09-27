class QbWindow : public QWidget {
  Q_OBJECT

public:
  void showHidden(bool);
  void showAt(double x, double y);
  void setFocus();
};
