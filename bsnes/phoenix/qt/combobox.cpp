void ComboBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  comboBox->setParent(parent.window->container);
  comboBox->setGeometry(x, y, width, height);

  if(*text) {
    lstring list;
    list.split("\n", text);
    foreach(item, list) addItem((const char*)item);
  }

  comboBox->connect(comboBox, SIGNAL(currentIndexChanged(int)), SLOT(onChange()));
  if(parent.window->defaultFont) comboBox->setFont(*parent.window->defaultFont);
  comboBox->show();
}

void ComboBox::reset() {
  while(comboBox->count()) comboBox->removeItem(0);
}

void ComboBox::addItem(const char *text) {
  comboBox->addItem(text);
}

unsigned ComboBox::selection() {
  signed index = comboBox->currentIndex();
  return (index >= 0 ? index : 0);
}

void ComboBox::setSelection(unsigned row) {
  comboBox->setCurrentIndex(row);
}

ComboBox::ComboBox() {
  comboBox = new ComboBox::Data(*this);
  widget->widget = comboBox;
}
