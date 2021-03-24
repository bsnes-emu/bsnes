#include "gb.h"
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define str(x) #x
#define xstr(x) str(x)
#ifdef GB_BIG_ENDIAN
#define BESS_NAME "SameBoy v" xstr(VERSION) "(Big Endian)"
#else
#define BESS_NAME "SameBoy v" xstr(VERSION)
#endif

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t size;
} BESS_block_t;

typedef struct __attribute__((packed)) {
    uint32_t size;
    uint32_t offset;
} BESS_buffer_t;

typedef struct __attribute__((packed)) {
    uint32_t start_offset;
    uint32_t magic;
} BESS_footer_t;

typedef struct __attribute__((packed)) {
    BESS_block_t header;
    uint16_t  major;
    uint16_t  minor;
    union {
        struct {
            char family;
            char model;
            char revision;
            char padding;
        };
        uint32_t full_model;
    };
    
    uint16_t pc;
    uint16_t af;
    uint16_t bc;
    uint16_t de;
    uint16_t hl;
    uint16_t sp;
    
    uint8_t ime;
    uint8_t ie;
    uint8_t execution_mode; // 0 = running; 1 = halted; 2 = stopped
    
    uint8_t io_registers[0x80];
    uint8_t hram[0x7f];
    
    BESS_buffer_t ram;
    BESS_buffer_t vram;
    BESS_buffer_t mbc_ram;
} BESS_CORE_t;

typedef struct __attribute__((packed)) {
    BESS_block_t header;
    uint8_t oam[256];
} BESS_OAM_t;

typedef struct __attribute__((packed)) {
    BESS_block_t header;
    uint8_t background_palettes[0x40];
    uint8_t sprite_palettes[0x40];
} BESS_PALS_t;

typedef struct  __attribute__((packed)) {
    uint16_t address;
    uint8_t value;
} BESS_MBC_pair_t;


#ifdef GB_BIG_ENDIAN
#define BESS16(x) __builtin_bswap16(x)
#define BESS32(x) __builtin_bswap32(x)
#else
#define BESS16(x) (x)
#define BESS32(x) (x)
#endif

typedef struct virtual_file_s virtual_file_t;
struct virtual_file_s
{
    size_t (*read)(virtual_file_t *file, void *dest, size_t length);
    size_t (*write)(virtual_file_t *file, const void *dest, size_t length);
    void (*seek)(virtual_file_t *file, ssize_t ammount, int origin);
    size_t (*tell)(virtual_file_t *file);
    union {
        FILE *file;
        struct {
            uint8_t *buffer;
            size_t position;
            size_t size;
        };
    };
};

static size_t file_read(virtual_file_t *file, void *dest, size_t length)
{
    return fread(dest, 1, length, file->file);
}

static void file_seek(virtual_file_t *file, ssize_t ammount, int origin)
{
    fseek(file->file, ammount, origin);
}

static size_t file_write(virtual_file_t *file, const void *src, size_t length)
{
    return fwrite(src, 1, length, file->file);
}

static size_t file_tell(virtual_file_t *file)
{
    return ftell(file->file);
}

static size_t buffer_read(virtual_file_t *file, void *dest, size_t length)
{
    if (length & 0x80000000) {
        return 0;
    }
    errno = 0;
    if (length > file->size - file->position) {
        errno = EIO;
        length = file->size - file->position;
    }
    
    memcpy(dest, file->buffer + file->position, length);
    file->position += length;
    
    return length;
}

static void buffer_seek(virtual_file_t *file, ssize_t ammount, int origin)
{
    switch (origin) {
        case SEEK_SET:
            file->position = ammount;
            break;
        case SEEK_CUR:
            file->position += ammount;
            break;
        case SEEK_END:
            file->position = file->size + ammount;
            break;
        default:
            break;
    }
    
    if (file->position > file->size) {
        file->position = file->size;
    }
}

static size_t buffer_write(virtual_file_t *file, const void *src, size_t size)
{
    memcpy(file->buffer + file->position, src, size);
    file->position += size;
    return size;
}

static size_t buffer_tell(virtual_file_t *file)
{
    return file->position;
}

