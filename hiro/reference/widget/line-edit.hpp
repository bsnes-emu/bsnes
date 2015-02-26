namespace phoenix {

struct pLineEdit : public pWidget {
  LineEdit& lineEdit;

  void setBackgroundColor(Color color);
  void setEditable(bool editable);
  void setForegroundColor(Color color);
  void setText(string text);
  string text();

  pLineEdit(LineEdit& lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
};

}
