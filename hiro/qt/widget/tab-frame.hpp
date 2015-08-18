#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto remove(sTabFrameItem item) -> void;
  auto setEdge(Edge edge) -> void;
  auto setGeometry(Geometry geometry) -> void override;

  auto _setState() -> void;

  QtTabFrame* qtTabFrame = nullptr;
};

}

#endif