static size_t bess_size_for_cartridge(const GB_cartridge_t *cart)
{
    switch (cart->mbc_type) {
        default:
        case GB_NO_MBC: return 0;
        case GB_MBC1:
            return sizeof(BESS_block_t) + 4 * sizeof(BESS_MBC_pair_t);
        case GB_MBC2:
            return sizeof(BESS_block_t) + 2 * sizeof(BESS_MBC_pair_t);
        case GB_MBC3:
            return sizeof(BESS_block_t) + 4 * sizeof(BESS_MBC_pair_t);
        case GB_MBC5:
            return sizeof(BESS_block_t) + 4 * sizeof(BESS_MBC_pair_t);
        case GB_HUC1:
            return sizeof(BESS_block_t) + 4 * sizeof(BESS_MBC_pair_t);
        case GB_HUC3:
            return sizeof(BESS_block_t) + 3 * sizeof(BESS_MBC_pair_t);
    }
}

size_t GB_get_save_state_size(GB_gameboy_t *gb)
{
    return GB_SECTION_SIZE(header)
    + GB_SECTION_SIZE(core_state) + sizeof(uint32_t)
    + GB_SECTION_SIZE(dma       ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(mbc       ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(hram      ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(timing    ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(apu       ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(rtc       ) + sizeof(uint32_t)
    + GB_SECTION_SIZE(video     ) + sizeof(uint32_t)
    + (GB_is_hle_sgb(gb)? sizeof(*gb->sgb) + sizeof(uint32_t) : 0)
    + gb->mbc_ram_size
    + gb->ram_size
    + gb->vram_size
    // BESS
    + sizeof(BESS_CORE_t)
    + sizeof(BESS_block_t) // NAME
    + sizeof(BESS_NAME) - 1
    + sizeof(BESS_OAM_t)
    + (GB_is_cgb(gb)? sizeof(BESS_PALS_t) : 0)
    + bess_size_for_cartridge(gb->cartridge_type) // MBC block
    + sizeof(BESS_block_t) // END block
    + sizeof(BESS_footer_t);
}

static bool verify_and_update_state_compatibility(GB_gameboy_t *gb, GB_gameboy_t *save, bool *attempt_bess)
{
    *attempt_bess = false;
    if (save->ram_size == 0 && (&save->ram_size)[-1] == gb->ram_size) {
        /* This is a save state with a bad printer struct from a 32-bit OS */
        memmove(save->extra_oam + 4, save->extra_oam, (uintptr_t)&save->ram_size - (uintptr_t)&save->extra_oam);
    }
    if (save->ram_size == 0) {
        /* Save doesn't have ram size specified, it's a pre 0.12 save state with potentially
         incorrect RAM amount if it's a CGB instance */
        if (GB_is_cgb(save)) {
            save->ram_size = 0x2000 * 8; // Incorrect RAM size
        }
        else {
            save->ram_size = gb->ram_size;
        }
    }
    
    if (save->model & GB_MODEL_PAL_BIT_OLD) {
        save->model &= ~GB_MODEL_PAL_BIT_OLD;
        save->model |= GB_MODEL_PAL_BIT;
    }
    
    if (save->model & GB_MODEL_NO_SFC_BIT_OLD) {
        save->model &= ~GB_MODEL_NO_SFC_BIT_OLD;
        save->model |= GB_MODEL_NO_SFC_BIT;
    }
    
    if (gb->version != save->version) {
        GB_log(gb, "The save state is for a different version of SameBoy.\n");
        *attempt_bess = true;
        return false;
    }
    
    if (gb->mbc_ram_size < save->mbc_ram_size) {
        GB_log(gb, "The save state has non-matching MBC RAM size.\n");
        return false;
    }
    
    if (gb->vram_size != save->vram_size) {
        GB_log(gb, "The save state has non-matching VRAM size. Try changing the emulated model.\n");
        return false;
    }
    
    if (GB_is_hle_sgb(gb) != GB_is_hle_sgb(save)) {
        GB_log(gb, "The save state is %sfor a Super Game Boy. Try changing the emulated model.\n", GB_is_hle_sgb(save)? "" : "not ");
        return false;
    }
    
    if (gb->ram_size != save->ram_size) {
        if (gb->ram_size == 0x1000 * 8 && save->ram_size == 0x2000 * 8) {
            /* A bug in versions prior to 0.12 made CGB instances allocate twice the ammount of RAM.
               Ignore this issue to retain compatibility with older, 0.11, save states. */
        }
        else {
            GB_log(gb, "The save state has non-matching RAM size. Try changing the emulated model.\n");
            return false;
        }
    }
    
    return true;
}

static void sanitize_state(GB_gameboy_t *gb)
{
    for (unsigned i = 0; i < 32; i++) {
        GB_palette_changed(gb, false, i * 2);
        GB_palette_changed(gb, true, i * 2);
    }
    
    gb->bg_fifo.read_end &= 0xF;
    gb->bg_fifo.write_end &= 0xF;
    gb->oam_fifo.read_end &= 0xF;
    gb->oam_fifo.write_end &= 0xF;
    gb->object_low_line_address &= gb->vram_size & ~1;
    gb->fetcher_x &= 0x1f;
    if (gb->lcd_x > gb->position_in_line) {
        gb->lcd_x = gb->position_in_line;
    }
    
    if (gb->object_priority == GB_OBJECT_PRIORITY_UNDEFINED) {
        gb->object_priority = gb->cgb_mode? GB_OBJECT_PRIORITY_INDEX : GB_OBJECT_PRIORITY_X;
    }
}

static bool dump_section(virtual_file_t *file, const void *src, uint32_t size)
{
    if (file->write(file, &size, sizeof(size)) != sizeof(size)) {
        return false;
    }
    
    if (file->write(file, src, size) != size) {
        return false;
    }
    
    return true;
}

#define DUMP_SECTION(gb, f, section) dump_section(f, GB_GET_SECTION(gb, section), GB_SECTION_SIZE(section))

static int save_bess_mbc_block(GB_gameboy_t *gb, virtual_file_t *file)
{
    
    BESS_block_t mbc_block = {htonl('MBC '), 0};
    BESS_MBC_pair_t pairs[4];
    switch (gb->cartridge_type->mbc_type) {
        default:
        case GB_NO_MBC: return 0;
        case GB_MBC1:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->mbc_ram_enable? 0xA : 0x0};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x2000), gb->mbc1.bank_low};
            pairs[2] = (BESS_MBC_pair_t){BESS16(0x4000), gb->mbc1.bank_high};
            pairs[3] = (BESS_MBC_pair_t){BESS16(0x6000), gb->mbc1.mode};
            mbc_block.size = 4 * sizeof(pairs[0]);
            break;
        case GB_MBC2:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->mbc_ram_enable? 0xA : 0x0};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x0100), gb->mbc2.rom_bank};
            mbc_block.size = 2 * sizeof(pairs[0]);
            break;
        case GB_MBC3:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->mbc_ram_enable? 0xA : 0x0};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x2000), gb->mbc3.rom_bank};
            pairs[2] = (BESS_MBC_pair_t){BESS16(0x4000), gb->mbc3.ram_bank | (gb->mbc3_rtc_mapped? 8 : 0)};
            pairs[3] = (BESS_MBC_pair_t){BESS16(0x6000), gb->rtc_latch};
            mbc_block.size = 4 * sizeof(pairs[0]);
            break;
        case GB_MBC5:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->mbc_ram_enable? 0xA : 0x0};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x2000), gb->mbc5.rom_bank_low};
            pairs[2] = (BESS_MBC_pair_t){BESS16(0x3000), gb->mbc5.rom_bank_high};
            pairs[3] = (BESS_MBC_pair_t){BESS16(0x4000), gb->mbc5.ram_bank};
            mbc_block.size = 4 * sizeof(pairs[0]);
            break;
        case GB_HUC1:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->huc1.ir_mode? 0xE : 0x0};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x2000), gb->huc1.bank_low};
            pairs[2] = (BESS_MBC_pair_t){BESS16(0x4000), gb->huc1.bank_high};
            pairs[3] = (BESS_MBC_pair_t){BESS16(0x6000), gb->huc1.mode};
            mbc_block.size = 4 * sizeof(pairs[0]);
            
        case GB_HUC3:
            pairs[0] = (BESS_MBC_pair_t){BESS16(0x0000), gb->huc3_mode};
            pairs[1] = (BESS_MBC_pair_t){BESS16(0x2000), gb->huc3.rom_bank};
            pairs[2] = (BESS_MBC_pair_t){BESS16(0x4000), gb->huc3.ram_bank};
            mbc_block.size = 3 * sizeof(pairs[0]);
            break;
    }
    
    if (file->write(file, &mbc_block, sizeof(mbc_block)) != sizeof(mbc_block)) {
        return errno;
    }
    
    if (file->write(file, &pairs, mbc_block.size) != mbc_block.size) {
        return errno;
    }
    
    return 0;
}

