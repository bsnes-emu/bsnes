#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto remove(sTabFrameItem item) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setNavigation(Navigation navigation) -> void;
  auto setVisible(bool visible) -> void override;

  auto onChange() -> void;

  auto _buildImageList() -> void;
  auto _synchronizeSizable() -> void;

  HIMAGELIST imageList = nullptr;
};

}

#endif
