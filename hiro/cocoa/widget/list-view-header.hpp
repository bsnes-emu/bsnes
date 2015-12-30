#if defined(Hiro_ListView)

namespace hiro {

struct pListViewHeader : pObject {
  Declare(ListViewHeader, Object)

  auto append(sListViewColumn column) -> void;
  auto remove(sListViewColumn column) -> void;
};

}

#endif
