#if defined(Hiro_IconView)

namespace hiro {

struct pIconView : pWidget {
  Declare(IconView, Widget)

  auto append(sIconViewItem item) -> void;
  auto remove(sIconViewItem item) -> void;
  auto reset() -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setFlow(Orientation flow) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setItemImage(unsigned position, const Image& image) -> void;
  auto setItemSelected(unsigned position, bool selected) -> void;
  auto setItemSelected(const vector<signed>& selections) -> void;
  auto setItemSelectedAll() -> void;
  auto setItemSelectedNone() -> void;
  auto setItemText(unsigned position, const string& text) -> void;
  auto setOrientation(Orientation orientation) -> void;

  auto _updateSelected() -> void;

  GtkWidget* subWidget = nullptr;
  GtkListStore* store = nullptr;
  vector<unsigned> currentSelection;
};

}

#endif
