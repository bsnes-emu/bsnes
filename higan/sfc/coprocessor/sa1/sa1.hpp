struct SA1 : Processor::R65816, Thread {
  //sa1.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto tick() -> void;
  auto interrupt() -> void override;

  alwaysinline auto triggerIRQ() -> void;
  alwaysinline auto lastCycle() -> void override;
  alwaysinline auto interruptPending() const -> bool override;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //bus.cpp
  struct CPUIRAM : Memory {
    auto size() const -> uint;
    alwaysinline auto read(uint24, uint8 = 0) -> uint8;
    alwaysinline auto write(uint24, uint8) -> void;
  };

  struct CPUBWRAM : Memory {
    auto size() const -> uint;
    alwaysinline auto read(uint24, uint8 = 0) -> uint8;
    alwaysinline auto write(uint24, uint8) -> void;
    bool dma;
  };

  //dma.cpp
  struct DMA {
    enum CDEN : uint { DmaNormal = 0, DmaCharConversion = 1 };
    enum SD : uint { SourceROM = 0, SourceBWRAM = 1, SourceIRAM = 2 };
    enum DD : uint { DestIRAM = 0, DestBWRAM = 1 };
    uint line;
  };

  auto dmaNormal() -> void;
  auto dmaCC1() -> void;
  auto dmaCC1Read(uint addr) -> uint8;
  auto dmaCC2() -> void;

  //memory.cpp
  auto busRead(uint24 addr, uint8 data) -> uint8;
  auto busWrite(uint24 addr, uint8 data) -> void;
  auto vbrRead(uint24 addr, uint8 data = 0) -> uint8;

  alwaysinline auto idle() -> void override;
  alwaysinline auto read(uint24 addr) -> uint8 override;
  alwaysinline auto write(uint24 addr, uint8 data) -> void override;

  auto mmcromRead(uint24 addr, uint8 data) -> uint8;
  auto mmcromWrite(uint24 addr, uint8 data) -> void;

  auto mmcbwramRead(uint24 addr, uint8 data) -> uint8;
  auto mmcbwramWrite(uint24 addr, uint8 data) -> void;

  auto mmcSA1Read(uint addr, uint8 data) -> uint8;
  auto mmcSA1Write(uint addr, uint8 data) -> void;

  auto bitmapRead(uint addr, uint8 data) -> uint8;
  auto bitmapWrite(uint addr, uint8 data) -> void;

  //io.cpp
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM iram;
  MappedRAM bwram;

  CPUIRAM cpuiram;
  CPUBWRAM cpubwram;

private:
  DMA dma;

  struct Status {
    uint8 counter;

    bool interruptPending;

    uint16 scanlines;
    uint16 vcounter;
    uint16 hcounter;
  } status;

  struct MMIO {
    //$2200 CCNT
    bool sa1_irq;
    bool sa1_rdyb;
    bool sa1_resb;
    bool sa1_nmi;
    uint8 smeg;

    //$2201 SIE
    bool cpu_irqen;
    bool chdma_irqen;

    //$2202 SIC
    bool cpu_irqcl;
    bool chdma_irqcl;

    //$2203,$2204 CRV
    uint16 crv;

    //$2205,$2206 CNV
    uint16 cnv;

    //$2207,$2208 CIV
    uint16 civ;

    //$2209 SCNT
    bool cpu_irq;
    bool cpu_ivsw;
    bool cpu_nvsw;
    uint8 cmeg;

    //$220a CIE
    bool sa1_irqen;
    bool timer_irqen;
    bool dma_irqen;
    bool sa1_nmien;

    //$220b CIC
    bool sa1_irqcl;
    bool timer_irqcl;
    bool dma_irqcl;
    bool sa1_nmicl;

    //$220c,$220d SNV
    uint16 snv;

    //$220e,$220f SIV
    uint16 siv;

    //$2210 TMC
    bool hvselb;
    bool ven;
    bool hen;

    //$2212,$2213
    uint16 hcnt;

    //$2214,$2215
    uint16 vcnt;

    //$2220 CXB
    bool cbmode;
    uint cb;

    //$2221 DXB
    bool dbmode;
    uint db;

    //$2222 EXB
    bool ebmode;
    uint eb;

    //$2223 FXB
    bool fbmode;
    uint fb;

    //$2224 BMAPS
    uint8 sbm;

    //$2225 BMAP
    bool sw46;
    uint8 cbm;

    //$2226 SBWE
    bool swen;

    //$2227 CBWE
    bool cwen;

    //$2228 BWPA
    uint8 bwp;

    //$2229 SIWP
    uint8 siwp;

    //$222a CIWP
    uint8 ciwp;

    //$2230 DCNT
    bool dmaen;
    bool dprio;
    bool cden;
    bool cdsel;
    bool dd;
    uint8 sd;

    //$2231 CDMA
    bool chdend;
    uint8 dmasize;
    uint8 dmacb;

    //$2232-$2234 SDA
    uint32 dsa;

    //$2235-$2237 DDA
    uint32 dda;

    //$2238,$2239 DTC
    uint16 dtc;

    //$223f BBF
    bool bbf;

    //$2240-224f BRF
    uint8 brf[16];

    //$2250 MCNT
    bool acm;
    bool md;

    //$2251,$2252 MA
    uint16 ma;

    //$2253,$2254 MB
    uint16 mb;

    //$2258 VBD
    bool hl;
    uint8 vb;

    //$2259-$225b VDA
    uint32 va;
    uint8 vbit;

    //$2300 SFR
    bool cpu_irqfl;
    bool chdma_irqfl;

    //$2301 CFR
    bool sa1_irqfl;
    bool timer_irqfl;
    bool dma_irqfl;
    bool sa1_nmifl;

    //$2302,$2303 HCR
    uint16 hcr;

    //$2304,$2305 VCR
    uint16 vcr;

    //$2306-230a MR
    uint64 mr;

    //$230b OF
    bool overflow;
  } mmio;
};

extern SA1 sa1;