static int save_state_internal(GB_gameboy_t *gb, virtual_file_t *file)
{
    if (file->write(file, GB_GET_SECTION(gb, header), GB_SECTION_SIZE(header)) != GB_SECTION_SIZE(header)) goto error;
    if (!DUMP_SECTION(gb, file, core_state)) goto error;
    if (!DUMP_SECTION(gb, file, dma       )) goto error;
    if (!DUMP_SECTION(gb, file, mbc       )) goto error;
    if (!DUMP_SECTION(gb, file, hram      )) goto error;
    if (!DUMP_SECTION(gb, file, timing    )) goto error;
    if (!DUMP_SECTION(gb, file, apu       )) goto error;
    if (!DUMP_SECTION(gb, file, rtc       )) goto error;
    if (!DUMP_SECTION(gb, file, video     )) goto error;
    
    if (GB_is_hle_sgb(gb)) {
        if (!dump_section(file, gb->sgb, sizeof(*gb->sgb))) goto error;
    }
    
    BESS_CORE_t bess_core;
    
    bess_core.mbc_ram.offset = file->tell(file);
    bess_core.mbc_ram.size = gb->mbc_ram_size;
    if (file->write(file, gb->mbc_ram, gb->mbc_ram_size) != gb->mbc_ram_size) {
        goto error;
    }
    
    bess_core.ram.offset = file->tell(file);
    bess_core.ram.size = gb->ram_size;
    if (file->write(file, gb->ram, gb->ram_size) != gb->ram_size) {
        goto error;
    }
    
    bess_core.vram.offset = file->tell(file);
    bess_core.vram.size = gb->vram_size;
    if (file->write(file, gb->vram, gb->vram_size) != gb->vram_size) {
        goto error;
    }
    
    BESS_footer_t bess_footer = {
        .start_offset = file->tell(file),
        .magic = htonl('BESS'),
    };
    
    /* BESS NAME */
    
    static const BESS_block_t bess_name = {htonl('NAME'), BESS32(sizeof(BESS_NAME) - 1)};
    
    if (file->write(file, &bess_name, sizeof(bess_name)) != sizeof(bess_name)) {
        goto error;
    }
    
    if (file->write(file, BESS_NAME, sizeof(BESS_NAME) - 1) != sizeof(BESS_NAME) - 1) {
        goto error;
    }
    
    /* BESS CORE */
    
    bess_core.header = (BESS_block_t){htonl('CORE'), BESS32(sizeof(bess_core) - sizeof(bess_core.header))};
    bess_core.major = BESS16(1);
    bess_core.minor = BESS16(1);
    switch (gb->model) {

        case GB_MODEL_DMG_B: bess_core.full_model = htonl('GDB '); break;
            
        case GB_MODEL_SGB_NTSC:
        case GB_MODEL_SGB_NTSC_NO_SFC:
            bess_core.full_model = htonl('SN  '); break;
            
        case GB_MODEL_SGB_PAL_NO_SFC:
        case GB_MODEL_SGB_PAL:
            bess_core.full_model = htonl('SP  '); break;
        
        case GB_MODEL_SGB2_NO_SFC:
        case GB_MODEL_SGB2:
            bess_core.full_model = htonl('S2  '); break;
 
 
        case GB_MODEL_CGB_C: bess_core.full_model = htonl('CCC '); break;
        case GB_MODEL_CGB_E: bess_core.full_model = htonl('CCE '); break;
        case GB_MODEL_AGB: bess_core.full_model = htonl('CA  '); break; // SameBoy doesn't emulate a specific AGB revision yet
    }
    
    bess_core.pc = BESS16(gb->pc);
    bess_core.af = BESS16(gb->af);
    bess_core.bc = BESS16(gb->bc);
    bess_core.de = BESS16(gb->de);
    bess_core.hl = BESS16(gb->hl);
    bess_core.sp = BESS16(gb->sp);
    
    bess_core.ime = gb->ime;
    bess_core.ie = gb->interrupt_enable;
    bess_core.execution_mode = 0;
    if (gb->halted) {
        bess_core.execution_mode = 1;
    }
    else if (gb->stopped) {
        bess_core.execution_mode = 2;
    }
    
    memcpy(bess_core.io_registers, gb->io_registers, sizeof(gb->io_registers));
    bess_core.io_registers[GB_IO_DIV] = gb->div_counter >> 8;
    bess_core.io_registers[GB_IO_BANK] = gb->boot_rom_finished;
    bess_core.io_registers[GB_IO_KEY1] |= gb->cgb_double_speed? 0x80 : 0;
    memcpy(bess_core.hram, gb->hram, sizeof(gb->hram));
    
    
    if (file->write(file, &bess_core, sizeof(bess_core)) != sizeof(bess_core)) {
        goto error;
    }
        
    /* BESS OAM */
    
    BESS_OAM_t bess_oam;
    bess_oam.header = (BESS_block_t){htonl('OAM '), BESS32(sizeof(bess_oam) - sizeof(bess_oam.header))};
    memcpy(bess_oam.oam, gb->oam, sizeof(gb->oam));
    memcpy(bess_oam.oam + sizeof(gb->oam), gb->extra_oam, sizeof(gb->extra_oam));
    
    if (file->write(file, &bess_oam, sizeof(bess_oam)) != sizeof(bess_oam)) {
        goto error;
    }
    
    save_bess_mbc_block(gb, file);
    
    if (GB_is_cgb(gb)) {
        /* BESS PALS */
        
        BESS_PALS_t bess_pals;
        bess_pals.header = (BESS_block_t){htonl('PALS'), BESS32(sizeof(bess_pals) - sizeof(bess_oam.header))};
        memcpy(bess_pals.background_palettes, gb->background_palettes_data, sizeof(bess_pals.background_palettes));
        memcpy(bess_pals.sprite_palettes, gb->sprite_palettes_data, sizeof(bess_pals.sprite_palettes));

        if (file->write(file, &bess_pals, sizeof(bess_pals)) != sizeof(bess_pals)) {
            goto error;
        }
    }
    
    /* BESS END */
    
    static const BESS_block_t bess_end = {htonl('END '), 0};

    if (file->write(file, &bess_end, sizeof(bess_end)) != sizeof(bess_end)) {
        goto error;
    }
    
    /* BESS Footer */
    
    if (file->write(file, &bess_footer, sizeof(bess_footer)) != sizeof(bess_footer)) {
        goto error;
    }
    
    errno = 0;
error:
    return errno;
}

