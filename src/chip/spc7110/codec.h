class SPC7110Codec {
public:
  uint8_t *buffer;
  uint8_t *output;

  void decomp_mode0(int len);
  void decomp_mode1(int len);
  void decomp_mode2(int len);

  SPC7110Codec();
  ~SPC7110Codec();

private:
  static const uint8_t EvolutionTable[53][4];
  static const uint8_t Mode2ContextTable[32][4];
  struct ContextState {
    uint8_t index;
    uint8_t invert;
  } Contexts[32];
};
