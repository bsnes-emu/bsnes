struct Expansion : Cothread {
  Expansion();
  static auto Enter() -> void;
  virtual auto main() -> void;
};

#include <sfc/expansion/satellaview/satellaview.hpp>
#include <sfc/expansion/superdisc/superdisc.hpp>
#include <sfc/expansion/21fx/21fx.hpp>
