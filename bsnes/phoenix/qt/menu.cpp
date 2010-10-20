void Menu::create(Window &parent, const char *text) {
  menu->setTitle(text);
  parent.window->menuBar->addMenu(menu);
}

void Menu::create(Menu &parent, const char *text) {
  menu->setTitle(text);
  parent.menu->addMenu(menu);
}

bool Menu::visible() {
  return menu->isVisible();
}

void Menu::setVisible(bool visible) {
  menu->setVisible(visible);
}

bool Menu::enabled() {
  return menu->isEnabled();
}

void Menu::setEnabled(bool enabled) {
  menu->setEnabled(enabled);
}

Menu::Menu() {
  menu = new Menu::Data(*this);
}

void MenuSeparator::create(Menu &parent) {
  menuSeparator->action = parent.menu->addSeparator();
}

bool MenuSeparator::visible() {
  return menuSeparator->action->isVisible();
}

void MenuSeparator::setVisible(bool visible) {
  menuSeparator->action->setVisible(visible);
}

bool MenuSeparator::enabled() {
  return menuSeparator->action->isEnabled();
}

void MenuSeparator::setEnabled(bool enabled) {
  menuSeparator->action->setEnabled(enabled);
}

MenuSeparator::MenuSeparator() {
  menuSeparator = new MenuSeparator::Data(*this);
}

void MenuItem::create(Menu &parent, const char *text) {
  menuItem->setText(text);
  menuItem->connect(menuItem, SIGNAL(triggered()), SLOT(onTick()));
  parent.menu->addAction(menuItem);
}

bool MenuItem::visible() {
  return menuItem->isVisible();
}

void MenuItem::setVisible(bool visible) {
  menuItem->setVisible(visible);
}

bool MenuItem::enabled() {
  return menuItem->isEnabled();
}

void MenuItem::setEnabled(bool enabled) {
  menuItem->setEnabled(enabled);
}

MenuItem::MenuItem() {
  menuItem = new MenuItem::Data(*this);
}

void MenuCheckItem::create(Menu &parent, const char *text) {
  menuCheckItem->setText(text);
  menuCheckItem->setCheckable(true);
  menuCheckItem->connect(menuCheckItem, SIGNAL(triggered()), SLOT(onTick()));
  parent.menu->addAction(menuCheckItem);
}

bool MenuCheckItem::visible() {
  return menuCheckItem->isVisible();
}

void MenuCheckItem::setVisible(bool visible) {
  menuCheckItem->setVisible(visible);
}

bool MenuCheckItem::enabled() {
  return menuCheckItem->isEnabled();
}

void MenuCheckItem::setEnabled(bool enabled) {
  menuCheckItem->setEnabled(enabled);
}

bool MenuCheckItem::checked() {
  return menuCheckItem->isChecked();
}

void MenuCheckItem::setChecked(bool checked) {
  menuCheckItem->setChecked(checked);
}

MenuCheckItem::MenuCheckItem() {
  menuCheckItem = new MenuCheckItem::Data(*this);
}

void MenuRadioItem::create(Menu &parent, const char *text) {
  menuRadioItem->parent = &parent;
  menuRadioItem->actionGroup = new QActionGroup(0);
  menuRadioItem->actionGroup->addAction(menuRadioItem);
  menuRadioItem->setText(text);
  menuRadioItem->setCheckable(true);
  menuRadioItem->setChecked(true);
  menuRadioItem->connect(menuRadioItem, SIGNAL(changed()), SLOT(onTick()));
  menuRadioItem->parent->menu->addAction(menuRadioItem);
}

void MenuRadioItem::create(MenuRadioItem &parent, const char *text) {
  menuRadioItem->parent = parent.menuRadioItem->parent;
  menuRadioItem->actionGroup = parent.menuRadioItem->actionGroup;
  menuRadioItem->actionGroup->addAction(menuRadioItem);
  menuRadioItem->setText(text);
  menuRadioItem->setCheckable(true);
  menuRadioItem->connect(menuRadioItem, SIGNAL(changed()), SLOT(onTick()));
  menuRadioItem->parent->menu->addAction(menuRadioItem);
}

bool MenuRadioItem::visible() {
  return menuRadioItem->isVisible();
}

void MenuRadioItem::setVisible(bool visible) {
  menuRadioItem->setVisible(visible);
}

bool MenuRadioItem::enabled() {
  return menuRadioItem->isEnabled();
}

void MenuRadioItem::setEnabled(bool enabled) {
  menuRadioItem->setEnabled(enabled);
}

bool MenuRadioItem::checked() {
  return menuRadioItem->isChecked();
}

void MenuRadioItem::setChecked() {
  object->locked = true;
  menuRadioItem->setChecked(true);
  object->locked = false;
}

MenuRadioItem::MenuRadioItem() {
  menuRadioItem = new MenuRadioItem::Data(*this);
}
