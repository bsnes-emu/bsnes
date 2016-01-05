#if defined(Hiro_MenuBar)

namespace hiro {

struct pMenuBar : pObject {
  Declare(MenuBar, Object)

  auto append(sMenu menu) -> void;
  auto remove(sMenu menu) -> void;

  auto _parent() -> maybe<pWindow&>;
};

}

#endif
