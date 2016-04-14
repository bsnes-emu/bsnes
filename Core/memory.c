#include <stdio.h>
#include <stdbool.h>
#include "gb.h"
#include "joypad.h"
#include "display.h"
#include "memory.h"

typedef unsigned char GB_read_function_t(GB_gameboy_t *gb, unsigned short addr);
typedef void GB_write_function_t(GB_gameboy_t *gb, unsigned short addr, unsigned char value);

static unsigned char read_rom(GB_gameboy_t *gb, unsigned short addr)
{
    if (addr < 0x100 && !gb->bios_finished) {
        return gb->bios[addr];
    }

    if (addr >= 0x200 && addr < 0x900 && gb->is_cgb && !gb->bios_finished) {
        return gb->bios[addr];
    }

    if (!gb->rom_size) {
        return 0xFF;
    }
    return gb->rom[addr];
}

static unsigned char read_mbc_rom(GB_gameboy_t *gb, unsigned short addr)
{
    if (gb->mbc_rom_bank >= gb->rom_size / 0x4000) {
        return 0xFF;
    }
    return gb->rom[(addr & 0x3FFF) + gb->mbc_rom_bank * 0x4000];
}

static unsigned char read_vram(GB_gameboy_t *gb, unsigned short addr)
{
    if ((gb->io_registers[GB_IO_STAT] & 0x3) == 3) {
        return 0xFF;
    }
    return gb->vram[(addr & 0x1FFF) + (unsigned short) gb->cgb_vram_bank * 0x2000];
}

static unsigned char read_mbc_ram(GB_gameboy_t *gb, unsigned short addr)
{
    if (gb->cartridge_type->has_rtc && gb->mbc_ram_bank >= 8 && gb->mbc_ram_bank <= 0xC) {
        /* RTC read */
        gb->rtc_high |= ~0xC1; /* Not all bytes in RTC high are used. */
        return gb->rtc_data[gb->mbc_ram_bank - 8];
    }
    unsigned int ram_index = (addr & 0x1FFF) + gb->mbc_ram_bank * 0x2000;
    if (!gb->mbc_ram_enable)
    {
        gb_log(gb, "Read from %02x:%04x (%06x) (Disabled MBC RAM)\n", gb->mbc_ram_bank, addr, ram_index);
        return 0xFF;
    }
    if (ram_index >= gb->mbc_ram_size) {
        gb_log(gb, "Read from %02x:%04x (%06x) (Unmapped MBC RAM)\n", gb->mbc_ram_bank, addr, ram_index);
        return 0xFF;
    }
    return gb->mbc_ram[(addr & 0x1FFF) + gb->mbc_ram_bank * 0x2000];
}

static unsigned char read_ram(GB_gameboy_t *gb, unsigned short addr)
{
    return gb->ram[addr & 0x0FFF];
}

static unsigned char read_banked_ram(GB_gameboy_t *gb, unsigned short addr)
{
    return gb->ram[(addr & 0x0FFF) + gb->cgb_ram_bank * 0x1000];
}

