namespace phoenix {

void pPopupMenu::append(Action& action) {
  if(dynamic_cast<Menu*>(&action)) {
    qtMenu->addMenu(((Menu&)action).p.qtMenu);
  } else if(dynamic_cast<Separator*>(&action)) {
    qtMenu->addAction(((Separator&)action).p.qtAction);
  } else if(dynamic_cast<Item*>(&action)) {
    qtMenu->addAction(((Item&)action).p.qtAction);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    qtMenu->addAction(((CheckItem&)action).p.qtAction);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    qtMenu->addAction(((RadioItem&)action).p.qtAction);
  }
}

void pPopupMenu::remove(Action& action) {
  if(dynamic_cast<Menu*>(&action)) {
    //QMenu::removeMenu() does not exist
    qtMenu->clear();
    for(auto& action : popupMenu.state.action) append(action);
  } else if(dynamic_cast<Separator*>(&action)) {
    qtMenu->removeAction(((Separator&)action).p.qtAction);
  } else if(dynamic_cast<Item*>(&action)) {
    qtMenu->removeAction(((Item&)action).p.qtAction);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    qtMenu->removeAction(((CheckItem&)action).p.qtAction);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    qtMenu->removeAction(((RadioItem&)action).p.qtAction);
  }
}

void pPopupMenu::setVisible() {
  qtMenu->popup(QCursor::pos());
}

void pPopupMenu::constructor() {
  qtMenu = new QMenu;
}

void pPopupMenu::destructor() {
  delete qtMenu;
  qtMenu = nullptr;
}

}
