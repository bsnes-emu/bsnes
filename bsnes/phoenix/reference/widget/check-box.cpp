bool pCheckBox::checked() {
  return false;
}

void pCheckBox::setChecked(bool checked) {
}

void pCheckBox::setText(const string &text) {
}

pCheckBox::pCheckBox(CheckBox &checkBox) : pWidget(checkBox), checkBox(checkBox) {
}
