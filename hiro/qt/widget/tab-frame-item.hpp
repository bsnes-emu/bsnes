#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrameItem : pObject {
  Declare(TabFrameItem, Object)

  auto append(sSizable sizable) -> void;
  auto remove(sSizable sizable) -> void;
  auto setClosable(bool closable) -> void;
  auto setIcon(const image& icon) -> void;
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
