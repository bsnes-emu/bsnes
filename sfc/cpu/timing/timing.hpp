//timing.cpp
auto dma_counter() -> uint;

auto add_clocks(uint clocks) -> void;
auto scanline() -> void;

alwaysinline auto alu_edge() -> void;
alwaysinline auto dma_edge() -> void;
alwaysinline auto last_cycle() -> void;

auto timing_power() -> void;
auto timing_reset() -> void;

//irq.cpp
alwaysinline auto poll_interrupts() -> void;
auto nmitimen_update(uint8 data) -> void;
auto rdnmi() -> bool;
auto timeup() -> bool;

alwaysinline auto nmi_test() -> bool;
alwaysinline auto irq_test() -> bool;

//joypad.cpp
auto step_auto_joypad_poll() -> void;