static unsigned char read_high_memory(GB_gameboy_t *gb, unsigned short addr)
{

    if (addr < 0xFE00) {
        return gb->ram[addr & 0x0FFF];
    }

    if (addr < 0xFEA0) {
        if ((gb->io_registers[GB_IO_STAT] & 0x3) >= 2) {
            return 0xFF;
        }
        return gb->oam[addr & 0xFF];
    }

    if (addr < 0xFF00) {
        /* Unusable, simulate Gameboy Color */
        if ((gb->io_registers[GB_IO_STAT] & 0x3) >= 2) { /* Seems to be disabled in Modes 2 and 3 */
            return 0xFF;
        }
        return (addr & 0xF0) | ((addr >> 4) & 0xF);
    }

    if (addr < 0xFF80) {
        switch (addr & 0xFF) {
            case GB_IO_IF:
                return gb->io_registers[GB_IO_IF] | 0xE0;
            case GB_IO_TAC:
                return gb->io_registers[GB_IO_TAC] | 0xF8;
            case GB_IO_STAT:
                return gb->io_registers[GB_IO_STAT] | 0x80;
            case GB_IO_DMG_EMULATION_INDICATION:
                return gb->io_registers[GB_IO_DMG_EMULATION_INDICATION] | 0xFE;
            case GB_IO_JOYP:
            case GB_IO_DIV:
            case GB_IO_TIMA:
            case GB_IO_TMA:
            case GB_IO_LCDC:
            case GB_IO_SCY:
            case GB_IO_SCX:
            case GB_IO_LY:
            case GB_IO_LYC:
            case GB_IO_BGP:
            case GB_IO_OBP0:
            case GB_IO_OBP1:
            case GB_IO_WY:
            case GB_IO_WX:
            case GB_IO_HDMA1:
            case GB_IO_HDMA2:
            case GB_IO_HDMA3:
            case GB_IO_HDMA4:
            case GB_IO_PCM_12:
            case GB_IO_PCM_34:
            case GB_IO_SB:
                return gb->io_registers[addr & 0xFF];
            case GB_IO_HDMA5:
                return (gb->io_registers[GB_IO_HDMA5] & 0x80) | ((gb->hdma_steps_left - 1) & 0x7F);
            case GB_IO_SVBK:
                if (!gb->cgb_mode) {
                    return 0xFF;
                }
                return gb->cgb_ram_bank | ~0x7;
            case GB_IO_VBK:
                if (!gb->cgb_mode) {
                    return 0xFF;
                }
                return gb->cgb_vram_bank | ~0x1;

            case GB_IO_BGPI:
            case GB_IO_OBPI:
                if (!gb->is_cgb) {
                    return 0xFF;
                }
                return gb->io_registers[addr & 0xFF] | 0x40;

            case GB_IO_BGPD:
            case GB_IO_OBPD:
            {
                if (!gb->is_cgb) {
                    return 0xFF;
                }
                unsigned char index_reg = (addr & 0xFF) - 1;
                return ((addr & 0xFF) == GB_IO_BGPD?
                       gb->background_palletes_data :
                       gb->sprite_palletes_data)[gb->io_registers[index_reg] & 0x3F];
            }

            case GB_IO_KEY1:
                if (!gb->cgb_mode) {
                    return 0xFF;
                }
                return (gb->io_registers[GB_IO_KEY1] & 0x7F) | (gb->cgb_double_speed? 0xFE : 0x7E);


            default:
                if ((addr & 0xFF) >= GB_IO_NR10 && (addr & 0xFF) <= GB_IO_WAV_END) {
                    return apu_read(gb, addr & 0xFF);
                }
                return 0xFF;
        }
        /* Hardware registers */
        return 0;
    }

    if (addr == 0xFFFF) {
        /* Interrupt Mask */
        return gb->interrupt_enable;
    }

    /* HRAM */
    return gb->hram[addr - 0xFF80];
}

static GB_read_function_t * const read_map[] =
{
    read_rom,         read_rom,         read_rom, read_rom,         /* 0XXX, 1XXX, 2XXX, 3XXX */
    read_mbc_rom,     read_mbc_rom,     read_mbc_rom, read_mbc_rom, /* 4XXX, 5XXX, 6XXX, 7XXX */
    read_vram,        read_vram,                                    /* 8XXX, 9XXX */
    read_mbc_ram,     read_mbc_ram,                                 /* AXXX, BXXX */
    read_ram,         read_banked_ram,                              /* CXXX, DXXX */
    read_high_memory, read_high_memory,                             /* EXXX FXXX */
};

unsigned char read_memory(GB_gameboy_t *gb, unsigned short addr)
{
    if (addr < 0xFF00 && gb->dma_cycles) {
        /* Todo: can we access IO registers during DMA? */
        return 0xFF;
    }
    return read_map[addr >> 12](gb, addr);
}

