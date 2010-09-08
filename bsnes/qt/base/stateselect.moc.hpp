class StateSelectWindow : public Window {
  Q_OBJECT

public:
  QGridLayout *layout;
  QPushButton *slot[10];

  void setSlot(unsigned slot);
  void keyReleaseEvent(QKeyEvent*);
  StateSelectWindow();

public slots:
  void slotClicked();
};

extern StateSelectWindow *stateSelectWindow;
