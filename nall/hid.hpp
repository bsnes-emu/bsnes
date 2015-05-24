#ifndef NALL_HID_HPP
#define NALL_HID_HPP

namespace nall { namespace HID {

struct Input {
  Input(const string& name) : _name(name) {}

  auto name() const -> string { return _name; }
  auto value() const -> int16_t { return _value; }
  auto setValue(int16_t value) -> void { _value = value; }

private:
  string _name;
  int16_t _value = 0;
  friend class Group;
};

struct Group : vector<Input> {
  Group(const string& name) : _name(name) {}

  auto name() const -> string { return _name; }
  auto input(unsigned id) -> Input& { return operator[](id); }
  auto append(const string& name) -> void { vector::append({name}); }

  auto find(const string& name) const -> maybe<unsigned> {
    for(auto id : range(size())) {
      if(operator[](id)._name == name) return id;
    }
    return nothing;
  }

private:
  string _name;
  friend class Device;
};

struct Device : vector<Group> {
  Device(const string& name) : _name(name) {}

  auto pathID() const -> uint32_t { return (uint32_t)(_id >> 32); }
  auto deviceID() const -> uint32_t { return (uint32_t)(_id >> 0); }
  auto vendorID() const -> uint16_t  { return (uint16_t)(_id >> 16); }
  auto productID() const -> uint16_t { return (uint16_t)(_id >> 0); }

  virtual auto isNull() const -> bool { return false; }
  virtual auto isKeyboard() const -> bool { return false; }
  virtual auto isMouse() const -> bool { return false; }
  virtual auto isJoypad() const -> bool { return false; }

  auto name() const -> string { return _name; }
  auto id() const -> uint64_t { return _id; }
  auto setID(uint64_t id) -> void { _id = id; }
  auto group(unsigned id) -> Group& { return operator[](id); }
  auto append(const string& name) -> void { vector::append({name}); }

  auto find(const string& name) const -> maybe<unsigned> {
    for(auto id : range(size())) {
      if(operator[](id)._name == name) return id;
    }
    return nothing;
  }

private:
  string _name;
  uint64_t _id = 0;
};

struct Null : Device {
  Null() : Device("Null") {}
  auto isNull() const -> bool { return true; }
};

struct Keyboard : Device {
  enum GroupID : unsigned { Button };

  Keyboard() : Device("Keyboard") { append("Button"); }
  auto isKeyboard() const -> bool { return true; }
  auto buttons() -> Group& { return group(GroupID::Button); }
};

struct Mouse : Device {
  enum GroupID : unsigned { Axis, Button };

  Mouse() : Device("Mouse") { append("Axis"), append("Button"); }
  auto isMouse() const -> bool { return true; }
  auto axes() -> Group& { return group(GroupID::Axis); }
  auto buttons() -> Group& { return group(GroupID::Button); }
};

struct Joypad : Device {
  enum GroupID : unsigned { Axis, Hat, Trigger, Button };

  Joypad() : Device("Joypad") { append("Axis"), append("Hat"), append("Trigger"), append("Button"); }
  auto isJoypad() const -> bool { return true; }
  auto axes() -> Group& { return group(GroupID::Axis); }
  auto hats() -> Group& { return group(GroupID::Hat); }
  auto triggers() -> Group& { return group(GroupID::Trigger); }
  auto buttons() -> Group& { return group(GroupID::Button); }

  auto rumble() const -> bool { return _rumble; }
  auto setRumble(bool rumble) -> void { _rumble = rumble; }

private:
  bool _rumble = false;
};

}}

#endif
