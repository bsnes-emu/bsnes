#if defined(Hiro_FixedLayout)
using sFixedLayout = shared_pointer<mFixedLayout>;
struct FixedLayout : sFixedLayout {
  DeclareSharedLayout(FixedLayout)

  auto append(sSizable sizable, Geometry geometry) { return self().append(sizable, geometry), *this; }
};
#endif

#if defined(Hiro_HorizontalLayout)
using sHorizontalLayout = shared_pointer<mHorizontalLayout>;
struct HorizontalLayout : sHorizontalLayout {
  DeclareSharedLayout(HorizontalLayout)

  auto append(sSizable sizable, Size size, signed spacing = 5) { return self().append(sizable, size, spacing), *this; }
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
  auto setAlignment(double alignment = 0.0) { return self().setAlignment(alignment), *this; }
  auto setMargin(signed margin = 0) { return self().setMargin(margin), *this; }
  auto setSpacing(signed spacing = 5) { return self().setSpacing(spacing), *this; }
};
#endif
