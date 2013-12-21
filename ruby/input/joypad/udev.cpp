namespace ruby {

struct InputJoypadUdev {
  udev* context = nullptr;
  udev_monitor* monitor = nullptr;
  udev_enumerate* enumerator = nullptr;
  udev_list_entry* devices = nullptr;
  udev_list_entry* item = nullptr;

  struct JoystickInput {
    signed code = 0;
    unsigned id = 0;
    int16_t value = 0;
    input_absinfo info;

    JoystickInput() {}
    JoystickInput(signed code) : code(code) {}
    JoystickInput(signed code, unsigned id) : code(code), id(id) {}
    bool operator< (const JoystickInput& source) const { return code <  source.code; }
    bool operator==(const JoystickInput& source) const { return code == source.code; }
  };

  struct Joystick {
    string path;
    dev_t device = 0;
    int fd = -1;
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

    set<JoystickInput> axes;
    set<JoystickInput> hats;
    set<JoystickInput> buttons;
    bool rumble = false;
    unsigned effectID = 0;
  };
  vector<Joystick> joysticks;

  bool poll(int16_t* table) {
    unsigned i = 0;
    for(auto& js : joysticks) {
      input_event events[32];
      signed length = 0;
      while((length = read(js.fd, events, sizeof(events))) > 0) {
        length /= sizeof(input_event);
        for(unsigned i = 0; i < length; i++) {
          signed code = events[i].code;
          signed type = events[i].type;
          signed value = events[i].value;

          if(type == EV_ABS) {
            if(auto input = js.axes.find({code})) {
              signed range = input().info.maximum - input().info.minimum;
              signed axis = (value - input().info.minimum) * 65535ll / range - 32767;
              if(axis > +32767) axis = +32767;
              if(axis < -32768) axis = -32768;
              input().value = axis;
            }
            if(auto input = js.hats.find({code})) {
              input().value = value;
            }
          }

          if(type == EV_KEY) {
            if(code >= BTN_MISC) {
              if(auto input = js.buttons.find({code})) {
                input().value = value;
              }
            }
          }
        }
      }

      for(auto input : js.axes) {
        table[joypad(i).axis(input.id)] = input.value;
      }

      for(unsigned id = 0; id < (js.hats.size() + 1) / 2; id++) {
        table[joypad(i).hat(id)] = 0;
      }

      for(auto input : js.hats) {
        unsigned hat = 0;
        if(input.code == ABS_HAT0X || input.code == ABS_HAT0Y) hat = 0;
        if(input.code == ABS_HAT1X || input.code == ABS_HAT1Y) hat = 1;
        if(input.code == ABS_HAT2X || input.code == ABS_HAT2Y) hat = 2;
        if(input.code == ABS_HAT3X || input.code == ABS_HAT3Y) hat = 3;

        bool orientation = 0;
        if(input.code == ABS_HAT0X || input.code == ABS_HAT1X || input.code == ABS_HAT2X || input.code == ABS_HAT3X) orientation = 0;
        if(input.code == ABS_HAT0Y || input.code == ABS_HAT1Y || input.code == ABS_HAT2Y || input.code == ABS_HAT3Y) orientation = 1;

        signed value = 0;
        if(orientation == 0) {
          if(input.value < 0) value |= Joypad::HatLeft;
          if(input.value > 0) value |= Joypad::HatRight;
        } else {
          if(input.value < 0) value |= Joypad::HatUp;
          if(input.value > 0) value |= Joypad::HatDown;
        }

        table[joypad(i).hat(hat)] |= value;
      }

      for(auto input : js.buttons) {
        table[joypad(i).button(input.id)] = input.value;
      }

      i++;
    }

    return true;
  }

  void rumble(unsigned id, bool enable) {
    if(id >= joysticks.size()) return;

    Joystick& js = joysticks[id];
    if(js.rumble == false) return;

    input_event play;
    memset(&play, 0, sizeof(input_event));
    play.type = EV_FF;
    play.code = js.effectID;
    play.value = enable;
    write(js.fd, &play, sizeof(input_event));
  }

  bool init() {
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
        const char* name = udev_list_entry_get_name(item);
        struct udev_device* device = udev_device_new_from_syspath(context, name);
        const char* deviceNode = udev_device_get_devnode(device);
        if(deviceNode) createJoystick(device, deviceNode);
        udev_device_unref(device);
      }
    }

    return true;
  }

  void term() {
    if(enumerator) { udev_enumerate_unref(enumerator); enumerator = nullptr; }
  }

