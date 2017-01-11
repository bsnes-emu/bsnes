Peripherals peripherals;

auto Peripherals::unload() -> void {
  delete controllerPort;
  controllerPort = nullptr;
}

auto Peripherals::reset() -> void {
  connect(ID::Port::Controller, settings.controllerPort);
}

auto Peripherals::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller) {
    settings.controllerPort = device;
    if(!system.loaded()) return;

    delete controllerPort;
    switch(device) { default:
    case ID::Device::None:    controllerPort = new Controller; break;
    case ID::Device::Gamepad: controllerPort = new Gamepad; break;
    }
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort);
}
