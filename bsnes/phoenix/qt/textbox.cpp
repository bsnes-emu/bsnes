void TextBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  textBox->setParent(parent.window->container);
  textBox->setGeometry(x, y, width, height);
  textBox->setText(QString::fromUtf8(text));
  if(parent.window->defaultFont) textBox->setFont(*parent.window->defaultFont);
  textBox->show();
  textBox->connect(textBox, SIGNAL(returnPressed()), SLOT(onActivate()));
  textBox->connect(textBox, SIGNAL(textEdited(const QString&)), SLOT(onChange()));
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
}
