Geometry pLineEdit::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), lineEdit.state.text);
  return { 0, 0, geometry.width + 12, geometry.height + 12 };
}

void pLineEdit::setEditable(bool editable) {
  qtLineEdit->setReadOnly(!editable);
}

void pLineEdit::setText(const string &text) {
  qtLineEdit->setText(QString::fromUtf8(text));
}

string pLineEdit::text() {
  return qtLineEdit->text().toUtf8().constData();
}

void pLineEdit::constructor() {
  qtWidget = qtLineEdit = new QLineEdit;
  connect(qtLineEdit, SIGNAL(returnPressed()), SLOT(onActivate()));
  connect(qtLineEdit, SIGNAL(textEdited(const QString&)), SLOT(onChange()));
  setEditable(lineEdit.state.editable);
  setText(lineEdit.state.text);
}

void pLineEdit::destructor() {
  delete qtLineEdit;
  qtWidget = qtLineEdit = 0;
}

void pLineEdit::orphan() {
  destructor();
  constructor();
}

void pLineEdit::onActivate() {
  if(lineEdit.onActivate) lineEdit.onActivate();
}

void pLineEdit::onChange() {
  lineEdit.state.text = text();
  if(lineEdit.onChange) lineEdit.onChange();
}
