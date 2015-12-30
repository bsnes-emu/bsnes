auto get_vram_address() -> uint16;

auto vram_mmio_read(uint16 addr) -> uint8;
auto vram_mmio_write(uint16 addr, uint8 data) -> void;

auto oam_mmio_read(uint16 addr) -> uint8;
auto oam_mmio_write(uint16 addr, uint8 data) -> void;

auto cgram_mmio_read(uint16 addr) -> uint8;
auto cgram_mmio_write(uint16 addr, uint8 data) -> void;
