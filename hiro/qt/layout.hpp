#if defined(Hiro_Layout)

namespace hiro {

struct pLayout : pSizable {
  Declare(Layout, Sizable)

  auto setVisible(bool visible) -> void override;
};

}

#endif
