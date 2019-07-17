#if defined(Hiro_Action)

namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
  @autoreleasepool {
    [cocoaAction setEnabled:enabled];
  }
}

auto pAction::setVisible(bool visible) -> void {
  @autoreleasepool {
    [cocoaAction setHidden:!visible];
  }
}

}

#endif
