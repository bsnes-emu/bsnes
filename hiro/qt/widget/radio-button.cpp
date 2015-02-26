namespace phoenix {

Size pRadioButton::minimumSize() {
  Size size = pFont::size(qtWidget->font(), radioButton.state.text);

  if(radioButton.state.orientation == Orientation::Horizontal) {
    size.width += radioButton.state.image.width;
    size.height = max(radioButton.state.image.height, size.height);
  }

  if(radioButton.state.orientation == Orientation::Vertical) {
    size.width = max(radioButton.state.image.width, size.width);
    size.height += radioButton.state.image.height;
  }

  return {size.width + 20, size.height + 12};
}

void pRadioButton::setChecked() {
  parent().lock();
  for(auto& item : radioButton.state.group) {
    bool checked = &item.p == this;
    item.p.qtRadioButton->setChecked(item.state.checked = checked);
  }
  parent().unlock();
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
  parent().lock();
  for(auto& item : radioButton.state.group) {
    item.p.qtRadioButton->setChecked(item.state.checked);
  }
  parent().unlock();
}

void pRadioButton::setImage(const image& image, Orientation orientation) {
  qtRadioButton->setIconSize(QSize(image.width, image.height));
  qtRadioButton->setIcon(CreateIcon(image));
  qtRadioButton->setStyleSheet("text-align: top;");
  switch(orientation) {
  case Orientation::Horizontal: qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); break;
  case Orientation::Vertical:   qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  break;
  }
}

void pRadioButton::setText(string text) {
  qtRadioButton->setText(QString::fromUtf8(text));
}

pRadioButton& pRadioButton::parent() {
  if(radioButton.state.group.size()) return radioButton.state.group.first().p;
  return *this;
}

void pRadioButton::constructor() {
  qtWidget = qtRadioButton = new QToolButton;
  qtRadioButton->setCheckable(true);
  qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  connect(qtRadioButton, SIGNAL(toggled(bool)), SLOT(onActivate()));

  pWidget::synchronizeState();
  setGroup(radioButton.state.group);
  setText(radioButton.state.text);
}

void pRadioButton::destructor() {
  if(qtRadioButton) delete qtRadioButton;
  qtWidget = qtRadioButton = nullptr;
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

void pRadioButton::onActivate() {
  if(parent().locked()) return;
  bool wasChecked = radioButton.state.checked;
  setChecked();
  if(!wasChecked) {
    if(radioButton.onActivate) radioButton.onActivate();
  }
}

}
