void Menu::setText(const string &text) {
  menu->setTitle(QString::fromUtf8(text));
}

void Menu::append(Action &item) {
  if(dynamic_cast<Menu*>(&item)) {
    menu->addMenu(((Menu*)&item)->menu);
  } else if(dynamic_cast<MenuSeparator*>(&item)) {
    menu->addSeparator();
  } else if(dynamic_cast<MenuItem*>(&item)) {
    menu->addAction(((MenuItem*)&item)->menuItem);
  } else if(dynamic_cast<MenuCheckItem*>(&item)) {
    menu->addAction(((MenuCheckItem*)&item)->menuCheckItem);
  } else if(dynamic_cast<MenuRadioItem*>(&item)) {
    MenuRadioItem &radioItem = (MenuRadioItem&)item;
    menu->addAction(radioItem.menuRadioItem);
    if(radioItem.menuRadioItem->actionGroup()->checkedAction() == 0) radioItem.setChecked();
  }
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
