Installing the GBA BIOS
-----------------------

For most of the systems higan emulates,
the console itself contains (almost) no actual software,
so emulating the system does not require
infringing the copyright of the hardware manufacturer.
However,
the Game Boy Advance is different:
every device contains a standard library of software routines
for [common functions games require][bios],
often called a "BIOS"
by analogy with the Basic Input/Output System
used in IBM PC compatibles.
Although the GBA BIOS is required
in order to emulate GBA games,
it cannot be distributed with higan
for the same legal reasons that commercial games
cannot be distributed with higan,
so you'll need to obtain a copy of the BIOS for yourself.

If you have a real GBA and a flash-cart,
the Internet contains many tools
that will extract the BIOS image so it can be copied
to your desktop computer.
The correct GBA BIOS file is exactly 16,384 bytes long,
and has the SHA256 hash
`fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570`.

Once you have the correct BIOS file:

 1. rename it to `bios.rom`
      - if you're using Windows,
        turn off "hide extensions for known file types"
        so you don't wind up with a file called
        `bios.rom.dat`
        or whatever the file's original extension was.
 2. Copy the file into higan's `Game Boy Advance.sys` folder,
    alongside the `manifest.bml` file that is already there.
      - In Windows,
        find `Game Boy Advance.sys` in the same folder
        as `higan.exe`
      - In Linux,
        find `Game Boy Advance.sys` in
        `~/.local/share/higan/`

**Note:**
If you upgrade this version of higan to a newer version,
make sure the `bios.rom` file
winds up in the `Game Boy Advance.sys` folder
of the new version.

[bios]: http://problemkaputt.de/gbatek.htm#biosfunctions
