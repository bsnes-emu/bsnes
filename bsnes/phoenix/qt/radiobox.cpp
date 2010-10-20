void RadioBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  radioBox->parent = &parent;
  radioBox->buttonGroup = new QButtonGroup;
  radioBox->buttonGroup->addButton(radioBox);
  radioBox->setParent(radioBox->parent->window->container);
  radioBox->setGeometry(x, y, width, height);
  radioBox->setText(text);
  radioBox->setChecked(true);
  if(parent.window->defaultFont) radioBox->setFont(*parent.window->defaultFont);
  radioBox->show();
  radioBox->connect(radioBox, SIGNAL(toggled(bool)), SLOT(onTick()));
}

void RadioBox::create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  radioBox->parent = parent.radioBox->parent;
  radioBox->buttonGroup = parent.radioBox->buttonGroup;
  radioBox->buttonGroup->addButton(radioBox);
  radioBox->setParent(radioBox->parent->window->container);
  radioBox->setGeometry(x, y, width, height);
  radioBox->setText(text);
  if(radioBox->parent->window->defaultFont) radioBox->setFont(*radioBox->parent->window->defaultFont);
  radioBox->show();
  radioBox->connect(radioBox, SIGNAL(toggled(bool)), SLOT(onTick()));
}

bool RadioBox::checked() {
  return radioBox->isChecked();
}

void RadioBox::setChecked() {
  radioBox->setChecked(true);
}

RadioBox::RadioBox() {
  radioBox = new RadioBox::Data(*this);
  widget->widget = radioBox;
}