static void write_mbc(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    if (gb->cartridge_type->mbc_type == NO_MBC) return;
    switch (addr >> 12) {
        case 0:
        case 1:
            gb->mbc_ram_enable = value == 0x0a;
            break;
        case 2:
        bank_low:
            /* Bank number, lower bits */
            if (gb->cartridge_type->mbc_type == MBC1) {
                value &= 0x1F;
            }
            if (gb->cartridge_type->mbc_type != MBC5 && !value) {
                value++;
            }
            gb->mbc_rom_bank = (gb->mbc_rom_bank & 0x100) | value;
            break;
        case 3:
            if (gb->cartridge_type->mbc_type != MBC5) goto bank_low;
            if (value > 1) {
                gb_log(gb, "Bank overflow: [%x] <- %d\n", addr, value);
            }
            gb->mbc_rom_bank = (gb->mbc_rom_bank & 0xFF) | value << 8;
            break;
        case 4:
        case 5:
            if (gb->cartridge_type->mbc_type == MBC1) {
                if (gb->mbc_ram_banking) {
                    gb->mbc_ram_bank = value & 0x3;
                }
                else {
                    gb->mbc_rom_bank = (gb->mbc_rom_bank & 0x1F) | ((value & 0x3) << 5);
                }
            }
            else {
                gb->mbc_ram_bank = value;
                /* Some games assume banks wrap around. We can do this if RAM size is a power of two */
                if (gb->mbc_ram_bank >= gb->mbc_ram_size / 0x2000 && (gb->mbc_ram_size & (gb->mbc_ram_size - 1)) == 0 && gb->mbc_ram_size != 0) {
                    gb->mbc_ram_bank %= gb->mbc_ram_size / 0x2000;
                }
            }
            break;
        case 6:
        case 7:
            if (gb->cartridge_type->mbc_type == MBC1) {
                value &= 1;

                if (value & !gb->mbc_ram_banking) {
                    gb->mbc_ram_bank = gb->mbc_rom_bank >> 5;
                    gb->mbc_rom_bank &= 0x1F;
                }
                else if (value & !gb->mbc_ram_banking) {
                    gb->mbc_rom_bank = gb->mbc_rom_bank | (gb->mbc_ram_bank << 5);
                    gb->mbc_ram_bank = 0;
                }

                gb->mbc_ram_banking = value;
            }
            break;
    }

    /* Some games assume banks wrap around. We can do this if ROM size is a power of two */
    if (gb->mbc_rom_bank >= gb->rom_size / 0x4000 && (gb->rom_size & (gb->rom_size - 1)) == 0 && gb->rom_size != 0) {
        gb->mbc_rom_bank %= gb->rom_size / 0x4000;
    }

    if (gb->cartridge_type->mbc_type != MBC5 && !gb->mbc_rom_bank) {
        gb->mbc_rom_bank = 1;
    }
}

static void write_vram(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    if ((gb->io_registers[GB_IO_STAT] & 0x3) == 3) {
        //gb_log(gb, "Wrote %02x to %04x (VRAM) during mode 3\n", value, addr);
        return;
    }
    gb->vram[(addr & 0x1FFF) + (unsigned short) gb->cgb_vram_bank * 0x2000] = value;
}

static void write_mbc_ram(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    if (gb->mbc_ram_bank >= 8 && gb->mbc_ram_bank <= 0xC) {
        /* RTC write*/
        gb->rtc_data[gb->mbc_ram_bank - 8] = value;
        gb->rtc_high |= ~0xC1; /* Not all bytes in RTC high are used. */
        return;
    }
    unsigned int ram_index = (addr & 0x1FFF) + gb->mbc_ram_bank * 0x2000;
    if (!gb->mbc_ram_enable)
    {
        gb_log(gb, "Write to %02x:%04x (%06x) (Disabled MBC RAM)\n", gb->mbc_ram_bank, addr, ram_index);
        return;
    }
    if (ram_index >= gb->mbc_ram_size) {
        gb_log(gb, "Write to %02x:%04x (%06x) (Unmapped MBC RAM)\n", gb->mbc_ram_bank, addr, ram_index);
        return;
    }
    gb->mbc_ram[(addr & 0x1FFF) + gb->mbc_ram_bank * 0x2000] = value;
}

static void write_ram(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    gb->ram[addr & 0x0FFF] = value;
}

static void write_banked_ram(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    gb->ram[(addr & 0x0FFF) + gb->cgb_ram_bank * 0x1000] = value;
}

