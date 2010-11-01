void EditBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  editBox->setParent(parent.window->container);
  editBox->setGeometry(x, y, width, height);
  editBox->setPlainText(QString::fromUtf8(text));
  if(parent.window->defaultFont) editBox->setFont(*parent.window->defaultFont);
  editBox->show();
  editBox->connect(editBox, SIGNAL(textChanged()), SLOT(onChange()));
}

void EditBox::setEditable(bool editable) {
  editBox->setReadOnly(editable == false);
}

void EditBox::setWordWrap(bool wordWrap) {
  editBox->setWordWrapMode(wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);
}

string EditBox::text() {
  return editBox->toPlainText().toUtf8().constData();
}

void EditBox::setText(const string &text) {
  editBox->setPlainText(QString::fromUtf8(text));
}

void EditBox::setCursorPosition(unsigned position) {
  QTextCursor cursor = editBox->textCursor();
  unsigned lastchar = strlen(editBox->toPlainText().toUtf8().constData());
  cursor.setPosition(min(position, lastchar));
  editBox->setTextCursor(cursor);
}

EditBox::EditBox() {
  editBox = new EditBox::Data(*this);
  widget->widget = editBox;
}
