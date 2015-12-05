struct Input {
  enum class Device : uint {
    Joypad,
    None,
  };

  auto latch(bool data) -> void;
  auto data(bool port) -> bool;
  auto connect(bool port, Device device) -> void;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

private:
  Device port1;
  Device port2;

  bool latchdata;
  uint counter1;
  uint counter2;
};

extern Input input;
