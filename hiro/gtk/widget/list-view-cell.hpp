#if defined(Hiro_ListView)

namespace hiro {

struct pListViewCell : pObject {
  Declare(ListViewCell, Object)

  auto setBackgroundColor(Color color) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pListViewItem*;
};

}

#endif
