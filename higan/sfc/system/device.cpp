Device device;

Device::Device() {
  connect(0, ID::None);
  connect(1, ID::None);
  connect(2, ID::None);
}

Device::~Device() {
  if(controllerPort1) delete controllerPort1;
  if(controllerPort2) delete controllerPort2;
}

auto Device::connect(uint port, Device::ID id) -> void {
  if(port == 0 || port == 1) {
    Controller*& controller = (port == 0 ? controllerPort1 : controllerPort2);

    if(controller) {
      delete controller;
      controller = nullptr;
    }

    switch(id) { default:
    case ID::None: controller = new Controller(port); break;
    case ID::Gamepad: controller = new Gamepad(port); break;
    case ID::Multitap: controller = new Multitap(port); break;
    case ID::Mouse: controller = new Mouse(port); break;
    case ID::SuperScope: controller = new SuperScope(port); break;
    case ID::Justifier: controller = new Justifier(port, false); break;
    case ID::Justifiers: controller = new Justifier(port, true); break;
    case ID::USART: controller = new USART(port); break;
    }

    switch(port) {
    case 0: settings.controllerPort1 = (uint)id; break;
    case 1: settings.controllerPort2 = (uint)id; break;
    }
  }

  if(port == 2) {
    settings.expansionPort = (uint)id;
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort1);
  cpu.peripherals.append(controllerPort2);
}
