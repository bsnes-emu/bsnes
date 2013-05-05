namespace phoenix {

struct pLineEdit : public pWidget {
  LineEdit& lineEdit;

  void setEditable(bool editable);
  void setText(string text);
  string text();

  pLineEdit(LineEdit& lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
};

}
