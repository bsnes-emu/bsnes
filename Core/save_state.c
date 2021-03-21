#include "gb.h"
#include <stdio.h>
#include <errno.h>

typedef struct virtual_file_s virtual_file_t;
struct virtual_file_s
{
    size_t (*read)(virtual_file_t *file, void *dest, size_t length);
    size_t (*write)(virtual_file_t *file, const void *dest, size_t length);
    void (*seek)(virtual_file_t *file, ssize_t ammount, int origin);
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

static size_t buffer_read(virtual_file_t *file, void *dest, size_t length)
{
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
    + gb->vram_size;
}

static bool verify_and_update_state_compatibility(GB_gameboy_t *gb, GB_gameboy_t *save)
{
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
    
    if (file->write(file, gb->mbc_ram, gb->mbc_ram_size) != gb->mbc_ram_size) {
        goto error;
    }
    
    if (file->write(file, gb->ram, gb->ram_size) != gb->ram_size) {
        goto error;
    }
    
    if (file->write(file, gb->vram, gb->vram_size) != gb->vram_size) {
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
        .buffer = (uint8_t *)buffer,
        .position = 0,
    };
    
    save_state_internal(gb, &file);
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
        GB_log(gb, "The file is not a save state, or is from an incompatible operating system.\n");
        return false;
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
    
    
    if (!verify_and_update_state_compatibility(gb, &save)) {
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
        .buffer = (uint8_t *)buffer,
        .position = 0,
        .size = length,
    };
    
    return load_state_internal(gb, &file);
}
