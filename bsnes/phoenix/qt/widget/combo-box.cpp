void pComboBox::append(const string &text) {
  locked = true;
  qtComboBox->addItem(QString::fromUtf8(text));
  locked = false;
}

Geometry pComboBox::minimumGeometry() {
  unsigned maximumWidth = 0;
  foreach(text, comboBox.state.text) maximumWidth = max(maximumWidth, pFont::geometry(qtWidget->font(), text).width);
  Geometry geometry = pFont::geometry(qtWidget->font(), " ");
  return { 0, 0, maximumWidth + 32, geometry.height + 12 };
}

void pComboBox::reset() {
  locked = true;
  while(qtComboBox->count()) qtComboBox->removeItem(0);
  locked = false;
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

  locked = true;
  foreach(text, comboBox.state.text) append(text);
  locked = false;
  setSelection(comboBox.state.selection);
}

void pComboBox::destructor() {
  delete qtComboBox;
  qtWidget = qtComboBox = 0;
}

void pComboBox::orphan() {
  destructor();
  constructor();
}

void pComboBox::onChange() {
  comboBox.state.selection = selection();
  if(locked == false && comboBox.onChange) comboBox.onChange();
}
