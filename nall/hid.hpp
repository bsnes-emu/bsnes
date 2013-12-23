#ifndef NALL_HID_HPP
#define NALL_HID_HPP

namespace nall {

namespace HID {
  struct Input {
    string name;
    int16_t value = 0;

    Input() {}
    Input(const string& name) : name(name) {}
  };

  struct Group {
    string name;
    vector<Input> input;

    Group() {}
    Group(const string& name) : name(name) {}

    void append(const string& name) {
      input.append({name});
    }

    optional<unsigned> find(const string& name) {
      for(unsigned id = 0; id < input.size(); id++) {
        if(input[id].name == name) return {true, id};
      }
      return false;
    }
  };

  struct Device {
    uint64_t id = 0;
    string name;
    vector<Group> group;

    Device() {
      group.resize(4);
    }

    uint32_t pathID() const { return (uint32_t)(id >> 32); }
    uint32_t deviceID() const { return (uint32_t)(id >> 0); }
    uint16_t vendorID() const { return (uint16_t)(id >> 16); }
    uint16_t productID() const { return (uint16_t)(id >> 0); }

    virtual bool isNull() const { return false; }
    virtual bool isKeyboard() const { return false; }
    virtual bool isMouse() const { return false; }
    virtual bool isJoypad() const { return false; }

    optional<unsigned> find(const string& name) {
      for(unsigned id = 0; id < group.size(); id++) {
        if(group[id].name == name) return {true, id};
      }
      return false;
    }
  };

  struct Null : Device {
    Null() {
      name = "Null";
    }

    bool isNull() const { return true; }
  };

  struct Keyboard : Device {
    enum GroupID : unsigned { Button };
    Group& button = group[0];

    Keyboard() {
      name = "Keyboard";
      button.name = "Button";
    }

    bool isKeyboard() const { return true; }
  };

  struct Mouse : Device {
    enum GroupID : unsigned { Axis, Button };
    Group& axis = group[0];
    Group& button = group[1];

    Mouse() {
      name = "Mouse";
      axis.name = "Axis";
      button.name = "Button";
    }

    bool isMouse() const { return true; }
  };

  struct Joypad : Device {
    enum GroupID : unsigned { Axis, Hat, Trigger, Button };
    Group& axis = group[0];
    Group& hat = group[1];
    Group& trigger = group[2];
    Group& button = group[3];
    bool rumble = false;

    Joypad() {
      name = "Joypad";
      axis.name = "Axis";
      hat.name = "Hat";
      trigger.name = "Trigger";
      button.name = "Button";
    }

    bool isJoypad() const { return true; }
  };
}

}

#endif
