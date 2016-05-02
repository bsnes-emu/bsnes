struct Decompressor {
  struct IM {  //input manager
    IM(SDD1::Decompressor& self) : self(self) {}
    auto init(uint offset) -> void;
    auto get_codeword(uint8 code_length) -> uint8;

  private:
    Decompressor& self;
    uint offset;
    uint bit_count;
  };

  struct GCD {  //golomb-code decoder
    GCD(SDD1::Decompressor& self) : self(self) {}
    auto get_run_count(uint8 code_number, uint8& mps_count, bool& lps_index) -> void;

  private:
    Decompressor& self;
    static const uint8 run_count[256];
  };

  struct BG {  //bits generator
    BG(SDD1::Decompressor& self, uint8 code_number) : self(self), code_number(code_number) {}
    auto init() -> void;
    auto get_bit(bool& end_of_run) -> uint8;

  private:
    Decompressor& self;
    const uint8 code_number;
    uint8 mps_count;
    bool lps_index;
  };

  struct PEM {  //probability estimation module
    PEM(SDD1::Decompressor& self) : self(self) {}
    auto init() -> void;
    auto get_bit(uint8 context) -> uint8;

  private:
    Decompressor& self;
    struct State {
      uint8 code_number;
      uint8 next_if_mps;
      uint8 next_if_lps;
    };
    static const State evolution_table[33];
    struct ContextInfo {
      uint8 status;
      uint8 mps;
    } context_info[32];
  };

  struct CM {  //context model
    CM(SDD1::Decompressor& self) : self(self) {}
    auto init(uint offset) -> void;
    uint8 get_bit();

  private:
    Decompressor& self;
    uint8 bitplanes_info;
    uint8 context_bits_info;
    uint8 bit_number;
    uint8 current_bitplane;
    uint16 previous_bitplane_bits[8];
  };

  struct OL {  //output logic
    OL(SDD1::Decompressor& self) : self(self) {}
    auto init(uint offset) -> void;
    auto decompress() -> uint8;

  private:
    Decompressor& self;
    uint8 bitplanes_info;
    uint8 r0, r1, r2;
  };

  Decompressor();
  auto init(uint offset) -> void;
  auto read() -> uint8;

  IM  im;
  GCD gcd;
  BG  bg0, bg1, bg2, bg3, bg4, bg5, bg6, bg7;
  PEM pem;
  CM  cm;
  OL  ol;
};
