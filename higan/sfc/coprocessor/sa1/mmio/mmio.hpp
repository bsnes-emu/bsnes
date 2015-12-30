auto mmio_read(uint addr, uint8 data) -> uint8;
auto mmio_write(uint addr, uint8 data) -> void;

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

auto mmio_w2200(uint8) -> void;  //CCNT
auto mmio_w2201(uint8) -> void;  //SIE
auto mmio_w2202(uint8) -> void;  //SIC
auto mmio_w2203(uint8) -> void;  //CRVL
auto mmio_w2204(uint8) -> void;  //CRVH
auto mmio_w2205(uint8) -> void;  //CNVL
auto mmio_w2206(uint8) -> void;  //CNVH
auto mmio_w2207(uint8) -> void;  //CIVL
auto mmio_w2208(uint8) -> void;  //CIVH
auto mmio_w2209(uint8) -> void;  //SCNT
auto mmio_w220a(uint8) -> void;  //CIE
auto mmio_w220b(uint8) -> void;  //CIC
auto mmio_w220c(uint8) -> void;  //SNVL
auto mmio_w220d(uint8) -> void;  //SNVH
auto mmio_w220e(uint8) -> void;  //SIVL
auto mmio_w220f(uint8) -> void;  //SIVH
auto mmio_w2210(uint8) -> void;  //TMC
auto mmio_w2211(uint8) -> void;  //CTR
auto mmio_w2212(uint8) -> void;  //HCNTL
auto mmio_w2213(uint8) -> void;  //HCNTH
auto mmio_w2214(uint8) -> void;  //VCNTL
auto mmio_w2215(uint8) -> void;  //VCNTH
auto mmio_w2220(uint8) -> void;  //CXB
auto mmio_w2221(uint8) -> void;  //DXB
auto mmio_w2222(uint8) -> void;  //EXB
auto mmio_w2223(uint8) -> void;  //FXB
auto mmio_w2224(uint8) -> void;  //BMAPS
auto mmio_w2225(uint8) -> void;  //BMAP
auto mmio_w2226(uint8) -> void;  //SBWE
auto mmio_w2227(uint8) -> void;  //CBWE
auto mmio_w2228(uint8) -> void;  //BWPA
auto mmio_w2229(uint8) -> void;  //SIWP
auto mmio_w222a(uint8) -> void;  //CIWP
auto mmio_w2230(uint8) -> void;  //DCNT
auto mmio_w2231(uint8) -> void;  //CDMA
auto mmio_w2232(uint8) -> void;  //SDAL
auto mmio_w2233(uint8) -> void;  //SDAH
auto mmio_w2234(uint8) -> void;  //SDAB
auto mmio_w2235(uint8) -> void;  //DDAL
auto mmio_w2236(uint8) -> void;  //DDAH
auto mmio_w2237(uint8) -> void;  //DDAB
auto mmio_w2238(uint8) -> void;  //DTCL
auto mmio_w2239(uint8) -> void;  //DTCH
auto mmio_w223f(uint8) -> void;  //BBF
auto mmio_w2240(uint8) -> void;  //BRF0
auto mmio_w2241(uint8) -> void;  //BRF1
auto mmio_w2242(uint8) -> void;  //BRF2
auto mmio_w2243(uint8) -> void;  //BRF3
auto mmio_w2244(uint8) -> void;  //BRF4
auto mmio_w2245(uint8) -> void;  //BRF5
auto mmio_w2246(uint8) -> void;  //BRF6
auto mmio_w2247(uint8) -> void;  //BRF7
auto mmio_w2248(uint8) -> void;  //BRF8
auto mmio_w2249(uint8) -> void;  //BRF9
auto mmio_w224a(uint8) -> void;  //BRFA
auto mmio_w224b(uint8) -> void;  //BRFB
auto mmio_w224c(uint8) -> void;  //BRFC
auto mmio_w224d(uint8) -> void;  //BRFD
auto mmio_w224e(uint8) -> void;  //BRFE
auto mmio_w224f(uint8) -> void;  //BRFF
auto mmio_w2250(uint8) -> void;  //MCNT
auto mmio_w2251(uint8) -> void;  //MAL
auto mmio_w2252(uint8) -> void;  //MAH
auto mmio_w2253(uint8) -> void;  //MBL
auto mmio_w2254(uint8) -> void;  //MBH
auto mmio_w2258(uint8) -> void;  //VBD
auto mmio_w2259(uint8) -> void;  //VDAL
auto mmio_w225a(uint8) -> void;  //VDAH
auto mmio_w225b(uint8) -> void;  //VDAB

auto mmio_r2300() -> uint8;  //SFR
auto mmio_r2301() -> uint8;  //CFR
auto mmio_r2302() -> uint8;  //HCRL
auto mmio_r2303() -> uint8;  //HCRH
auto mmio_r2304() -> uint8;  //VCRL
auto mmio_r2305() -> uint8;  //VCRH
auto mmio_r2306() -> uint8;  //MR [00-07]
auto mmio_r2307() -> uint8;  //MR [08-15]
auto mmio_r2308() -> uint8;  //MR [16-23]
auto mmio_r2309() -> uint8;  //MR [24-31]
auto mmio_r230a() -> uint8;  //MR [32-40]
auto mmio_r230b() -> uint8;  //OF
auto mmio_r230c() -> uint8;  //VDPL
auto mmio_r230d() -> uint8;  //VDPH
auto mmio_r230e() -> uint8;  //VC
