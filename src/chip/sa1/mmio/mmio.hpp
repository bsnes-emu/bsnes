uint8_t mmio_read(unsigned addr);
void mmio_write(unsigned addr, uint8_t data);

struct MMIO {
  //$2200 CCNT
  bool sa1_irq;
  bool sa1_rdyb;
  bool sa1_resb;
  bool sa1_nmi;
  uint8_t smeg;

  //$2201 SIE
  bool cpu_irqen;
  bool chdma_irqen;

  //$2202 SIC
  bool cpu_irqcl;
  bool chdma_irqcl;

  //$2203,$2204 CRV
  uint16_t crv;

  //$2205,$2206 CNV
  uint16_t cnv;

  //$2207,$2208 CIV
  uint16_t civ;

  //$2209 SCNT
  bool cpu_irq;
  bool cpu_ivsw;
  bool cpu_nvsw;
  uint8_t cmeg;

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
  uint16_t snv;

  //$220e,$220f SIV
  uint16_t siv;

  //$2210 TMC
  bool hvselb;
  bool ven;
  bool hen;

  //$2212,$2213
  uint16_t hcnt;

  //$2214,$2215
  uint16_t vcnt;

  //$2220 CXB
  bool cbmode;
  uint8_t cb;

  //$2221 DXB
  bool dbmode;
  uint8_t db;

  //$2222 EXB
  bool ebmode;
  uint8_t eb;

  //$2223 FXB
  bool fbmode;
  uint8_t fb;

  //$2224 BMAPS
  uint8_t sbm;

  //$2225 BMAP
  bool sw46;
  uint8_t cbm;

  //$2226 SBWE
  bool swen;

  //$2227 CBWE
  bool cwen;

  //$2228 BWPA
  uint8_t bwp;

  //$2229 SIWP
  uint8_t siwp;

  //$222a CIWP
  uint8_t ciwp;

  //$2230 DCNT
  bool dmaen;
  bool dprio;
  bool cden;
  bool cdsel;
  bool dd;
  uint8_t sd;

  //$2231 CDMA
  bool chdend;
  uint8_t dmasize;
  uint8_t dmacb;

  //$2232-$2234 SDA
  uint32_t dsa;

  //$2235-$2237 DDA
  uint32_t dda;

  //$2238,$2239 DTC
  uint16_t dtc;

  //$223f BBF
  bool bbf;

  //$2240-224f BRF
  uint8_t brf[16];

  //$2250 MCNT
  bool acm;
  bool md;

  //$2251,$2252 MA
  uint16_t ma;

  //$2253,$2254 MB
  uint16_t mb;

  //$2258 VBD
  bool hl;
  uint8_t vb;

  //$2259-$225b VDA
  uint32_t va;
  uint8_t vbit;

  //$2300 SFR
  bool cpu_irqfl;
  bool chdma_irqfl;

  //$2301 CFR
  bool sa1_irqfl;
  bool timer_irqfl;
  bool dma_irqfl;
  bool sa1_nmifl;

  //$2302,$2303 HCR
  uint16_t hcr;

  //$2304,$2305 VCR
  uint16_t vcr;

  //$2306-230a MR
  uint64_t mr;

  //$230b OF
  bool overflow;
} mmio;

