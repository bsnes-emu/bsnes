#if defined(Hiro_MenuSeparator)

namespace hiro {

struct pMenuSeparator : pAction {
  Declare(MenuSeparator, Action)

  NSMenuItem* cocoaSeparator = nullptr;
};

}

#endif
