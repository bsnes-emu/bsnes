# BESS – Best Effort Save State

## Motivation

BESS is a save state format specification designed to allow different emulators, as well as majorly different versions of the same emulator, to import save states from other BESS-compliant save states. BESS works by appending additional, implementation-agnostic information about the emulation state. This allows a single save state file to be read as both a fully-featured, implementation specific save state which includes detailed timing information; and as a portable "best effort" save state that represents a state accurately enough to be restored in casual use-cases.

## Specification

**Note: This specification is currently under development and will potentially change in the near future**

Every integer used in the BESS specification is stored in Little Endian encoding.

### BESS footer

BESS works by appending a detectable footer at the end of an existing save state format. The footer uses the following format:

| Offset from end of file | Content                                               |
|-------------------------|-------------------------------------------------------|
| -8                      | Offset to the first BESS Block, from the file's start |
| -4                      | The ASCII string 'BESS'                               |

### BESS blocks

BESS uses a block format where each block contains the following header:

| Offset | Content                               |
|--------|---------------------------------------|
| 0      | A four-letter ASCII identifier        |
| 4      | Length of the block, excluding header |

Every block is followed by another blocked, until the END block is reached.

#### CORE block

The CORE block uses the `'CORE'` identifier, and is a required block that contains both core state information, as well as basic information about the BESS version used. This block must be the first block, except for the `NAME` block.

The length of the CORE block is 0x12E bytes, but implementations are expected to ignore any excess bytes. Following the BESS block header, the structure is as follows:

| Offset | Content                                |
|--------|----------------------------------------|
| 0x000  | Major BESS version as a 16-bit integer |
| 0x002  | Minor BESS version as a 16-bit integer |

Both major and minor versions should be 1. Implementations are expected to reject incompatible majors, but still attempt to read newer minor versions.

| Offset | Content                                 |
|--------|-----------------------------------------|
| 0x004  | A four-character ASCII model identifier |

BESS uses a four-character string to identify Game Boy models:

 * The first letter represents mutually-incompatible families of models and is required. The allowed values are `'G'` for the original Game Boy family, `'S'` for the Super Game Boy family, and `'C'` for the Game Boy Color and Advance family.
* The second letter represents a specific model within the family, and is optional (If an implementation does not distinguish between specific models in a family, a space character may be used). The allowed values for family G are `'D'` for DMG and `'M'` for MGB; the allowed values for family S are `'N'` for NTSC, `'P'` for PAL, and `'2'` for SGB2; and the allowed values for family C are `'C'` for CGB, and `'A'` for the various GBA line models.
* The third letter represents a specific CPU revision within a model, and is optional (If an implementation does not distinguish between revisions, a space character may be used). The allowed values for model GD (DMG) are `'0'` and `'A'`, through `'C'`; the allowed values for model CC (CGB) are `'0'` and `'A'`, through `'E'`; the allowed values for model CA (AGB, AGS, GBP) are `'0'`, `'A'` and `'B'`; and for every other model this value must be a space character.
* The last character is used for padding and must be a space character.

For example; `'GD  '` represents a DMG of an unspecified revision, `'S   '` represents some model of the SGB family, and `CCE ` represent a CGB using CPU revision E.

| Offset | Content                                                |
|--------|--------------------------------------------------------|
| 0x008  | The value of the PC register                           |
| 0x00A  | The value of the AF register                           |
| 0x00C  | The value of the BC register                           |
| 0x00E  | The value of the DE register                           |
| 0x010  | The value of the HL register                           |
| 0x012  | The value of the SP register                           |
| 0x014  | The value of IME (0 or 1)                              |
| 0x015  | The value of the IE register                           |
| 0x016  | Execution state (0 = running; 1 = halted; 2 = stopped) |
| 0x017  | The values of every memory-mapped register (128 bytes) |
| 0x097  | The contents of HRAM (127 bytes)                       |

The values of memory-mapped registers should be written 'as-is' to memory as if the actual ROM wrote them, with the following exceptions and note:
* Unused registers have Don't-Care values which should be ignored
* Unused register bits have Don't-Care values which should be ignored
* The value of KEY0 (FF4C) must be valid as it determines CGB mode
* Sprite priority is derived from KEY0 (FF4C) instead of OPRI (FF6C) because OPRI can be modified after booting, but only the value of OPRI during boot ROM execution takes effect
* BANK (FF50) should be 0 if the boot ROM is still mapped, and 1 otherwise, and must be valid.
* Implementations should not start a serial transfer when writing the value of SB
* Similarly, no value of NRx4 should trigger a sound pulse on save state load
* And similarly again, implementations should not trigger DMA transfers when writing the values of DMA or HDMA5
* The value store for DIV will be used to set the internal divisor to `DIV << 8`
* Implementation should apply care when ordering the write operations (For example, writes to NR52 must come before writes to the other APU registers)

|Offset | Content                                                  |
|--------|--------------------------------------------------------|
| 0x116  | The size of RAM (32-bit integer)                       |
| 0x11A  | The offset of RAM from file start (32-bit integer)     |
| 0x11E  | The size of VRAM (32-bit integer)                      |
| 0x122  | The offset of VRAM from file start (32-bit integer)    |
| 0x126  | The size of MBC RAM (32-bit integer)                   |
| 0x12A  | The offset of MBC RAM from file start (32-bit integer) |

The contents of large RAM sizes are stored outside of BESS structure so data from an implementation's native save state format can be reused. The offsets are absolute offsets from the save state file's start.


