void ComboBox::reset() {
  while(comboBox->count()) comboBox->removeItem(0);
}

void ComboBox::addItem(const string &text) {
  comboBox->addItem(QString::fromUtf8(text));
}

unsigned ComboBox::selection() {
  signed index = comboBox->currentIndex();
  return (index >= 0 ? index : 0);
}

void ComboBox::setSelection(unsigned row) {
  object->locked = true;
  comboBox->setCurrentIndex(row);
  object->locked = false;
}

ComboBox::ComboBox() {
  comboBox = new ComboBox::Data(*this);
  widget->widget = comboBox;
  comboBox->connect(comboBox, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));
}
