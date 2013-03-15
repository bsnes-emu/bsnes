namespace phoenix {

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
};

}
