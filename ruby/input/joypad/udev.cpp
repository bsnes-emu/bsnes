#ifndef RUBY_INPUT_JOYPAD_UDEV
#define RUBY_INPUT_JOYPAD_UDEV

struct InputJoypadUdev {
  Input& input;
  InputJoypadUdev(Input& input) : input(input) {}

  udev* context = nullptr;
  udev_monitor* monitor = nullptr;
  udev_enumerate* enumerator = nullptr;
  udev_list_entry* devices = nullptr;
  udev_list_entry* item = nullptr;

  struct JoypadInput {
    signed code = 0;
    unsigned id = 0;
    int16_t value = 0;
    input_absinfo info;

    JoypadInput() {}
    JoypadInput(signed code) : code(code) {}
    JoypadInput(signed code, unsigned id) : code(code), id(id) {}
    bool operator< (const JoypadInput& source) const { return code <  source.code; }
    bool operator==(const JoypadInput& source) const { return code == source.code; }
  };

  struct Joypad {
    shared_pointer<HID::Joypad> hid{new HID::Joypad};

    int fd = -1;
    dev_t device = 0;
    string deviceName;
    string deviceNode;

    uint8_t evbit[(EV_MAX + 7) / 8] = {0};
    uint8_t keybit[(KEY_MAX + 7) / 8] = {0};
    uint8_t absbit[(ABS_MAX + 7) / 8] = {0};
    uint8_t ffbit[(FF_MAX + 7) / 8] = {0};
    unsigned effects = 0;

    string name;
    string manufacturer;
    string product;
    string serial;
    string vendorID;
    string productID;

    set<JoypadInput> axes;
    set<JoypadInput> hats;
    set<JoypadInput> buttons;
    bool rumble = false;
    unsigned effectID = 0;
  };
  vector<Joypad> joypads;

  auto assign(shared_pointer<HID::Joypad> hid, unsigned groupID, unsigned inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    while(hotplugDevicesAvailable()) hotplugDevice();

    for(auto& jp : joypads) {
      input_event events[32];
      signed length = 0;
      while((length = read(jp.fd, events, sizeof(events))) > 0) {
        length /= sizeof(input_event);
        for(unsigned i = 0; i < length; i++) {
          signed code = events[i].code;
          signed type = events[i].type;
          signed value = events[i].value;

          if(type == EV_ABS) {
            if(auto input = jp.axes.find({code})) {
              signed range = input().info.maximum - input().info.minimum;
              value = (value - input().info.minimum) * 65535ll / range - 32767;
              assign(jp.hid, HID::Joypad::GroupID::Axis, input().id, sclamp<16>(value));
            } else if(auto input = jp.hats.find({code})) {
              signed range = input().info.maximum - input().info.minimum;
              value = (value - input().info.minimum) * 65535ll / range - 32767;
              assign(jp.hid, HID::Joypad::GroupID::Hat, input().id, sclamp<16>(value));
            }
          } else if(type == EV_KEY) {
            if(code >= BTN_MISC) {
              if(auto input = jp.buttons.find({code})) {
                assign(jp.hid, HID::Joypad::GroupID::Button, input().id, (bool)value);
              }
            }
          }
        }
      }

      devices.append(jp.hid);
    }
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    for(auto& jp : joypads) {
      if(jp.hid->id() != id) continue;
      if(!jp.hid->rumble()) continue;

      input_event play;
      memset(&play, 0, sizeof(input_event));
      play.type = EV_FF;
      play.code = jp.effectID;
      play.value = enable;
      auto unused = write(jp.fd, &play, sizeof(input_event));
      return true;
    }

    return false;
  }

  auto init() -> bool {
    context = udev_new();
    if(context == nullptr) return false;

    monitor = udev_monitor_new_from_netlink(context, "udev");
    if(monitor) {
      udev_monitor_filter_add_match_subsystem_devtype(monitor, "input", nullptr);
      udev_monitor_enable_receiving(monitor);
    }

    enumerator = udev_enumerate_new(context);
    if(enumerator) {
      udev_enumerate_add_match_property(enumerator, "ID_INPUT_JOYSTICK", "1");
      udev_enumerate_scan_devices(enumerator);
      devices = udev_enumerate_get_list_entry(enumerator);
      for(udev_list_entry* item = devices; item != nullptr; item = udev_list_entry_get_next(item)) {
        string name = udev_list_entry_get_name(item);
        udev_device* device = udev_device_new_from_syspath(context, name);
        string deviceNode = udev_device_get_devnode(device);
        if(deviceNode) createJoypad(device, deviceNode);
        udev_device_unref(device);
      }
    }

    return true;
  }

  auto term() -> void {
    if(enumerator) { udev_enumerate_unref(enumerator); enumerator = nullptr; }
  }

private:
  auto hotplugDevicesAvailable() -> bool {
    pollfd fd = {0};
    fd.fd = udev_monitor_get_fd(monitor);
    fd.events = POLLIN;
    return (::poll(&fd, 1, 0) == 1) && (fd.revents & POLLIN);
  }

