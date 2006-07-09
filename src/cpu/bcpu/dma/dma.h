struct {
  uint32 read_index; //set to 0 at beginning of DMA/HDMA

//$420b
  bool   dma_enabled;
//$420c
  bool   hdma_enabled;
//$43x0
  uint8  dmap;
  bool   direction;
  bool   hdma_indirect;
  int8   incmode;
  bool   fixedxfer;
  uint8  xfermode;
//$43x1
  uint8  destaddr;
//$43x2-$43x3
  uint16 srcaddr;
//$43x4
  uint8  srcbank;
//$43x5-$43x6
  union {
    uint16 xfersize;
    uint16 hdma_iaddr;
  };
//$43x7
  uint8  hdma_ibank;
//$43x8-$43x9
  uint16 hdma_addr;
//$43xa
  uint8  hdma_line_counter;
//$43xb/$43xf
  uint8  hdma_unknown;

//hdma-specific
  bool   hdma_completed; //for this frame
  bool   hdma_do_transfer;

  uint8  hdma_current_channel;
  uint8  hdma_current_pos;
} channel[8];

  inline void   dma_transfer_byte(bool direction, uint8 bbus, uint32 abus);
  inline uint8  dma_bbus(uint8 i, uint8 index);
  inline void   dma_add_cycles(uint32 cycles);
  inline void   hdma_add_cycles(uint32 cycles);
  inline void   dma_run();
  inline void   hdma_run();
  inline void   hdma_init();
  inline void   hdma_update(uint8 i);
  inline uint8  hdma_enabled_channels();
  inline uint8  hdma_active_channels();
  inline void   hdmainit_activate();
  inline void   hdma_activate();
  inline void   dma_cputommio(uint8 i, uint8 index);
  inline void   dma_mmiotocpu(uint8 i, uint8 index);
  inline void   dma_write(uint8 i, uint8 index);
  inline uint32 dma_addr(uint8 i);
  inline uint32 hdma_addr(uint8 i);
  inline uint32 hdma_iaddr(uint8 i);
  inline void   dma_reset();