static void write_high_memory(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    if (addr < 0xFE00) {
        gb_log(gb, "Wrote %02x to %04x (RAM Mirror)\n", value, addr);
        gb->ram[addr & 0x0FFF] = value;
        return;
    }

    if (addr < 0xFEA0) {
        if ((gb->io_registers[GB_IO_STAT] & 0x3) >= 2) {
            return;
        }
        gb->oam[addr & 0xFF] = value;
        return;
    }

    if (addr < 0xFF00) {
        gb_log(gb, "Wrote %02x to %04x (Unused)\n", value, addr);
        return;
    }

    if (addr < 0xFF80) {
        /* Hardware registers */
        switch (addr & 0xFF) {

            case GB_IO_TAC:
            case GB_IO_SCX:
            case GB_IO_IF:
            case GB_IO_TIMA:
            case GB_IO_TMA:
            case GB_IO_SCY:
            case GB_IO_LYC:
            case GB_IO_BGP:
            case GB_IO_OBP0:
            case GB_IO_OBP1:
            case GB_IO_WY:
            case GB_IO_WX:
            case GB_IO_HDMA1:
            case GB_IO_HDMA2:
            case GB_IO_HDMA3:
            case GB_IO_HDMA4:
            case GB_IO_SB:
            case GB_IO_DMG_EMULATION_INDICATION:
                gb->io_registers[addr & 0xFF] = value;
                return;

            case GB_IO_LCDC:
                if ((value & 0x80) && !(gb->io_registers[GB_IO_LCDC] & 0x80)) {
                    gb->display_cycles = 0;
                }
                gb->io_registers[GB_IO_LCDC] = value;
                return;

            case GB_IO_STAT:
                /* Delete previous R/W bits */
                gb->io_registers[GB_IO_STAT] &= 7;
                /* Set them by value */
                gb->io_registers[GB_IO_STAT] |= value & ~7;
                /* Set unused bit to 1 */
                gb->io_registers[GB_IO_STAT] |= 0x80;
                return;

            case GB_IO_DIV:
                gb->io_registers[GB_IO_DIV] = 0;
                return;

            case GB_IO_JOYP:
                gb->io_registers[GB_IO_JOYP] &= 0x0F;
                gb->io_registers[GB_IO_JOYP] |= value & 0xF0;
                return;

            case GB_IO_BIOS:
                gb->bios_finished = true;
                return;

            case GB_IO_DMG_EMULATION:
                if (gb->is_cgb && !gb->bios_finished) {
                    gb->cgb_mode = value != 4; /* The real "contents" of this register aren't quite known yet. */
                }
                return;

            case GB_IO_DMA:
                if (value <= 0xF1) { /* According to Pan Docs */
                    for (unsigned char i = 0xA0; i--;) {
                        gb->oam[i] = read_memory(gb, (value << 8) + i);
                    }
                }
                /* else { what? } */
                /* Todo: measure this value */
                gb->dma_cycles = 640;
                return;
            case GB_IO_SVBK:
                if (!gb->cgb_mode) {
                    return;
                }
                gb->cgb_ram_bank = value & 0x7;
                if (!gb->cgb_ram_bank) {
                    gb->cgb_ram_bank++;
                }
                return;
            case GB_IO_VBK:
                if (!gb->cgb_mode) {
                    return;
                }
                gb->cgb_vram_bank = value & 0x1;
                return;

            case GB_IO_BGPI:
            case GB_IO_OBPI:
                if (!gb->is_cgb) {
                    return;
                }
                gb->io_registers[addr & 0xFF] = value;
                return;
            case GB_IO_BGPD:
            case GB_IO_OBPD:
                if (!gb->is_cgb) {
                    return;
                }
                unsigned char index_reg = (addr & 0xFF) - 1;
                ((addr & 0xFF) == GB_IO_BGPD?
                 gb->background_palletes_data :
                 gb->sprite_palletes_data)[gb->io_registers[index_reg] & 0x3F] = value;
                palette_changed(gb, (addr & 0xFF) == GB_IO_BGPD, gb->io_registers[index_reg] & 0x3F);
                if (gb->io_registers[index_reg] & 0x80) {
                    gb->io_registers[index_reg]++;
                    gb->io_registers[index_reg] |= 0x80;
                }
                return;
            case GB_IO_KEY1:
                if (!gb->is_cgb) {
                    return;
                }
                gb->io_registers[GB_IO_KEY1] = value;
                return;

            case GB_IO_HDMA5:
                if ((value & 0x80) == 0 && gb->hdma_on_hblank) {
                    gb->hdma_on_hblank = false;
                    return;
                }
                gb->hdma_on = (value & 0x80) == 0;
                gb->hdma_on_hblank = (value & 0x80) != 0;
                gb->io_registers[GB_IO_HDMA5] = value;
                gb->hdma_current_src = (gb->io_registers[GB_IO_HDMA1] << 8) | (gb->io_registers[GB_IO_HDMA2] & 0xF0);
                gb->hdma_current_dest = (gb->io_registers[GB_IO_HDMA3] << 8) | (gb->io_registers[GB_IO_HDMA4] & 0xF0);
                gb->hdma_steps_left = (gb->io_registers[GB_IO_HDMA5] & 0x7F) + 1;
                gb->hdma_cycles = 0;
                return;

            case GB_IO_SC:
                if ((value & 0x80) && (value & 0x1) ) {
                    gb->io_registers[GB_IO_SB] = 0xFF;
                    gb->io_registers[GB_IO_IF] |= 0x8;
                }
                return;

            default:
                if ((addr & 0xFF) >= GB_IO_NR10 && (addr & 0xFF) <= GB_IO_WAV_END) {
                    apu_write(gb, addr & 0xFF, value);
                    return;
                }
                if (gb->io_registers[addr & 0xFF] != 0x37) {
                    gb_log(gb, "Wrote %02x to %04x (HW Register)\n", value, addr);
                }
                gb->io_registers[addr & 0xFF] = 0x37;
                return;
        }
    }

    if (addr == 0xFFFF) {
        /* Interrupt mask */
        gb->interrupt_enable = value & 0x1F;
        return;
    }
    
    /* HRAM */
    gb->hram[addr - 0xFF80] = value;
}



