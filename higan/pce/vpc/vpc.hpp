//Hudson Soft HuC6202 -- Video Priority Controller

struct VPC {
  auto bus(uint hclock) -> uint9;

  auto power() -> void;
  auto read(uint5 addr) -> uint8;
  auto write(uint5 addr, uint8 data) -> void;
  auto store(uint2 addr, uint8 data) -> void;

private:
  struct Settings {
    bool  enableVDC0;
    bool  enableVDC1;
    uint2 priority;
  } settings[4];

  uint10 window[2];
  bool select;
};

extern VPC vpc;
