namespace phoenix {

void pComboButton::append(string text) {
  locked = true;
  qtComboButton->addItem(QString::fromUtf8(text));
  locked = false;
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto& text : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(qtWidget->font(), text).width);
  Size size = pFont::size(qtWidget->font(), " ");
  return {maximumWidth + 32, size.height + 12};
}

void pComboButton::remove(unsigned selection) {
  locked = true;
  qtComboButton->removeItem(selection);
  locked = false;

  if(selection == comboButton.state.selection) comboButton.setSelection(0);
}

void pComboButton::reset() {
  locked = true;
  while(qtComboButton->count()) qtComboButton->removeItem(0);
  locked = false;
}

void pComboButton::setSelection(unsigned selection) {
  locked = true;
  qtComboButton->setCurrentIndex(selection);
  locked = false;
}

void pComboButton::setText(unsigned selection, string text) {
  qtComboButton->setItemText(selection, text);
}

void pComboButton::constructor() {
  qtWidget = qtComboButton = new QComboBox;
  connect(qtComboButton, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));

  pWidget::synchronizeState();
  unsigned selection = comboButton.state.selection;
  locked = true;
  for(auto& text : comboButton.state.text) append(text);
  locked = false;
  comboButton.setSelection(selection);
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
  if(!locked && comboButton.onChange) comboButton.onChange();
}

}