#### NAME block

The NAME block uses the `'NAME'` identifier, and is an optional block that contains the name of the emulator that created this save state. While optional, it is highly recommended to be included in every implementation – it allows the user to know which emulator and version is compatible with the native save state format contained in this file. When used, this block should come first.

The length of the NAME block is variable, and it only contains the name and version of the originating emulator in ASCII.

#### OAM block

The OAM block uses the `'OAM '` identifier, and is a required block that contains the data of OAM. This block length can be either 160 or 256. When 256 bytes of data are used, the addition bytes are used to set the values for the additional OAM range at `0xFEA0-0xFEFF`. Implementation that do not emulate that extra range are free to ignore the excess bytes.

#### PALS block

The PALS block uses the `'PALS'` identifier, and is an optional block that is only used for GBC-compatible models. The length of this block is 0x80 bytes and it contains the contents of background palette RAM (0x40 bytes) followed by the contents of object palette RAM (another 0x40 bytes).

#### MBC block

The MBC block uses the `'MBC '` identifier, and is an optional block that is only used when saving states of ROMs that use an MBC. The length of this block is variable and must be divisible by 3.

This block contains an MBC-specific number of 3-byte-long pairs that represent the values of each MBC register. For example, for MBC5 the contents would look like:

| Offset | Content                               |
|--------|---------------------------------------|
| 0x0    | The value 0x0000 as a 16-bit integer  |
| 0x2    | 0x0A if RAM is enabled, 0 otherwise   |
| 0x3    | The value 0x2000 as a 16-bit integer  |
| 0x5    | The lower 8 bits of the ROM bank      |
| 0x6    | The value 0x3000 as a 16-bit integer  |
| 0x8    | The bit 9 of the ROM bank             |
| 0x9    | The value 0x4000 as a 16-bit integer  |
| 0xB    | The current RAM bank                  |

An implementation should parse this block as a series of writes to be made. Values outside the `0x0000-0x7FFF` range are not allowed.

#### RTC block
The RTC block uses the `'RTC '` identifier, and is an optional block that is used while emulating an MBC3 an RTC. The contents of this block are identical to 64-bit RTC saves from VBA, which are also used by SameBoy and different emulators such as BGB.

The length of this block is 0x30 bytes long and it follows the following structure:

|Offset  | Content                                                                |
|--------|------------------------------------------------------------------------|
| 0x00   | Current seconds (1 byte), followed by 3 bytes of padding               |
| 0x04   | Current minutes (1 byte), followed by 3 bytes of padding               |
| 0x08   | Current hours (1 byte), followed by 3 bytes of padding                 |
| 0x0C   | Current days (1 byte), followed by 3 bytes of padding                  |
| 0x10   | Current high/overflow/running (1 byte), followed by 3 bytes of padding |
| 0x14   | Latched seconds (1 byte), followed by 3 bytes of padding               |
| 0x18   | Latched minutes (1 byte), followed by 3 bytes of padding               |
| 0x1C   | Latched hours (1 byte), followed by 3 bytes of padding                 |
| 0x20   | Latched days (1 byte), followed by 3 bytes of padding                  |
| 0x24   | Latched high/overflow/running (1 byte), followed by 3 bytes of padding |
| 0x28   | UNIX timestamp at the time of the save state (64-bit)                  |

#### SGB block

The SGB block uses the `'SGB '` identifier, and is an optional block that is only used while emulating an SGB or SGB2 *and* SGB commands enabled. Implementations must not save this block on other models or when SGB commands are disabled, and should assume SGB commands are disabled if this block is missing.

The length of this block is 0x39 bytes and it follows the following structure:

|Offset  | Content                                                                                          |
|--------|--------------------------------------------------------------------------------------------------|
| 0x00   | The size of the border tile data (=0x2000, 32-bit integer)                                       |
| 0x04   | The offset of the border tile data (SNES tile format, 32-bit integer)                            |
| 0x08   | The size of the border tilemap (=0x800, 32-bit integer)                                          |
| 0x0C   | The offset of the border tilemap (LE 16-bit sequences, 32-bit integer)                           |
| 0x10   | The size of the border palettes (=0x80, 32-bit integer)                                          |
| 0x14   | The offset of the border palettes (LE 16-bit sequences, 32-bit integer)                          |
| 0x18   | The size of active colorization palettes (=0x20, 32-bit integer)                                 |
| 0x1C   | The offset of the active colorization palettes (LE 16-bit sequences, 32-bit integer)             |
| 0x20   | The size of RAM colorization palettes (=0x1000, 32-bit integer)                                  |
| 0x24   | The offset of the RAM colorization palettes (LE 16-bit sequences, 32-bit integer)                |
| 0x28   | The size of the attribute map (=0x168, 32-bit integer)                                           |
| 0x2C   | The offset of the attribute map (32-bit integer)                                                 |
| 0x30   | The size of the attribute files (=0xfe0, 32-bit integer)                                         |
| 0x34   | The offset of the attribute files (32-bit integer)                                               |
| 0x38   | Multiplayer status (1 byte); high nibble is player count, low nibble is current player (0-based) |

If only some of the size-offset pairs are available (for example, partial HLE SGB implementation), missing fields are allowed to have 0 as their size, and implementations are expected to fall back to a sane default.

#### END block
The END block uses the `'END '` identifier, and is a required block that marks the end of BESS data. Naturally, it must be the last block.

