void pComboBox::append(const string &text) {
  qtComboBox->addItem(QString::fromUtf8(text));
}

void pComboBox::reset() {
  while(qtComboBox->count()) qtComboBox->removeItem(0);
}

unsigned pComboBox::selection() {
  signed index = qtComboBox->currentIndex();
  return index >= 0 ? index : 0;
}

void pComboBox::setSelection(unsigned row) {
  locked = true;
  qtComboBox->setCurrentIndex(row);
  locked = false;
}

void pComboBox::constructor() {
  qtWidget = qtComboBox = new QComboBox;
  connect(qtComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));
}

void pComboBox::onChange() {
  comboBox.state.selection = selection();
  if(locked == false && comboBox.onChange) comboBox.onChange();
}