  auto hotplugDevice() -> void {
    udev_device* device = udev_monitor_receive_device(monitor);
    if(device == nullptr) return;

    string value = udev_device_get_property_value(device, "ID_INPUT_JOYSTICK");
    string action = udev_device_get_action(device);
    string deviceNode = udev_device_get_devnode(device);
    if(value == "1") {
      if(action == "add") {
        createJoypad(device, deviceNode);
      }
      if(action == "remove") {
        removeJoypad(device, deviceNode);
      }
    }
  }

  auto createJoypad(udev_device* device, const string& deviceNode) -> void {
    Joypad jp;
    jp.deviceNode = deviceNode;

    struct stat st;
    if(stat(deviceNode, &st) < 0) return;
    jp.device = st.st_rdev;

    jp.fd = open(deviceNode, O_RDWR | O_NONBLOCK);
    if(jp.fd < 0) return;

    uint8_t evbit[(EV_MAX + 7) / 8] = {0};
    uint8_t keybit[(KEY_MAX + 7) / 8] = {0};
    uint8_t absbit[(ABS_MAX + 7) / 8] = {0};

    ioctl(jp.fd, EVIOCGBIT(0, sizeof(jp.evbit)), jp.evbit);
    ioctl(jp.fd, EVIOCGBIT(EV_KEY, sizeof(jp.keybit)), jp.keybit);
    ioctl(jp.fd, EVIOCGBIT(EV_ABS, sizeof(jp.absbit)), jp.absbit);
    ioctl(jp.fd, EVIOCGBIT(EV_FF, sizeof(jp.ffbit)), jp.ffbit);
    ioctl(jp.fd, EVIOCGEFFECTS, &jp.effects);

    #define testBit(buffer, bit) (buffer[(bit) >> 3] & 1 << ((bit) & 7))

    if(testBit(jp.evbit, EV_KEY)) {
      if(udev_device* parent = udev_device_get_parent_with_subsystem_devtype(device, "input", nullptr)) {
        jp.name = udev_device_get_sysattr_value(parent, "name");
        jp.vendorID = udev_device_get_sysattr_value(parent, "id/vendor");
        jp.productID = udev_device_get_sysattr_value(parent, "id/product");
        if(udev_device* root = udev_device_get_parent_with_subsystem_devtype(parent, "usb", "usb_device")) {
          if(jp.vendorID == udev_device_get_sysattr_value(root, "idVendor")
          && jp.productID == udev_device_get_sysattr_value(root, "idProduct")
          ) {
            jp.deviceName = udev_device_get_devpath(root);
            jp.manufacturer = udev_device_get_sysattr_value(root, "manufacturer");
            jp.product = udev_device_get_sysattr_value(root, "product");
            jp.serial = udev_device_get_sysattr_value(root, "serial");
          }
        }
      }

      unsigned axes = 0;
      unsigned hats = 0;
      unsigned buttons = 0;
      for(signed i = 0; i < ABS_MISC; i++) {
        if(testBit(jp.absbit, i)) {
          if(i >= ABS_HAT0X && i <= ABS_HAT3Y) {
            if(auto hat = jp.hats.insert({i, hats++})) {
              ioctl(jp.fd, EVIOCGABS(i), &hat().info);
            }
          } else {
            if(auto axis = jp.axes.insert({i, axes++})) {
              ioctl(jp.fd, EVIOCGABS(i), &axis().info);
            }
          }
        }
      }
      for(signed i = BTN_JOYSTICK; i < KEY_MAX; i++) {
        if(testBit(jp.keybit, i)) {
          jp.buttons.insert({i, buttons++});
        }
      }
      for(signed i = BTN_MISC; i < BTN_JOYSTICK; i++) {
        if(testBit(jp.keybit, i)) {
          jp.buttons.insert({i, buttons++});
        }
      }
      jp.rumble = jp.effects >= 2 && testBit(jp.ffbit, FF_RUMBLE);
      if(jp.rumble) {
        ff_effect effect;
        memset(&effect, 0, sizeof(ff_effect));
        effect.type = FF_RUMBLE;
        effect.id = -1;
        effect.u.rumble.strong_magnitude = 65535;
        effect.u.rumble.weak_magnitude   = 65535;
        ioctl(jp.fd, EVIOCSFF, &effect);
        jp.effectID = effect.id;
      }

      createJoypadHID(jp);
      joypads.append(jp);
    }

    #undef testBit
  }

  auto createJoypadHID(Joypad& jp) -> void {
    uint64_t pathID = Hash::CRC32(jp.deviceName.data(), jp.deviceName.size()).value();
    jp.hid->setID(pathID << 32 | hex(jp.vendorID) << 16 | hex(jp.productID) << 0);

    for(unsigned n = 0; n < jp.axes.size(); n++) jp.hid->axes().append(n);
    for(unsigned n = 0; n < jp.hats.size(); n++) jp.hid->hats().append(n);
    for(unsigned n = 0; n < jp.buttons.size(); n++) jp.hid->buttons().append(n);
    jp.hid->setRumble(jp.rumble);
  }

  auto removeJoypad(udev_device* device, const string& deviceNode) -> void {
    for(unsigned n = 0; n < joypads.size(); n++) {
      if(joypads[n].deviceNode == deviceNode) {
        close(joypads[n].fd);
        joypads.remove(n);
        return;
      }
    }
  }
};

#endif
