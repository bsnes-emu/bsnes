#include <stdbool.h>
#include "gb.h"

const GB_cartridge_t GB_cart_defs[256] = {
    // From http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header#0147_-_Cartridge_Type
    /* MBC        RAM    BAT.   RTC    RUMB.    */
    {  GB_NO_MBC, false, false, false, false}, // 00h  ROM ONLY
    {  GB_MBC1  , false, false, false, false}, // 01h  MBC1
    {  GB_MBC1  , true , false, false, false}, // 02h  MBC1+RAM
    {  GB_MBC1  , true , true , false, false}, // 03h  MBC1+RAM+BATTERY
    [5] =
    {  GB_MBC2  , true , false, false, false}, // 05h  MBC2
    {  GB_MBC2  , true , true , false, false}, // 06h  MBC2+BATTERY
    [8] =
    {  GB_NO_MBC, true , false, false, false}, // 08h  ROM+RAM
    {  GB_NO_MBC, true , true , false, false}, // 09h  ROM+RAM+BATTERY
    [0xB] =
    // Todo: What are these?
    {  GB_NO_MBC, false, false, false, false}, // 0Bh  MMM01
    {  GB_NO_MBC, false, false, false, false}, // 0Ch  MMM01+RAM
    {  GB_NO_MBC, false, false, false, false}, // 0Dh  MMM01+RAM+BATTERY
    [0xF] =
    {  GB_MBC3  , false, true,  true , false}, // 0Fh  MBC3+TIMER+BATTERY
    {  GB_MBC3  , true , true,  true , false}, // 10h  MBC3+TIMER+RAM+BATTERY
    {  GB_MBC3  , false, false, false, false}, // 11h  MBC3
    {  GB_MBC3  , true , false, false, false}, // 12h  MBC3+RAM
    {  GB_MBC3  , true , true , false, false}, // 13h  MBC3+RAM+BATTERY
    [0x15] =
    // Todo: Do these exist?
    {  GB_MBC4  , false, false, false, false}, // 15h  MBC4
    {  GB_MBC4  , true , false, false, false}, // 16h  MBC4+RAM
    {  GB_MBC4  , true , true , false, false}, // 17h  MBC4+RAM+BATTERY
    [0x19] =
    {  GB_MBC5  , false, false, false, false}, // 19h  MBC5
    {  GB_MBC5  , true , false, false, false}, // 1Ah  MBC5+RAM
    {  GB_MBC5  , true , true , false, false}, // 1Bh  MBC5+RAM+BATTERY
    {  GB_MBC5  , false, false, false, true }, // 1Ch  MBC5+RUMBLE
    {  GB_MBC5  , true , false, false, true }, // 1Dh  MBC5+RUMBLE+RAM
    {  GB_MBC5  , true , true , false, true }, // 1Eh  MBC5+RUMBLE+RAM+BATTERY
    [0xFC] =
    // Todo: What are these?
    {  GB_NO_MBC, false, false, false, false}, // FCh  POCKET CAMERA
    {  GB_NO_MBC, false, false, false, false}, // FDh  BANDAI TAMA5
    {  GB_NO_MBC, false, false, false, false}, // FEh  HuC3
    {  GB_NO_MBC, true , true , false, false}, // FFh  HuC1+RAM+BATTERY
};

void GB_update_mbc_mappings(GB_gameboy_t *gb)
{
    switch (gb->cartridge_type->mbc_type) {
        case GB_NO_MBC: case GB_MBC4: return;
        case GB_MBC1:
            /* Standard MBC1 wiring: */
            if (gb->mbc1.mode == 0) {
                gb->mbc_rom_bank = gb->mbc1.bank_low | (gb->mbc1.bank_high << 5);
                gb->mbc_ram_bank = 0;
            }
            else {
                gb->mbc_rom_bank = gb->mbc1.bank_low;
                gb->mbc_ram_bank = gb->mbc1.bank_high;
            }
            break;
        case GB_MBC2:
            gb->mbc_rom_bank = gb->mbc2.rom_bank;
            break;
        case GB_MBC3:
            gb->mbc_rom_bank = gb->mbc3.rom_bank;
            gb->mbc_ram_bank = gb->mbc3.ram_bank;
            break;
        case GB_MBC5:
            gb->mbc_rom_bank = gb->mbc5.rom_bank_low | (gb->mbc5.rom_bank_high << 8);
            gb->mbc_ram_bank = gb->mbc5.ram_bank;
            break;
    }
    if (gb->mbc_rom_bank == 0 && gb->cartridge_type->mbc_type != GB_MBC5) {
        gb->mbc_rom_bank = 1;
    }
}