void mmio_w2200(uint8_t);  //CCNT
void mmio_w2201(uint8_t);  //SIE
void mmio_w2202(uint8_t);  //SIC
void mmio_w2203(uint8_t);  //CRVL
void mmio_w2204(uint8_t);  //CRVH
void mmio_w2205(uint8_t);  //CNVL
void mmio_w2206(uint8_t);  //CNVH
void mmio_w2207(uint8_t);  //CIVL
void mmio_w2208(uint8_t);  //CIVH
void mmio_w2209(uint8_t);  //SCNT
void mmio_w220a(uint8_t);  //CIE
void mmio_w220b(uint8_t);  //CIC
void mmio_w220c(uint8_t);  //SNVL
void mmio_w220d(uint8_t);  //SNVH
void mmio_w220e(uint8_t);  //SIVL
void mmio_w220f(uint8_t);  //SIVH
void mmio_w2210(uint8_t);  //TMC
void mmio_w2211(uint8_t);  //CTR
void mmio_w2212(uint8_t);  //HCNTL
void mmio_w2213(uint8_t);  //HCNTH
void mmio_w2214(uint8_t);  //VCNTL
void mmio_w2215(uint8_t);  //VCNTH
void mmio_w2220(uint8_t);  //CXB
void mmio_w2221(uint8_t);  //DXB
void mmio_w2222(uint8_t);  //EXB
void mmio_w2223(uint8_t);  //FXB
void mmio_w2224(uint8_t);  //BMAPS
void mmio_w2225(uint8_t);  //BMAP
void mmio_w2226(uint8_t);  //SBWE
void mmio_w2227(uint8_t);  //CBWE
void mmio_w2228(uint8_t);  //BWPA
void mmio_w2229(uint8_t);  //SIWP
void mmio_w222a(uint8_t);  //CIWP
void mmio_w2230(uint8_t);  //DCNT
void mmio_w2231(uint8_t);  //CDMA
void mmio_w2232(uint8_t);  //SDAL
void mmio_w2233(uint8_t);  //SDAH
void mmio_w2234(uint8_t);  //SDAB
void mmio_w2235(uint8_t);  //DDAL
void mmio_w2236(uint8_t);  //DDAH
void mmio_w2237(uint8_t);  //DDAB
void mmio_w2238(uint8_t);  //DTCL
void mmio_w2239(uint8_t);  //DTCH
void mmio_w223f(uint8_t);  //BBF
void mmio_w2240(uint8_t);  //BRF0
void mmio_w2241(uint8_t);  //BRF1
void mmio_w2242(uint8_t);  //BRF2
void mmio_w2243(uint8_t);  //BRF3
void mmio_w2244(uint8_t);  //BRF4
void mmio_w2245(uint8_t);  //BRF5
void mmio_w2246(uint8_t);  //BRF6
void mmio_w2247(uint8_t);  //BRF7
void mmio_w2248(uint8_t);  //BRF8
void mmio_w2249(uint8_t);  //BRF9
void mmio_w224a(uint8_t);  //BRFA
void mmio_w224b(uint8_t);  //BRFB
void mmio_w224c(uint8_t);  //BRFC
void mmio_w224d(uint8_t);  //BRFD
void mmio_w224e(uint8_t);  //BRFE
void mmio_w224f(uint8_t);  //BRFF
void mmio_w2250(uint8_t);  //MCNT
void mmio_w2251(uint8_t);  //MAL
void mmio_w2252(uint8_t);  //MAH
void mmio_w2253(uint8_t);  //MBL
void mmio_w2254(uint8_t);  //MBH
void mmio_w2258(uint8_t);  //VBD
void mmio_w2259(uint8_t);  //VDAL
void mmio_w225a(uint8_t);  //VDAH
void mmio_w225b(uint8_t);  //VDAB

uint8_t mmio_r2300();  //SFR
uint8_t mmio_r2301();  //CFR
uint8_t mmio_r2302();  //HCRL
uint8_t mmio_r2303();  //HCRH
uint8_t mmio_r2304();  //VCRL
uint8_t mmio_r2305();  //VCRH
uint8_t mmio_r2306();  //MR [00-07]
uint8_t mmio_r2307();  //MR [08-15]
uint8_t mmio_r2308();  //MR [16-23]
uint8_t mmio_r2309();  //MR [24-31]
uint8_t mmio_r230a();  //MR [32-40]
uint8_t mmio_r230b();  //OF
uint8_t mmio_r230c();  //VDPL
uint8_t mmio_r230d();  //VDPH
uint8_t mmio_r230e();  //VC
