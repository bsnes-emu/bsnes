struct MSU1 : Coprocessor {
  static void Enter();
  void enter();
  void init();
  void load();
  void unload();
  void power();
  void reset();

  void data_open();
  void audio_open();

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  void serialize(serializer&);

private:
  bool boot;
  file datafile;
  file audiofile;

  enum Flag {
    DataBusy       = 0x80,
    AudioBusy      = 0x40,
    AudioRepeating = 0x20,
    AudioPlaying   = 0x10,
    AudioError     = 0x08,
    Revision       = 0x01,
  };

  struct MMIO {
    uint32 data_offset;
    uint32 audio_offset;
    uint32 audio_loop_offset;

    uint16 audio_track;
    uint8 audio_volume;

    bool data_busy;
    bool audio_busy;
    bool audio_repeat;
    bool audio_play;
    bool audio_error;
  } mmio;
};

extern MSU1 msu1;
