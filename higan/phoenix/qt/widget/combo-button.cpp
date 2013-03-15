namespace phoenix {

void pComboButton::append(const string &text) {
  locked = true;
  qtComboButton->addItem(QString::fromUtf8(text));
  locked = false;
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto &text : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(qtWidget->font(), text).width);
  Size size = pFont::size(qtWidget->font(), " ");
  return {maximumWidth + 32, size.height + 12};
}

void pComboButton::modify(unsigned row, const string &text) {
  qtComboButton->setItemText(row, text);
}

void pComboButton::remove(unsigned row) {
  locked = true;
  unsigned position = selection();
  qtComboButton->removeItem(row);
  if(position == row) qtComboButton->setCurrentIndex(0);
  locked = false;
}

void pComboButton::reset() {
  locked = true;
  while(qtComboButton->count()) qtComboButton->removeItem(0);
  locked = false;
}

unsigned pComboButton::selection() {
  signed index = qtComboButton->currentIndex();
  return index >= 0 ? index : 0;
}

void pComboButton::setSelection(unsigned row) {
  locked = true;
  qtComboButton->setCurrentIndex(row);
  locked = false;
}

void pComboButton::constructor() {
  qtWidget = qtComboButton = new QComboBox;
  connect(qtComboButton, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));

  pWidget::synchronizeState();
  unsigned selection = comboButton.state.selection;
  locked = true;
  for(auto &text : comboButton.state.text) append(text);
  locked = false;
  setSelection(selection);
}

void pComboButton::destructor() {
  delete qtComboButton;
  qtWidget = qtComboButton = nullptr;
}

void pComboButton::orphan() {
  destructor();
  constructor();
}

void pComboButton::onChange() {
  comboButton.state.selection = selection();
  if(locked == false && comboButton.onChange) comboButton.onChange();
}

}
