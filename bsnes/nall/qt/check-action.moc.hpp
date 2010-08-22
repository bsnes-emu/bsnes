#ifndef NALL_QT_CHECKACTION_HPP
#define NALL_QT_CHECKACTION_HPP

namespace nall {

class CheckAction : public QAction {
  Q_OBJECT

public:
  bool isChecked() const;
  void setChecked(bool);
  void toggleChecked();
  CheckAction(const QString&, QObject*);

protected slots:

protected:
  bool checked;
};

inline bool CheckAction::isChecked() const {
  return checked;
}

inline void CheckAction::setChecked(bool checked_) {
  checked = checked_;
  if(checked) setIcon(QIcon(":/16x16/item-check-on.png"));
  else setIcon(QIcon(":/16x16/item-check-off.png"));
}

inline void CheckAction::toggleChecked() {
  setChecked(!isChecked());
}

inline CheckAction::CheckAction(const QString &text, QObject *parent) : QAction(text, parent) {
  setChecked(false);
}

}

#endif
