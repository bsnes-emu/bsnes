#if defined(Hiro_FixedLayout)

struct FixedLayout;
struct FixedLayoutCell;

struct mFixedLayout;
struct mFixedLayoutCell;

using sFixedLayout = shared_pointer<mFixedLayout>;
using sFixedLayoutCell = shared_pointer<mFixedLayoutCell>;

struct mFixedLayout : mSizable {
  using type = mFixedLayout;
  using mSizable::remove;

  auto append(sSizable sizable, Geometry geometry) -> type&;
  auto cell(uint position) const -> FixedLayoutCell;
  auto cell(sSizable sizable) const -> FixedLayoutCell;
  auto cellCount() const -> uint;
  auto minimumSize() const -> Size override;
  auto remove(sSizable sizable) -> type&;
  auto remove(sFixedLayoutCell cell) -> type&;
  auto reset() -> type& override;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setVisible(bool visible) ->type& override;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    vector<FixedLayoutCell> cells;
  } state;
};

struct mFixedLayoutCell : mObject {
  using type = mFixedLayoutCell;

  auto geometry() const -> Geometry;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setGeometry(Geometry geometry) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSizable(sSizable sizable) -> type&;
  auto setVisible(bool visible) -> type& override;
  auto sizable() const -> Sizable;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    Geometry geometry;
    sSizable sizable;
  } state;

  friend class mFixedLayout;
};

#endif