static GB_write_function_t * const write_map[] =
{
    write_mbc,         write_mbc,        write_mbc, write_mbc, /* 0XXX, 1XXX, 2XXX, 3XXX */
    write_mbc,         write_mbc,        write_mbc, write_mbc, /* 4XXX, 5XXX, 6XXX, 7XXX */
    write_vram,        write_vram,                             /* 8XXX, 9XXX */
    write_mbc_ram,     write_mbc_ram,                          /* AXXX, BXXX */
    write_ram,         write_banked_ram,                       /* CXXX, DXXX */
    write_high_memory, write_high_memory,                      /* EXXX FXXX */
};

void write_memory(GB_gameboy_t *gb, unsigned short addr, unsigned char value)
{
    if (addr < 0xFF00 && gb->dma_cycles) {
        /* Todo: can we access IO registers during DMA? */
        return;
    }
    write_map[addr >> 12](gb, addr, value);
}

void hdma_run(GB_gameboy_t *gb)
{
    if (!gb->hdma_on) return;
    while (gb->hdma_cycles >= 8) {
        gb->hdma_cycles -= 8;
        // The CGB bios uses the dest in "absolute" space, while some games use it relative to VRAM.
        // This "normalizes" the dest to the CGB address space.
        gb->hdma_current_dest &= 0x1fff;
        gb->hdma_current_dest |= 0x8000;
        if ((gb->hdma_current_src < 0x8000 || (gb->hdma_current_src >= 0xa000 &&  gb->hdma_current_src < 0xe000))) {
            for (unsigned char i = 0; i < 0x10; i++) {
                write_memory(gb, gb->hdma_current_dest + i, read_memory(gb, gb->hdma_current_src + i));
            }
        }
        else {
            gb->halted = false;
        }
        gb->hdma_current_src += 0x10;
        gb->hdma_current_dest += 0x10;
        if(--gb->hdma_steps_left == 0){
            gb->hdma_on = false;
            gb->hdma_on_hblank = false;
            gb->io_registers[GB_IO_HDMA5] &= 0x7F;
            break;
        }
        if (gb->hdma_on_hblank) {
            gb->hdma_on = false;
            break;
        }
    }
}