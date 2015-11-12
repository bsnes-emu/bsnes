struct {
  //$420b
  bool dma_enabled;

  //$420c
  bool hdma_enabled;

  //$43x0
  bool direction;
  bool indirect;
  bool unused;
  bool reverse_transfer;
  bool fixed_transfer;
  uint3 transfer_mode;

  //$43x1
  uint8 dest_addr;

  //$43x2-$43x3
  uint16 source_addr;

  //$43x4
  uint8 source_bank;

  //$43x5-$43x6
  union {
    uint16 transfer_size;
    uint16 indirect_addr;
  };

  //$43x7
  uint8 indirect_bank;

  //$43x8-$43x9
  uint16 hdma_addr;

  //$43xa
  uint8 line_counter;

  //$43xb/$43xf
  uint8 unknown;

  //internal state
  bool hdma_completed;
  bool hdma_do_transfer;
} channel[8];

struct {
  bool valid;
  uint addr;
  uint8 data;
} pipe;

auto dma_add_clocks(uint clocks) -> void;
auto dma_transfer_valid(uint8 bbus, uint32 abus) -> bool;
auto dma_addr_valid(uint32 abus) -> bool;
auto dma_read(uint32 abus) -> uint8;
auto dma_write(bool valid, uint addr = 0, uint8 data = 0) -> void;
auto dma_transfer(bool direction, uint8 bbus, uint32 abus) -> void;

auto dma_bbus(uint i, uint channel) -> uint8;
auto dma_addr(uint i) -> uint32;
auto hdma_addr(uint i) -> uint32;
auto hdma_iaddr(uint i) -> uint32;

auto dma_enabled_channels() -> uint8;
auto hdma_active(uint i) -> bool;
auto hdma_active_after(uint i) -> bool;
auto hdma_enabled_channels() -> uint8;
auto hdma_active_channels() -> uint8;

auto dma_run() -> void;
auto hdma_update(uint i) -> void;
auto hdma_run() -> void;
auto hdma_init_reset() -> void;
auto hdma_init() -> void;

auto dma_power() -> void;
auto dma_reset() -> void;
