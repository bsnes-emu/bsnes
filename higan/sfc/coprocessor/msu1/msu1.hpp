struct MSU1 : Cothread {
  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto dataOpen() -> void;
  auto audioOpen() -> void;

  auto mmioRead(uint24 addr, uint8 data) -> uint8;
  auto mmioWrite(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

private:
  file dataFile;
  file audioFile;

  enum Flag : uint {
    DataBusy       = 0x80,
    AudioBusy      = 0x40,
    AudioRepeating = 0x20,
    AudioPlaying   = 0x10,
    AudioError     = 0x08,
    Revision       = 0x02,
  };

  struct MMIO {
    uint32 dataSeekOffset;
    uint32 dataReadOffset;

    uint32 audioPlayOffset;
    uint32 audioLoopOffset;

    uint16 audioTrack;
    uint8 audioVolume;

    uint32 audioResumeTrack;
    uint32 audioResumeOffset;

    bool dataBusy;
    bool audioBusy;
    bool audioRepeat;
    bool audioPlay;
    bool audioError;
  } mmio;
};

extern MSU1 msu1;
