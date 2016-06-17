Peripherals peripherals;

auto Peripherals::unload() -> void {
  delete controllerPort1;
  delete controllerPort2;
  delete expansionPort;
  controllerPort1 = nullptr;
  controllerPort2 = nullptr;
  expansionPort = nullptr;
}

auto Peripherals::reset() -> void {
  connect(0, settings.controllerPort1);
  connect(1, settings.controllerPort2);
  connect(2, settings.expansionPort);
}

auto Peripherals::connect(uint port, uint id) -> void {
  if(port == Port::Controller1) {
    settings.controllerPort1 = id;
    if(!system.loaded()) return;

    delete controllerPort1;
    switch(id) { default:
    case Device::None:     controllerPort1 = new Controller(0); break;
    case Device::Gamepad:  controllerPort1 = new Gamepad(0); break;
    case Device::Multitap: controllerPort1 = new Multitap(0); break;
    case Device::Mouse:    controllerPort1 = new Mouse(0); break;
    }
  }

  if(port == Port::Controller2) {
    settings.controllerPort2 = id;
    if(!system.loaded()) return;

    delete controllerPort2;
    switch(id) { default:
    case Device::None:       controllerPort2 = new Controller(1); break;
    case Device::Gamepad:    controllerPort2 = new Gamepad(1); break;
    case Device::Multitap:   controllerPort2 = new Multitap(1); break;
    case Device::Mouse:      controllerPort2 = new Mouse(1); break;
    case Device::SuperScope: controllerPort2 = new SuperScope(1); break;
    case Device::Justifier:  controllerPort2 = new Justifier(1, false); break;
    case Device::Justifiers: controllerPort2 = new Justifier(1, true); break;
    }
  }

  if(port == Port::Expansion) {
    settings.expansionPort = id;
    if(!system.loaded()) return;

    delete expansionPort;
    switch(id) { default:
    case Device::None:        expansionPort = new Expansion; break;
    case Device::Satellaview: expansionPort = new Satellaview; break;
    case Device::SuperDisc:   expansionPort = new SuperDisc; break;
    case Device::S21FX:       expansionPort = new S21FX; break;
    }
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort1);
  cpu.peripherals.append(controllerPort2);
  cpu.peripherals.append(expansionPort);
}
