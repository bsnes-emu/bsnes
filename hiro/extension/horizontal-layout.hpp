#if defined(Hiro_HorizontalLayout)

struct HorizontalLayout;
struct HorizontalLayoutCell;

struct mHorizontalLayout;
struct mHorizontalLayoutCell;

using sHorizontalLayout = shared_pointer<mHorizontalLayout>;
using sHorizontalLayoutCell = shared_pointer<mHorizontalLayoutCell>;

struct mHorizontalLayout : mSizable {
  using type = mHorizontalLayout;
  using mSizable::remove;

  auto alignment() const -> maybe<float>;
  auto append(sSizable sizable, Size size, float spacing = 5) -> type&;
  auto cell(uint position) const -> HorizontalLayoutCell;
  auto cell(sSizable sizable) const -> HorizontalLayoutCell;
  auto cellCount() const -> uint;
  auto minimumSize() const -> Size override;
  auto padding() const -> Geometry;
  auto remove(sSizable sizable) -> type&;
  auto remove(sHorizontalLayoutCell cell) -> type&;
  auto reset() -> type& override;
  auto setAlignment(maybe<float> alignment) -> type&;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setGeometry(Geometry geometry) -> type& override;
  auto setPadding(Geometry padding) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;
  auto setSpacing(float spacing) -> type&;
  auto setVisible(bool visible) -> type& override;
  auto spacing() const -> float;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    maybe<float> alignment;
    vector<HorizontalLayoutCell> cells;
    Geometry padding;
    float spacing = 5;
  } state;
};

struct mHorizontalLayoutCell : mObject {
  using type = mHorizontalLayoutCell;

  auto alignment() const -> maybe<float>;
  auto setAlignment(maybe<float> alignment) -> type&;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSizable(sSizable sizable) -> type&;
  auto setSize(Size size) -> type&;
  auto setSpacing(float spacing) -> type&;
  auto setVisible(bool visible) -> type& override;
  auto sizable() const -> Sizable;
  auto size() const -> Size;
  auto spacing() const -> float;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    maybe<float> alignment;
    sSizable sizable;
    Size size;
    float spacing = 5;
  } state;

  friend class mHorizontalLayout;
};

#endif