private:
  void createJoystick(udev_device* device, const char* path) {
    Joystick js;
    js.path = path;

    struct stat st;
    if(stat(path, &st) < 0) return;
    js.device = st.st_rdev;

    js.fd = open(path, O_RDWR | O_NONBLOCK);
    if(js.fd < 0) return;

    uint8_t evbit[(EV_MAX + 7) / 8] = {0};
    uint8_t keybit[(KEY_MAX + 7) / 8] = {0};
    uint8_t absbit[(ABS_MAX + 7) / 8] = {0};

    ioctl(js.fd, EVIOCGBIT(0, sizeof(js.evbit)), js.evbit);
    ioctl(js.fd, EVIOCGBIT(EV_KEY, sizeof(js.keybit)), js.keybit);
    ioctl(js.fd, EVIOCGBIT(EV_ABS, sizeof(js.absbit)), js.absbit);
    ioctl(js.fd, EVIOCGBIT(EV_FF, sizeof(js.ffbit)), js.ffbit);
    ioctl(js.fd, EVIOCGEFFECTS, &js.effects);

    #define testBit(buffer, bit) (buffer[(bit) >> 3] & 1 << ((bit) & 7))

    if(testBit(js.evbit, EV_KEY)) {
      if(udev_device* parent = udev_device_get_parent_with_subsystem_devtype(device, "input", nullptr)) {
        js.name = udev_device_get_sysattr_value(parent, "name");
        js.vendorID = udev_device_get_sysattr_value(parent, "id/vendor");
        js.productID = udev_device_get_sysattr_value(parent, "id/product");
        if(udev_device* root = udev_device_get_parent_with_subsystem_devtype(parent, "usb", "usb_device")) {
          if(js.vendorID == udev_device_get_sysattr_value(root, "idVendor")
          && js.productID == udev_device_get_sysattr_value(root, "idProduct")
          ) {
            js.manufacturer = udev_device_get_sysattr_value(root, "manufacturer");
            js.product = udev_device_get_sysattr_value(root, "product");
            js.serial = udev_device_get_sysattr_value(root, "serial");
          }
        }
      }

      unsigned axes = 0;
      unsigned hats = 0;
      unsigned buttons = 0;
      for(signed i = 0; i < ABS_MISC; i++) {
        if(testBit(js.absbit, i)) {
          if(i >= ABS_HAT0X && i <= ABS_HAT3Y) {
            if(auto hat = js.hats.insert({i, hats++})) {
              ioctl(js.fd, EVIOCGABS(i), &hat().info);
            }
          } else {
            if(auto axis = js.axes.insert({i, axes++})) {
              ioctl(js.fd, EVIOCGABS(i), &axis().info);
            }
          }
        }
      }
      for(signed i = BTN_JOYSTICK; i < KEY_MAX; i++) {
        if(testBit(js.keybit, i)) {
          js.buttons.insert({i, buttons++});
        }
      }
      for(signed i = BTN_MISC; i < BTN_JOYSTICK; i++) {
        if(testBit(js.keybit, i)) {
          js.buttons.insert({i, buttons++});
        }
      }
      js.rumble = js.effects >= 2 && testBit(js.ffbit, FF_RUMBLE);
      if(js.rumble) {
        ff_effect effect;
        memset(&effect, 0, sizeof(ff_effect));
        effect.type = FF_RUMBLE;
        effect.id = -1;
        effect.u.rumble.strong_magnitude = 65535;
        effect.u.rumble.weak_magnitude   = 65535;
        ioctl(js.fd, EVIOCSFF, &effect);
        js.effectID = effect.id;
      }

      joysticks.append(js);
    }

    #undef testBit
  }
};

}
