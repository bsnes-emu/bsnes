class sPPUDebugger : public sPPU, public PPUDebugger {
public:
  bool bg1_enabled[2];
  bool bg2_enabled[2];
  bool bg3_enabled[2];
  bool bg4_enabled[2];
  bool oam_enabled[4];

  bool interlace();
  bool overscan();
};
