struct Bridge {
  struct Buffer {
    bool ready;
    uint8 data;
  };
  Buffer cputoarm;
  Buffer armtocpu;
  uint32 timer;
  uint32 timerlatch;
  bool reset;
  bool ready;
  bool signal;

  auto status() const -> uint8 {
    return (ready << 7) | (cputoarm.ready << 3) | (signal << 2) | (armtocpu.ready << 0);
  }
} bridge;
