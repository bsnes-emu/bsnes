struct BSMemory : Memory {
  auto queryable(bool queryable) { pin.queryable = !ROM && queryable; }
  auto flashable(bool flashable) { pin.flashable = !ROM && flashable; }

  //bsmemory.cpp
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto data() -> uint8* override;
  auto size() const -> uint override;
  auto read(uint24 address, uint8 data) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint pathID = 0;

  //0 = Flash; 1 = MaskROM
  uint ROM = 1;

  //valid types are 1,2,3,4
  //type 2 is currently unsupported
  //type 1 is the only type to exist (all flash-based BS Memory Cassettes are type 1)
  uint Type = 1;

  ProtectableMemory memory;

private:
  struct Pin {
    uint1 queryable;
    uint1 flashable;
  } pin;

  struct IO {
    uint8 mode;
  } io;
};

extern BSMemory bsmemory;
