void EditBox::setParent(Layout &parent) {
  editBox->setParent(parent.widget->widget);
  editBox->show();
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
  editBox->connect(editBox, SIGNAL(textChanged()), SLOT(onChange()));
}
