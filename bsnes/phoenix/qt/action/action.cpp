void pAction::setEnabled(bool enabled) {
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->setEnabled(enabled);
  } else if(dynamic_cast<MenuSeparator*>(&action)) {
    ((MenuSeparator&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<MenuItem*>(&action)) {
    ((MenuItem&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<MenuCheckItem*>(&action)) {
    ((MenuCheckItem&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<MenuRadioItem*>(&action)) {
    ((MenuRadioItem&)action).p.qtAction->setEnabled(enabled);
  }
}

void pAction::setVisible(bool visible) {
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->setVisible(visible);
  } else if(dynamic_cast<MenuSeparator*>(&action)) {
    ((MenuSeparator&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<MenuItem*>(&action)) {
    ((MenuItem&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<MenuCheckItem*>(&action)) {
    ((MenuCheckItem&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<MenuRadioItem*>(&action)) {
    ((MenuRadioItem&)action).p.qtAction->setVisible(visible);
  }
}

pAction::pAction(Action &action) : action(action) {
}
