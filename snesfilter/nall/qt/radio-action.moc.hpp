#ifndef NALL_QT_RADIOACTION_HPP
#define NALL_QT_RADIOACTION_HPP

namespace nall {

class RadioAction : public QAction {
  Q_OBJECT

public:
  bool isChecked() const;
  void setChecked(bool);
  void toggleChecked();
  RadioAction(const QString&, QObject*);

protected slots:

protected:
  bool checked;
};

inline bool RadioAction::isChecked() const {
  return checked;
}

inline void RadioAction::setChecked(bool checked_) {
  checked = checked_;
  if(checked) setIcon(QIcon(":/16x16/item-radio-on.png"));
  else setIcon(QIcon(":/16x16/item-radio-off.png"));
}

inline void RadioAction::toggleChecked() {
  setChecked(!isChecked());
}

inline RadioAction::RadioAction(const QString &text, QObject *parent) : QAction(text, parent) {
  setChecked(false);
}

}

#endif
