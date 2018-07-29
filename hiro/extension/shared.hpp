#if defined(Hiro_FixedLayout)
struct FixedLayoutCell : sFixedLayoutCell {
  DeclareSharedObject(FixedLayoutCell)

  auto geometry() const { return self().geometry(); }
  auto setGeometry(Geometry geometry) { return self().setGeometry(geometry), *this; }
  auto setSizable(sSizable sizable) { return self().setSizable(sizable), *this; }
  auto sizable() const { return self().sizable(); }
};

struct FixedLayout : sFixedLayout {
  DeclareSharedSizable(FixedLayout)

  auto append(sSizable sizable, Geometry geometry) { return self().append(sizable, geometry), *this; }
  auto cell(uint position) const { return self().cell(position); }
  auto cell(sSizable sizable) const { return self().cell(sizable); }
  auto cellCount() const { return self().cellCount(); }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto remove(sFixedLayoutCell cell) { return self().remove(cell), *this; }
  auto reset() { return self().reset(), *this; }
};
#endif

#if defined(Hiro_HorizontalLayout)
struct HorizontalLayoutCell : sHorizontalLayoutCell {
  DeclareSharedObject(HorizontalLayoutCell)

  auto alignment() const { return self().alignment(); }
  auto setAlignment(maybe<float> alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setSizable(sSizable sizable) { return self().setSizable(sizable), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
  auto sizable() const { return self().sizable(); }
  auto size() const { return self().size(); }
  auto spacing() const { return self().spacing(); }
};

struct HorizontalLayout : sHorizontalLayout {
  DeclareSharedSizable(HorizontalLayout)

  auto alignment() const { return self().alignment(); }
  auto append(sSizable sizable, Size size, float spacing = 5) { return self().append(sizable, size, spacing), *this; }
  auto cell(uint position) const { return self().cell(position); }
  auto cell(sSizable sizable) const { return self().cell(sizable); }
  auto cellCount() const { return self().cellCount(); }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto remove(sHorizontalLayoutCell cell) { return self().remove(cell), *this; }
  auto reset() { return self().reset(), *this; }
  auto setAlignment(maybe<float> alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setPadding(float padding) { return self().setPadding({padding, padding, padding, padding}), *this; }
  auto setPadding(Geometry padding = {}) { return self().setPadding(padding), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
};
#endif

#if defined(Hiro_VerticalLayout)
struct VerticalLayoutCell : sVerticalLayoutCell {
  DeclareSharedObject(VerticalLayoutCell)

  auto alignment() const { return self().alignment(); }
  auto setAlignment(maybe<float> alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setSizable(sSizable sizable) { return self().setSizable(sizable), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
  auto sizable() const { return self().sizable(); }
  auto size() const { return self().size(); }
  auto spacing() const { return self().spacing(); }
};

struct VerticalLayout : sVerticalLayout {
  DeclareSharedSizable(VerticalLayout)

  auto alignment() const { return self().alignment(); }
  auto append(sSizable sizable, Size size, float spacing = 5) { return self().append(sizable, size, spacing), *this; }
  auto cell(uint position) const { return self().cell(position); }
  auto cell(sSizable sizable) const { return self().cell(sizable); }
  auto cellCount() const { return self().cellCount(); }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto remove(sVerticalLayoutCell cell) { return self().remove(cell), *this; }
  auto reset() { return self().reset(), *this; }
  auto setAlignment(maybe<float> alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setPadding(float padding) { return self().setPadding({padding, padding, padding, padding}), *this; }
  auto setPadding(Geometry padding = {}) { return self().setPadding(padding), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
};
#endif

#if defined(Hiro_TableLayout)
struct TableLayoutCell : sTableLayoutCell {
  DeclareSharedObject(TableLayoutCell)

  auto alignment() const { return self().alignment(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setSizable(sSizable sizable) { return self().setSizable(sizable), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto sizable() const { return self().sizable(); }
  auto size() const { return self().size(); }
};

struct TableLayoutColumn : sTableLayoutColumn {
  DeclareSharedObject(TableLayoutColumn)

  auto alignment() const { return self().alignment(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
  auto spacing() const { return self().spacing(); }
};

struct TableLayoutRow : sTableLayoutRow {
  DeclareSharedObject(TableLayoutRow)

  auto alignment() const { return self().alignment(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setSpacing(float spacing = 5) { return self().setSpacing(spacing), *this; }
  auto spacing() const { return self().spacing(); }
};

struct TableLayout : sTableLayout {
  DeclareSharedSizable(TableLayout)

  auto alignment() const { return self().alignment(); }
  auto append(sSizable sizable, Size size) { return self().append(sizable, size), *this; }
  auto cell(uint position) const { return self().cell(position); }
  auto cell(uint x, uint y) const { return self().cell(x, y); }
  auto cell(sSizable sizable) const { return self().cell(sizable); }
  auto cellCount() const { return self().cellCount(); }
  auto column(uint position) const { return self().column(position); }
  auto columnCount() const { return self().columnCount(); }
  auto padding() const { return self().padding(); }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto remove(sTableLayoutCell cell) { return self().remove(cell), *this; }
  auto reset() { return self().reset(), *this; }
  auto row(uint position) const { return self().row(position); }
  auto rowCount() const { return self().rowCount(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setPadding(float padding) { return self().setPadding({padding, padding, padding, padding}), *this; }
  auto setPadding(Geometry padding = {}) { return self().setPadding(padding), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto size() const { return self().size(); }
};
#endif

#if defined(Hiro_ListView)
struct ListViewItem : sListViewItem {
  DeclareSharedObject(ListViewItem)

  auto alignment() const { return self().alignment(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto checkable() const { return self().checkable(); }
  auto checked() const { return self().checked(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto icon() const { return self().icon(); }
  auto reset() { return self().reset(), *this; }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) { return self().setCheckable(checkable), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};

struct ListView : sListView {
  DeclareSharedWidget(ListView)

  auto alignment() const { return self().alignment(); }
  auto append(sListViewItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto batchable() const { return self().batchable(); }
  auto batched() const { return self().batched(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto doToggle(ListViewItem item) const { return self().doToggle(item); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(uint position) const { return self().item(position); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto onToggle(const function<void (ListViewItem)>& callback = {}) { return self().onToggle(callback), *this; }
  auto remove(sListViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() { return self().selected(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setBatchable(bool batchable = true) { return self().setBatchable(batchable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
};
#endif
