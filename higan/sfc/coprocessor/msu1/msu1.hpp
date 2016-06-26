struct MSU1 : Cothread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto dataOpen() -> void;
  auto audioOpen() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

private:
  vfs::shared::file dataFile;
  vfs::shared::file audioFile;

  enum Flag : uint {
    Revision       = 0x02,  //max: 0x07
    AudioError     = 0x08,
    AudioPlaying   = 0x10,
    AudioRepeating = 0x20,
    AudioBusy      = 0x40,
    DataBusy       = 0x80,
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

    bool audioError;
    bool audioPlay;
    bool audioRepeat;
    bool audioBusy;
    bool dataBusy;
  } mmio;
};

extern MSU1 msu1;
