class PPUDebugger : public PPU, public ChipDebugger {
public:
  bool property(unsigned id, string &name, string &value);

  bool bg1_enabled[2];
  bool bg2_enabled[2];
  bool bg3_enabled[2];
  bool bg4_enabled[2];
  bool oam_enabled[4];
};
