Peripherals peripherals;

auto Peripherals::unload() -> void {
  delete controllerPort1;
  delete controllerPort2;
  controllerPort1 = nullptr;
  controllerPort2 = nullptr;
}

auto Peripherals::reset() -> void {
  connect(ID::Port::Controller1, settings.controllerPort1);
  connect(ID::Port::Controller2, settings.controllerPort2);
}

auto Peripherals::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) {
    settings.controllerPort1 = device;
    if(!system.loaded()) return;

    delete controllerPort1;
    switch(device) { default:
    case ID::Device::None:    controllerPort1 = new Controller(0); break;
    case ID::Device::Gamepad: controllerPort1 = new Gamepad(0); break;
    }
  }

  if(port == ID::Port::Controller2) {
    settings.controllerPort2 = device;
    if(!system.loaded()) return;

    delete controllerPort2;
    switch(device) { default:
    case ID::Device::None:    controllerPort2 = new Controller(1); break;
    case ID::Device::Gamepad: controllerPort2 = new Gamepad(1); break;
    }
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort1);
  cpu.peripherals.append(controllerPort2);
}
