#include <ms/ms.hpp>

namespace MasterSystem {

System system;
Scheduler scheduler;

auto System::run() -> void {
}

auto System::load() -> bool {
  return false;
}

auto System::save() -> void {
}

auto System::unload() -> void {
}

auto System::power() -> void {
}

auto System::reset() -> void {
}

}
