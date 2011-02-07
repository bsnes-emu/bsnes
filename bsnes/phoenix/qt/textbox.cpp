void TextBox::setParent(Layout &parent) {
  textBox->setParent(parent.widget->widget);
  textBox->show();
}

void TextBox::setEditable(bool editable) {
  textBox->setReadOnly(editable == false);
}

string TextBox::text() {
  return textBox->text().toUtf8().constData();
}

void TextBox::setText(const string &text) {
  textBox->setText(QString::fromUtf8(text));
}

TextBox::TextBox() {
  textBox = new TextBox::Data(*this);
  widget->widget = textBox;
  textBox->connect(textBox, SIGNAL(returnPressed()), SLOT(onActivate()));
  textBox->connect(textBox, SIGNAL(textEdited(const QString&)), SLOT(onChange()));
}