int GB_save_state(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) {
        GB_log(gb, "Could not open save state: %s.\n", strerror(errno));
        return errno;
    }
    virtual_file_t file = {
        .write = file_write,
        .seek = file_seek,
        .tell = file_tell,
        .file = f,
    };
    int ret = save_state_internal(gb, &file);
    fclose(f);
    return ret;
}

void GB_save_state_to_buffer(GB_gameboy_t *gb, uint8_t *buffer)
{
    virtual_file_t file = {
        .write = buffer_write,
        .seek = buffer_seek,
        .tell = buffer_tell,
        .buffer = (uint8_t *)buffer,
        .position = 0,
    };
    
    save_state_internal(gb, &file);
    assert(file.position == GB_get_save_state_size(gb));
}


static bool read_section(virtual_file_t *file, void *dest, uint32_t size, bool fix_broken_windows_saves)
{
    uint32_t saved_size = 0;
    if (file->read(file, &saved_size, sizeof(size)) != sizeof(size)) {
        return false;
    }
    
    if (fix_broken_windows_saves) {
        if (saved_size < 4) {
            return false;
        }
        saved_size -= 4;
        file->seek(file, 4, SEEK_CUR);
    }
    
    if (saved_size <= size) {
        if (file->read(file, dest, saved_size) != saved_size) {
            return false;
        }
    }
    else {
        if (file->read(file, dest, size) != size) {
            return false;
        }
        file->seek(file, saved_size - size, SEEK_CUR);
    }
    
    return true;
}

