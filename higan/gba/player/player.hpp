struct Player : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;
  auto frame() -> void;

  auto keyinput() -> maybe<uint16>;
  auto read() -> maybe<uint32>;
  auto write(uint2 addr, uint8 byte) -> void;

  auto serialize(serializer& s) -> void;

private:
  struct Status {
    bool enable;
    bool rumble;

    bool logoDetected;
    uint logoCounter;

    uint packet;
    uint32 send;
    uint32 recv;

    uint timeout;
  } status;
};

extern Player player;
