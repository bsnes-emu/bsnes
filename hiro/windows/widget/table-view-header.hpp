#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewHeader : pObject {
  Declare(TableViewHeader, Object)

  auto append(sTableViewColumn column) -> void;
  auto remove(sTableViewColumn column) -> void;
  auto setVisible(bool visible) -> void override;

  auto _parent() -> maybe<pTableView&>;
  auto _setState() -> void;
};

}

#endif
