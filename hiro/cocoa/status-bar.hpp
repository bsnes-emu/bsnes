#if defined(Hiro_StatusBar)

namespace hiro {

struct pStatusBar : pObject {
  Declare(StatusBar, Object)

  auto setText(const string& text) -> void;
};

}

#endif
