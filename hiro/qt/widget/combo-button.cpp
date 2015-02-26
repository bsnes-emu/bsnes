namespace phoenix {

void pComboButton::append() {
  lock();
  qtComboButton->addItem("");
  unlock();
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto& text : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(qtWidget->font(), text).width);
  Size size = pFont::size(qtWidget->font(), " ");
  return {maximumWidth + 32, size.height + 12};
}

void pComboButton::remove(unsigned selection) {
  lock();
  qtComboButton->removeItem(selection);
  if(selection == comboButton.state.selection) comboButton[0].setSelected();
  unlock();
}

void pComboButton::reset() {
  lock();
  while(qtComboButton->count()) qtComboButton->removeItem(0);
  unlock();
}

void pComboButton::setSelected(unsigned selection) {
  lock();
  qtComboButton->setCurrentIndex(selection);
  unlock();
}

void pComboButton::setText(unsigned selection, string text) {
  qtComboButton->setItemText(selection, text);
}

void pComboButton::constructor() {
  qtWidget = qtComboButton = new QComboBox;
  connect(qtComboButton, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));

  pWidget::synchronizeState();
  unsigned selection = comboButton.state.selection;
  for(unsigned n = 0; n < comboButton.count(); n++) {
    append();
    setText(n, comboButton.state.text[n]);
  }
  comboButton[selection].setSelected();
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
  comboButton.state.selection = qtComboButton->currentIndex();
  if(!locked() && comboButton.onChange) comboButton.onChange();
}

}
