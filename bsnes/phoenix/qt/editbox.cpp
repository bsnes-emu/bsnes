void EditBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  editBox->setParent(parent.window->container);
  editBox->setGeometry(x, y, width, height);
  editBox->setText(text);
  if(parent.window->defaultFont) editBox->setFont(*parent.window->defaultFont);
  editBox->show();
  editBox->connect(editBox, SIGNAL(textChanged()), SLOT(onChange()));
}

void EditBox::setEditable(bool editable) {
}

void EditBox::setWordWrap(bool wordWrap) {
  editBox->setWordWrapMode(wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);
}

string EditBox::text() {
}

void EditBox::setText(const char *text) {
}

EditBox::EditBox() {
  editBox = new EditBox::Data(*this);
  widget->widget = editBox;
}
