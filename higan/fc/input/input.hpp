struct Input {
  enum class Device : unsigned {
    Joypad,
    None,
  };

  void latch(bool data);
  bool data(bool port);
  void connect(bool port, Device device);

  void power();
  void reset();

  void serialize(serializer&);

private:
  Device port1;
  Device port2;

  bool latchdata;
  unsigned counter1;
  unsigned counter2;
};

extern Input input;
