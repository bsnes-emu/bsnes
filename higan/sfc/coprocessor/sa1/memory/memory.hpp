auto bus_read(uint24 addr, uint8 data) -> uint8;
auto bus_write(uint24 addr, uint8 data) -> void;
auto vbr_read(uint24 addr, uint8 data = 0) -> uint8;

alwaysinline auto io() -> void override;
alwaysinline auto read(uint24 addr) -> uint8 override;
alwaysinline auto write(uint24 addr, uint8 data) -> void override;

auto mmcrom_read(uint24 addr, uint8 data) -> uint8;
auto mmcrom_write(uint24 addr, uint8 data) -> void;

auto mmcbwram_read(uint24 addr, uint8 data) -> uint8;
auto mmcbwram_write(uint24 addr, uint8 data) -> void;

auto mmc_sa1_read(uint addr, uint8 data) -> uint8;
auto mmc_sa1_write(uint addr, uint8 data) -> void;

auto bitmap_read(uint addr, uint8 data) -> uint8;
auto bitmap_write(uint addr, uint8 data) -> void;
