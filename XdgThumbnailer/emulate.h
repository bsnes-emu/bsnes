#pragma once

#include <stddef.h>
#include <stdint.h>

enum FileKind {
    KIND_GB,
    KIND_GBC,
    KIND_ISX,
};

void load_boot_rom(void);
void unload_boot_rom(void);

unsigned emulate(enum FileKind kind, unsigned char const *rom, size_t rom_size, uint32_t screen[static 160 * 144]);
