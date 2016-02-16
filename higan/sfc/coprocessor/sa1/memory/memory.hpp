auto bus_read(uint addr, uint8 data) -> uint8;
auto bus_write(uint addr, uint8 data) -> void;
auto vbr_read(uint addr, uint8 data = 0) -> uint8;

alwaysinline auto op_io() -> void;
alwaysinline auto op_read(uint32 addr) -> uint8;
alwaysinline auto op_write(uint32 addr, uint8 data) -> void;

auto mmcrom_read(uint addr, uint8 data) -> uint8;
auto mmcrom_write(uint addr, uint8 data) -> void;

auto mmcbwram_read(uint addr, uint8 data) -> uint8;
auto mmcbwram_write(uint addr, uint8 data) -> void;

auto mmc_sa1_read(uint addr, uint8 data) -> uint8;
auto mmc_sa1_write(uint addr, uint8 data) -> void;

auto bitmap_read(uint addr, uint8 data) -> uint8;
auto bitmap_write(uint addr, uint8 data) -> void;
