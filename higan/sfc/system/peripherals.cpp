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
  connect(ID::Port::Controller1, settings.controllerPort1);
  connect(ID::Port::Controller2, settings.controllerPort2);
  connect(ID::Port::Expansion, settings.expansionPort);
}

auto Peripherals::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) {
    settings.controllerPort1 = device;
    if(!system.loaded()) return;

    delete controllerPort1;
    switch(device) { default:
    case ID::Device::None:    controllerPort1 = new Controller(0); break;
    case ID::Device::Gamepad: controllerPort1 = new Gamepad(0); break;
    case ID::Device::Mouse:   controllerPort1 = new Mouse(0); break;
    }
  }

  if(port == ID::Port::Controller2) {
    settings.controllerPort2 = device;
    if(!system.loaded()) return;

    delete controllerPort2;
    switch(device) { default:
    case ID::Device::None:          controllerPort2 = new Controller(1); break;
    case ID::Device::Gamepad:       controllerPort2 = new Gamepad(1); break;
    case ID::Device::Mouse:         controllerPort2 = new Mouse(1); break;
    case ID::Device::SuperMultitap: controllerPort2 = new SuperMultitap(1); break;
    case ID::Device::SuperScope:    controllerPort2 = new SuperScope(1); break;
    case ID::Device::Justifier:     controllerPort2 = new Justifier(1, false); break;
    case ID::Device::Justifiers:    controllerPort2 = new Justifier(1, true); break;
    }
  }

  if(port == ID::Port::Expansion) {
    settings.expansionPort = device;
    if(!system.loaded()) return;

    delete expansionPort;
    switch(device) { default:
    case ID::Device::None:        expansionPort = new Expansion; break;
    case ID::Device::Satellaview: expansionPort = new Satellaview; break;
    case ID::Device::S21FX:       expansionPort = new S21FX; break;
    }
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort1);
  cpu.peripherals.append(controllerPort2);
  cpu.peripherals.append(expansionPort);
}
