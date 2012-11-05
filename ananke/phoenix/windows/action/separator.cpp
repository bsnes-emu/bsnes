void pSeparator::constructor() {
}

void pSeparator::destructor() {
  if(parentMenu) parentMenu->remove(separator);
}