static void read_bess_buffer(const BESS_buffer_t *buffer, virtual_file_t *file, uint8_t *dest, size_t max_size)
{
    size_t pos = file->tell(file);
    file->seek(file, BESS32(buffer->offset), SEEK_SET);
    file->read(file, dest, MIN(BESS32(buffer->size), max_size));
    file->seek(file, pos, SEEK_SET);
}

static int load_bess_save(GB_gameboy_t *gb, virtual_file_t *file, bool is_sameboy)
{
    char emulator_name[65] = {0,};
    file->seek(file, -sizeof(BESS_footer_t), SEEK_END);
    BESS_footer_t footer = {0, };
    file->read(file, &footer, sizeof(footer));
    if (footer.magic != htonl('BESS')) {
        // Not a BESS file
        if (!is_sameboy) {
            GB_log(gb, "The file is not a save state, or is from an incompatible operating system.\n");
        }
        return -1;
    }
    
    GB_gameboy_t save;
    GB_init(&save, gb->model);
    save.cartridge_type = gb->cartridge_type;
    
    file->seek(file, BESS32(footer.start_offset), SEEK_SET);
    bool found_core = false;
    BESS_CORE_t core;
    while (true) {
        BESS_block_t block;
        if (file->read(file, &block, sizeof(block)) != sizeof(block)) goto error;
        switch (block.magic) {
            case htonl('CORE'):
                if (found_core) goto parse_error;
                found_core = true;
                if (BESS32(block.size) > sizeof(core) - sizeof(block)) {
                    if (file->read(file, &core.header + 1, sizeof(core) - sizeof(block)) != sizeof(core) - sizeof(block)) goto error;
                    file->seek(file, BESS32(block.size) - (sizeof(core) - sizeof(block)), SEEK_CUR);
                }
                else {
                    if (file->read(file, &core.header + 1, BESS32(block.size)) != BESS32(block.size)) goto error;
                }
                
                if (core.major != BESS16(1)) {
                    GB_log(gb, "This save state uses an incompatible version of the BESS specification");
                    GB_free(&save);
                    return -1;
                }
                
                switch (core.family) {
                    case 'C':
                        if (!GB_is_cgb(&save)) goto wrong_model;
                        break;
                    case 'S':
                        if (!GB_is_sgb(&save)) goto wrong_model;
                        break;
                    case 'G':
                        if (GB_is_cgb(&save) || GB_is_sgb(&save)) goto wrong_model;
                        break;
                    default:
                    wrong_model:
                        GB_log(gb, "The save state is for a different model. Try changing the emulated model.\n");
                        GB_free(&save);
                        return -1;
                }

                
                save.pc = BESS16(core.pc);
                save.af = BESS16(core.af);
                save.bc = BESS16(core.bc);
                save.de = BESS16(core.de);
                save.hl = BESS16(core.hl);
                save.sp = BESS16(core.sp);
                
                save.ime = core.ime;
                save.interrupt_enable = core.ie;
                
                save.halted = core.execution_mode == 1;
                save.stopped = core.execution_mode == 2;
                
                memcpy(save.hram, core.hram, sizeof(save.hram));
                
                // CPU related
                
                // Determines DMG mode
                GB_write_memory(&save, 0xFF00 + GB_IO_KEY0, core.io_registers[GB_IO_KEY0]);
                save.boot_rom_finished = core.io_registers[GB_IO_BANK];
                GB_write_memory(&save, 0xFF00 + GB_IO_KEY1, core.io_registers[GB_IO_KEY1]);
                if (save.cgb_mode) {
                    save.cgb_double_speed = core.io_registers[GB_IO_KEY1] & 0x80;
                    save.object_priority = GB_OBJECT_PRIORITY_INDEX;
                }
                else {
                    save.object_priority = GB_OBJECT_PRIORITY_X;
                }
                
                // Timers, Joypad and Serial
                GB_write_memory(&save, 0xFF00 + GB_IO_JOYP, core.io_registers[GB_IO_JOYP]);
                GB_write_memory(&save, 0xFF00 + GB_IO_SB, core.io_registers[GB_IO_SB]);
                save.io_registers[GB_IO_SC] = core.io_registers[GB_IO_SC];
                save.div_counter = core.io_registers[GB_IO_DIV] << 8;
                GB_write_memory(&save, 0xFF00 + GB_IO_TIMA, core.io_registers[GB_IO_TIMA]);
                GB_write_memory(&save, 0xFF00 + GB_IO_TMA, core.io_registers[GB_IO_TMA]);
                GB_write_memory(&save, 0xFF00 + GB_IO_TAC, core.io_registers[GB_IO_TAC]);
                
                // APU
                GB_write_memory(&save, 0xFF00 + GB_IO_NR52, core.io_registers[GB_IO_NR52]);
                for (unsigned i = GB_IO_NR10; i < GB_IO_NR52; i++) {
                    uint8_t value = core.io_registers[i];
                    if (i == GB_IO_NR14 || i == GB_IO_NR24 || i == GB_IO_NR34 || i == GB_IO_NR44) {
                        value &= ~0x80;
                    }
                    GB_write_memory(&save, 0xFF00 + i, value);
                }
                
                for (unsigned i = GB_IO_WAV_START; i <= GB_IO_WAV_END; i++) {
                    GB_write_memory(&save, 0xFF00 + i, core.io_registers[i]);
                }
                
                // PPU
                GB_write_memory(&save, 0xFF00 + GB_IO_LCDC, core.io_registers[GB_IO_LCDC]);
                GB_write_memory(&save, 0xFF00 + GB_IO_STAT, core.io_registers[GB_IO_STAT]);
                GB_write_memory(&save, 0xFF00 + GB_IO_SCY, core.io_registers[GB_IO_SCY]);
                GB_write_memory(&save, 0xFF00 + GB_IO_SCX, core.io_registers[GB_IO_SCX]);
                GB_write_memory(&save, 0xFF00 + GB_IO_LYC, core.io_registers[GB_IO_LYC]);
                save.io_registers[GB_IO_DMA] = core.io_registers[GB_IO_DMA];
                GB_write_memory(&save, 0xFF00 + GB_IO_BGP, core.io_registers[GB_IO_BGP]);
                GB_write_memory(&save, 0xFF00 + GB_IO_OBP0, core.io_registers[GB_IO_OBP0]);
                GB_write_memory(&save, 0xFF00 + GB_IO_OBP1, core.io_registers[GB_IO_OBP1]);
                GB_write_memory(&save, 0xFF00 + GB_IO_WX, core.io_registers[GB_IO_WX]);
                GB_write_memory(&save, 0xFF00 + GB_IO_WY, core.io_registers[GB_IO_WY]);
                
                // Other registers
                GB_write_memory(&save, 0xFF00 + GB_IO_VBK, core.io_registers[GB_IO_VBK]);
                GB_write_memory(&save, 0xFF00 + GB_IO_HDMA1, core.io_registers[GB_IO_HDMA1]);
                GB_write_memory(&save, 0xFF00 + GB_IO_HDMA2, core.io_registers[GB_IO_HDMA2]);
                GB_write_memory(&save, 0xFF00 + GB_IO_HDMA3, core.io_registers[GB_IO_HDMA3]);
                GB_write_memory(&save, 0xFF00 + GB_IO_HDMA4, core.io_registers[GB_IO_HDMA4]);
                GB_write_memory(&save, 0xFF00 + GB_IO_RP, core.io_registers[GB_IO_RP]);
                GB_write_memory(&save, 0xFF00 + GB_IO_BGPI, core.io_registers[GB_IO_BGPI]);
                GB_write_memory(&save, 0xFF00 + GB_IO_OBPI, core.io_registers[GB_IO_OBPI]);
                GB_write_memory(&save, 0xFF00 + GB_IO_OPRI, core.io_registers[GB_IO_OPRI]);
                GB_write_memory(&save, 0xFF00 + GB_IO_SVBK, core.io_registers[GB_IO_SVBK]);

                // Interrupts
                GB_write_memory(&save, 0xFF00 + GB_IO_IF, core.io_registers[GB_IO_IF]);
                
                break;
            case htonl('NAME'):
                if (BESS32(block.size) > sizeof(emulator_name) - 1) {
                    file->seek(file, BESS32(block.size), SEEK_CUR);
                }
                else {
                    file->read(file, emulator_name, BESS32(block.size));
                }
                break;
            case htonl('OAM '):
                if (!found_core) goto parse_error;
                if (BESS32(block.size) != 256 && BESS32(block.size) != 160) goto parse_error;
                file->read(file, save.oam, sizeof(save.oam));
                if (BESS32(block.size) == 256) {
                    file->read(file, save.extra_oam, sizeof(save.extra_oam));
                }
                break;
            case htonl('PALS'):
                if (!found_core) goto parse_error;
                if (BESS32(block.size) != sizeof(BESS_PALS_t) - sizeof(block)) goto parse_error;
                file->read(file, save.background_palettes_data, sizeof(save.background_palettes_data));
                file->read(file, save.sprite_palettes_data, sizeof(save.sprite_palettes_data));
                break;
            case htonl('MBC '):
                if (!found_core) goto parse_error;
                if (BESS32(block.size) % 3 != 0) goto parse_error;
                for (unsigned i = BESS32(block.size); i > 0;  i -= 3) {
                    BESS_MBC_pair_t pair;
                    file->read(file, &pair, sizeof(pair));
                    if (BESS16(pair.address) >= 0x8000) goto parse_error;
                    GB_write_memory(&save, BESS16(pair.address), pair.value);
                }
                break;
            case htonl('END '):
                if (!found_core) goto parse_error;
                if (BESS32(block.size) != 0) goto parse_error;
                goto done;
            default:
                file->seek(file, BESS32(block.size), SEEK_CUR);
                break;
        }
    }
done:
    save.mbc_ram_size = gb->mbc_ram_size;
    memcpy(gb, &save, GB_SECTION_OFFSET(unsaved));
    assert(GB_get_save_state_size(gb) == GB_get_save_state_size(&save));
    GB_free(&save);
    read_bess_buffer(&core.ram, file, gb->ram, gb->ram_size);
    read_bess_buffer(&core.vram, file, gb->vram, gb->vram_size);
    read_bess_buffer(&core.mbc_ram, file, gb->mbc_ram, gb->mbc_ram_size);
    if (emulator_name[0]) {
        GB_log(gb, "Save state imported from %s.\n", emulator_name);
    }
    else {
        GB_log(gb, "Save state imported from another emulator.\n"); // SameBoy always contains a NAME block
    }
    for (unsigned i = 0; i < 32; i++) {
        GB_palette_changed(gb, false, i * 2);
        GB_palette_changed(gb, true, i * 2);
    }
    return 0;
parse_error:
    errno = -1;
error:
    if (emulator_name[0]) {
        GB_log(gb, "Attempted to import a save state from %s, but the save state is invalid.\n", emulator_name);
    }
    else {
        GB_log(gb, "Attempted to import a save state from a different emulator or incompatible version, but the save state is invalid.\n");
    }
    GB_free(&save);
    return errno;
}

