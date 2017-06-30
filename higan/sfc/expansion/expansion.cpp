#include <sfc/sfc.hpp>

namespace SuperFamicom {

ExpansionPort expansionPort;

Expansion::Expansion() {
  if(!handle()) create(Expansion::Enter, 1);
}

Expansion::~Expansion() {
  scheduler.remove(*this);
}

auto Expansion::Enter() -> void {
  while(true) scheduler.synchronize(), expansionPort.device->main();
}

auto Expansion::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ExpansionPort::connect(uint deviceID) -> void {
  if(!system.loaded()) return;
  delete device;

  switch(deviceID) { default:
  case ID::Device::None: device = new Expansion; break;
  case ID::Device::Satellaview: device = new Satellaview; break;
  case ID::Device::S21FX: device = new S21FX; break;
  }

  cpu.peripherals.reset();
  if(auto device = controllerPort1.device) cpu.peripherals.append(device);
  if(auto device = controllerPort2.device) cpu.peripherals.append(device);
  if(auto device = expansionPort.device) cpu.peripherals.append(device);
}

auto ExpansionPort::power() -> void {
}

auto ExpansionPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ExpansionPort::serialize(serializer& s) -> void {
}

}
