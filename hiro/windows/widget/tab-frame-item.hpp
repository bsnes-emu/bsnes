#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrameItem : pObject {
  Declare(TabFrameItem, Object)

  auto append(sLayout layout) -> void;
  auto remove(sLayout layout) -> void;
  auto setClosable(bool closable) -> void;
  auto setIcon(const image& icon) -> void;
  auto setMovable(bool movable) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> maybe<pTabFrame&>;
};

}

#endif