static int load_state_internal(GB_gameboy_t *gb, virtual_file_t *file)
{
    GB_gameboy_t save;
    
    /* Every unread value should be kept the same. */
    memcpy(&save, gb, sizeof(save));
    /* ...Except ram size, we use it to detect old saves with incorrect ram sizes */
    save.ram_size = 0;
    
    bool fix_broken_windows_saves = false;
    
    if (file->read(file, GB_GET_SECTION(&save, header), GB_SECTION_SIZE(header)) != GB_SECTION_SIZE(header)) return errno;
    if (save.magic == 0) {
        /* Potentially legacy, broken Windows save state*/
        
        file->seek(file, 4, SEEK_SET);
        if (file->read(file, GB_GET_SECTION(&save, header), GB_SECTION_SIZE(header)) != GB_SECTION_SIZE(header)) return errno;
        fix_broken_windows_saves = true;
    }
    if (gb->magic != save.magic) {
        return load_bess_save(gb, file, false);
    }
#define READ_SECTION(gb, file, section) read_section(file, GB_GET_SECTION(gb, section), GB_SECTION_SIZE(section), fix_broken_windows_saves)
    if (!READ_SECTION(&save, file, core_state)) return errno ?: EIO;
    if (!READ_SECTION(&save, file, dma       )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, mbc       )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, hram      )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, timing    )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, apu       )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, rtc       )) return errno ?: EIO;
    if (!READ_SECTION(&save, file, video     )) return errno ?: EIO;
