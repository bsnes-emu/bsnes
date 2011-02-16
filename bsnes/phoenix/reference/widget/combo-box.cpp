void pComboBox::append(const string &text) {
}

void pComboBox::reset() {
}

unsigned pComboBox::selection() {
  return 0;
}

void pComboBox::setSelection(unsigned row) {
}

pComboBox::pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {
}
