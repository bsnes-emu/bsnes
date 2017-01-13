Peripherals peripherals;

auto Peripherals::unload() -> void {
  delete hardware;
  delete controllerPort1;
  delete controllerPort2;
  hardware = nullptr;
  controllerPort1 = nullptr;
  controllerPort2 = nullptr;
}

auto Peripherals::reset() -> void {
  connect(ID::Port::Hardware, settings.hardware);
  connect(ID::Port::Controller1, settings.controllerPort1);
  connect(ID::Port::Controller2, settings.controllerPort2);
}

auto Peripherals::connect(uint port, uint device) -> void {
  cpu.peripherals.reset();

  if(system.model() == Model::MasterSystem) {
    if(port == ID::Port::Hardware) {
      settings.hardware = device;
      if(!system.loaded()) return;

      delete hardware;
      hardware = new MasterSystemControls(ID::Port::Hardware);
    }

    if(port == ID::Port::Controller1) {
      settings.controllerPort1 = device;
      if(!system.loaded()) return;

      delete controllerPort1;
      switch(device) { default:
      case ID::Device::None:    controllerPort1 = new Controller(ID::Port::Controller1); break;
      case ID::Device::Gamepad: controllerPort1 = new Gamepad(ID::Port::Controller1); break;
      }
    }

    if(port == ID::Port::Controller2) {
      settings.controllerPort2 = device;
      if(!system.loaded()) return;

      delete controllerPort2;
      switch(device) { default:
      case ID::Device::None:    controllerPort2 = new Controller(ID::Port::Controller2); break;
      case ID::Device::Gamepad: controllerPort2 = new Gamepad(ID::Port::Controller2); break;
      }
    }

    cpu.peripherals.append(hardware);
    cpu.peripherals.append(controllerPort1);
    cpu.peripherals.append(controllerPort2);
  }

  if(system.model() == Model::GameGear) {
    if(port == ID::Port::Hardware) {
      settings.hardware = device;
      if(!system.loaded()) return;

      delete hardware;
      hardware = new GameGearControls(ID::Port::Hardware);
    }

    cpu.peripherals.append(hardware);
  }
}
