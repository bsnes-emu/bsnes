namespace phoenix {

void pAction::setEnabled(bool enabled) {
  @autoreleasepool {
    [cocoaAction setEnabled:enabled];
  }
}

void pAction::setVisible(bool visible) {
  @autoreleasepool {
    [cocoaAction setHidden:!visible];
  }
}

void pAction::constructor() {
}

void pAction::destructor() {
}

}
