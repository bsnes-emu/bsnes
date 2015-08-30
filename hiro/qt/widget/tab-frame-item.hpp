#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrameItem : pObject {
  Declare(TabFrameItem, Object)

  auto append(sLayout layout) -> void;
  auto remove(sLayout layout) -> void;
  auto setClosable(bool closable) -> void;
  auto setImage(const Image& image) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setMovable(bool movable) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _parent() -> maybe<pTabFrame&>;
  auto _setState() -> void;

  QWidget* qtTabFrameItem = nullptr;
};

}

#endif