#undef READ_SECTION
    
    
    bool attempt_bess = false;
    if (!verify_and_update_state_compatibility(gb, &save, &attempt_bess)) {
        if (attempt_bess) {
            return load_bess_save(gb, file, true);
        }
        return errno;
    }
    
    if (GB_is_hle_sgb(gb)) {
        if (!read_section(file, gb->sgb, sizeof(*gb->sgb), false)) return errno ?: EIO;
    }
    
    memset(gb->mbc_ram + save.mbc_ram_size, 0xFF, gb->mbc_ram_size - save.mbc_ram_size);
    if (file->read(file, gb->mbc_ram, save.mbc_ram_size) != save.mbc_ram_size) {
        return errno ?: EIO;
    }
    
    if (file->read(file, gb->ram, gb->ram_size) != gb->ram_size) {
        return errno ?: EIO;
    }
    
    /* Fix for 0.11 save states that allocate twice the amount of RAM in CGB instances */
    file->seek(file, save.ram_size - gb->ram_size, SEEK_CUR);
    
    if (file->read(file, gb->vram, gb->vram_size) != gb->vram_size) {
        return errno ?: EIO;
    }
    
    size_t orig_ram_size = gb->ram_size;
    memcpy(gb, &save, sizeof(save));
    gb->ram_size = orig_ram_size;
    
    sanitize_state(gb);
    
    return 0;
}

int GB_load_state(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        GB_log(gb, "Could not open save state: %s.\n", strerror(errno));
        return errno;
    }
    virtual_file_t file = {
        .read = file_read,
        .seek = file_seek,
        .tell = file_tell,
        .file = f,
    };
    int ret = load_state_internal(gb, &file);
    fclose(f);
    return ret;
}

int GB_load_state_from_buffer(GB_gameboy_t *gb, const uint8_t *buffer, size_t length)
{
    virtual_file_t file = {
        .read = buffer_read,
        .seek = buffer_seek,
        .tell = buffer_tell,
        .buffer = (uint8_t *)buffer,
        .position = 0,
        .size = length,
    };
    
    return load_state_internal(gb, &file);
}
