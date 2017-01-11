#include <processor/processor.hpp>
#include "huc6280.hpp"

namespace Processor {

auto HuC6280::power() -> void {
}

auto HuC6280::instruction() -> void {
  step(1);
}

}
