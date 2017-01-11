//Hudson Soft HuC6280A

#pragma once

namespace Processor {

struct HuC6280 {
  virtual auto step(uint clocks) -> void = 0;

  auto power() -> void;
  auto instruction() -> void;
};

}
