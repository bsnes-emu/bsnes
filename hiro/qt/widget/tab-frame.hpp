#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto remove(sTabFrameItem item) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setNavigation(Navigation navigation) -> void;

  auto _setState() -> void;

  QtTabFrame* qtTabFrame = nullptr;
};

}

#endif
