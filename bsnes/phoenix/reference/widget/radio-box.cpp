bool pRadioBox::checked() {
  return false;
}

void pRadioBox::setChecked() {
}

void pRadioBox::setGroup(const array<RadioBox*> &group) {
}

void pRadioBox::setText(const string &text) {
}

pRadioBox::pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {
}
