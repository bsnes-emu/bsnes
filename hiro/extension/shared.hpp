#if defined(Hiro_FixedLayout)
using sFixedLayout = shared_pointer<mFixedLayout>;
struct FixedLayout : sFixedLayout {
  DeclareSharedLayout(FixedLayout)

  auto append(sSizable sizable, Geometry geometry) { return self().append(sizable, geometry), *this; }
  auto modify(sSizable sizable, Geometry geometry) { return self().modify(sizable, geometry), *this; }
};
#endif

#if defined(Hiro_HorizontalLayout)
using sHorizontalLayout = shared_pointer<mHorizontalLayout>;
struct HorizontalLayout : sHorizontalLayout {
  DeclareSharedLayout(HorizontalLayout)

  auto append(sSizable sizable, Size size, signed spacing = 5) { return self().append(sizable, size, spacing), *this; }
  auto modify(sSizable sizable, Size size, signed spacing = 5) { return self().modify(sizable, size, spacing), *this; }
  auto setAlignment(double alignment = 0.5) { return self().setAlignment(alignment), *this; }
  auto setMargin(signed margin = 0) { return self().setMargin(margin), *this; }
  auto setSpacing(signed spacing = 5) { return self().setSpacing(spacing), *this; }
};
#endif

#if defined(Hiro_VerticalLayout)
using sVerticalLayout = shared_pointer<mVerticalLayout>;
struct VerticalLayout : sVerticalLayout {
  DeclareSharedLayout(VerticalLayout)

  auto append(sSizable sizable, Size size, signed spacing = 5) { return self().append(sizable, size, spacing), *this; }
  auto modify(sSizable sizable, Size size, signed spacing = 5) { return self().modify(sizable, size, spacing), *this; }
  auto setAlignment(double alignment = 0.0) { return self().setAlignment(alignment), *this; }
  auto setMargin(signed margin = 0) { return self().setMargin(margin), *this; }
  auto setSpacing(signed spacing = 5) { return self().setSpacing(spacing), *this; }
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
#endif

#if defined(Hiro_ListView)
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
