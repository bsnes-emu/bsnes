void pLineEdit::setEditable(bool editable) {
  qtLineEdit->setReadOnly(!editable);
}

void pLineEdit::setText(const string &text) {
  qtLineEdit->setText(QString::fromUtf8(text));
}

string pLineEdit::text() {
  return qtLineEdit->text().toUtf8().constData();
}

pLineEdit::pLineEdit(LineEdit &lineEdit) : lineEdit(lineEdit), pWidget(lineEdit) {
  qtWidget = qtLineEdit = new QLineEdit;
  connect(qtLineEdit, SIGNAL(returnPressed()), SLOT(onActivate()));
  connect(qtLineEdit, SIGNAL(textEdited(const QString&)), SLOT(onChange()));
}

void pLineEdit::onActivate() {
  if(lineEdit.onActivate) lineEdit.onActivate();
}

void pLineEdit::onChange() {
  lineEdit.state.text = text();
  if(lineEdit.onChange) lineEdit.onChange();
}
