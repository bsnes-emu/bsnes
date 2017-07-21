higan maintains a "game library"
containing all the games you've played.

  - In Windows,
    the default location of
    the game library is the `Emulation` folder
    inside your profile folder
    (To find your profile folder,
    press `Win+R` to open the Run dialog,
    then type `%USERPROFILE%` and press Enter).
  - In Linux,
    the default location of
    the game library is the `Emulation` directory
    inside your home directory.
  - On all platforms,
    the game library location can be configured.
    See [Moving the Game Library](#moving-the-game-library)
    below.

Inside the library directory there is a subdirectory for each system,
and inside each system directory are the game folders
for each imported game.

For more information about game folders,
see [Why game folders?](#why-game-folders)
and [What's in a game folder?](#whats-in-a-game-folder)
below.

Importing and playing regular games
-----------------------------------

icarus supports importing games
in the most commonly-used formats
for each supported console,
and also those same formats inside `.zip` files.
More advanced compression formats
like RAR or 7-zip are not supported.

For most games
that do not use special chips or co-processors,
importing a game is straight-forward.
From [the Library menu](#the-library-menu)
choose "Load ROM File ..."
to open [a filesystem browser](#the-filesystem-browser),
choose the game you want to play,
and it will be imported into the library and loaded.

To play the game again
select the console the game runs on from
[the Library menu](#the-library-menu)
to open another [filesystem browser](#the-filesystem-browser)
that lists all the previously-imported games for that platform.

Importing and playing games with co-processor firmware
------------------------------------------------------

Many games included extra chips inside the game cartridge,
to provide enhanced capabilities of one kind or another.
Sometimes,
those extra chips were separate CPUs
running their own separate firmware,
and for those cases
higan requires a copy of the co-processor firmware
as well as the actual game.
Unfortunately,
like games themselves,
co-processor firmware cannot legally be distributed,
so you'll need to obtain
copies of the relevant firmware data
yourself.

To import a game that requires co-processor firmware,
you must copy the required firmware files
beside the game you want to import.
For example,
if you want to import Megaman X2,
which is stored in the file `mmx2.sfc`,
the file `cx4.data.rom`
must be placed in the same folder
for the import to succeed.

Wikipedia [lists which Super Famicom games use which co-processors][wpec],
although not all co-processors require separate firmware.
Once you've figured out which co-processor
(if any)
is used by the game you want to import,
here's the firmware files you'll need:

[wpec]: https://en.wikipedia.org/wiki/List_of_Super_NES_enhancement_chips#List_of_Super_NES_games_that_use_enhancement_chips

<table>
    <thead>
        <tr>
            <th>Co-processor</th>
            <th>Filename</th>
            <th>Size (bytes)</th>
            <th>SHA256</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th scope="row">CX4</th>
            <td><code>cx4.data.rom</code></td>
            <td>3072</td>
            <td><code>ae8d4d1961b93421ff00b3caa1d0f0ce7783e749772a3369c36b3dbf0d37ef18</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP1/1A<br><sup>See Note 1</sup></th>
            <td><code>dsp1.data.rom</code></td>
            <td>2048</td>
            <td><code>0b5da6533e55852ee8fc397977ec5576c5b9f1fb2e05656d8f87123a121b076e</code></td>
        </tr>
        <tr>
            <td><code>dsp1.program.rom</code></td>
            <td>6144</td>
            <td><code>269584b347a22953a2989494c850a7c1c027f4ca5add517a60e0c7d8833d0fac</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP1B<br><sup>See Note 2</sup></th>
            <td><code>dsp1b.data.rom</code></td>
            <td>2048</td>
            <td><code>8546cbac530830446bb8a277f6b139d4ad64d650bdbac7e4e150e2f095665049</code></td>
        </tr>
        <tr>
            <td><code>dsp1b.program.rom</code></td>
            <td>6144</td>
            <td><code>2eccb54a8f89374911f7e2db48f1b4cde855655e28103f7bda2982a5b418a187</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP2</th>
            <td><code>dsp2.data.rom</code></td>
            <td>2048</td>
            <td><code>3beef9bffdc1e84c9f99f3301d8bd3e520d2e62909a995320f9faeae8f46ec11</code></td>
        </tr>
        <tr>
            <td><code>dsp2.program.rom</code></td>
            <td>6144</td>
            <td><code>62a2ef8d2d7db638f4ec0fbcebf0e5bf18a75ee95be06e885d9519a10487f0da</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">DSP3</th>
            <td><code>dsp3.data.rom</code></td>
            <td>2048</td>
            <td><code>7fe51796e9c97fee1fa2aab40592b7c78997f67dd00333e69d0f79a12f3cb69f</code></td>
        </tr>
        <tr>
            <td><code>dsp3.program.rom</code></td>
            <td>6144</td>
            <td><code>aea7b622e7c1de346cb15d16afcbedf92b6798507e179f83ed2a4cff40d0e663</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">DSP4</th>
            <td><code>dsp4.data.rom</code></td>
            <td>2048</td>
            <td><code>ef3ffb4256dd896a60213269b4e2d3bdd120c97e2fd623bddabbf43c2be422af</code></td>
        </tr>
        <tr>
            <td><code>dsp4.program.rom</code></td>
            <td>6144</td>
            <td><code>89b1826e6038be3a0ea0f923e85d589ff6f02dc1a1819fb2ec8c0cea5b333dcd</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST010</th>
            <td><code>st010.data.rom</code></td>
            <td>4096</td>
            <td><code>dc7056a51b53993d7a8ba5bacf9501f785d2fce5e5be748e9ff2737c5938d4a5</code></td>
        </tr>
        <tr>
            <td><code>st010.program.rom</code></td>
            <td>49152</td>
            <td><code>2c1f74bb5f466d81c64c326e71ac054489efe1abc9a5d6f91aac7747f2ddab67</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST011</th>
            <td><code>st011.data.rom</code></td>
            <td>4096</td>
            <td><code>b5377d1bebe8adc507a024b6e2b9b8fdf4877e451da84fbad05dff4e4a70311e</code></td>
        </tr>
        <tr>
            <td><code>st011.program.rom</code></td>
            <td>49152</td>
            <td><code>d90a5cda380e81cb9ba11a9da7539b173c49b31bedc7a3ac9c3c8b3f97e89e14</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST018</th>
            <td><code>st018.data.rom</code></td>
            <td>32768</td>
            <td><code>b5377d1bebe8adc507a024b6e2b9b8fdf4877e451da84fbad05dff4e4a70311e</code></td>
        </tr>
        <tr>
            <td><code>st018.program.rom</code></td>
            <td>131072</td>
            <td><code>d90a5cda380e81cb9ba11a9da7539b173c49b31bedc7a3ac9c3c8b3f97e89e14</code></td>
        </tr>
    </tbody>
</table>

**Note 1:**
The DSP1 and DSP1A are physically different,
but the firmware inside is identical.

**Note 2:**
The DSP1B is very similar to the DSP1A,
but has some bugs fixed.
Note that icarus' heuristics cannot distinguish between
a game that uses DSP1
and one that uses DSP1B,
so if it cannot find your game in its manifest database,
it will assume it uses DSP1B.
Many games work just as well with either DSP1 or DSP1B,
but Pilotwings is a notable exception.

If you try to import a game
using the "Import ROM Files ..." option
in [the Library menu](#the-library-menu)
(or using icarus directly)
but do not have the required firmware files
in the correct place,
a window will appear saying
"Import completed, but with 1 errors. View log?"
(or howevery many games were lacking the correct firmware).
If you press "Yes",
a new window will appear listing the games that couldn't be imported,
and at least one firmware file that was missing or incorrect, like this:

> [smk.zip] firmware (dsp1b.program.rom) missing or invalid

If you try to import a game
using the "Load ROM File ..." option
in [the Library menu](#the-library-menu)
but do not have the required firmware files
in the correct place,
nothing will happen,
and higan will just sit there
with "No cartridge loaded" in
[the status bar](#the-status-bar).

Once a game with co-processor firmware is imported,
you can play it just like any [regular game](#importing-and-playing-regular-games).

Importing and playing Satellaview games
---------------------------------------

The [Satellaview][wpbsx]
was a satellite modem peripheral
released for the Super Famicom in Japan.
As well as the actual modem
(designed to sit underneath the Super Famicom),
it also included a cartridge
with software to control the modem,
browse online services,
and download games and data.
This control cartridge was called
*BS-X Sore wa Namae o Nusumareta Machi no Monogatari*,
which translates as
*BS-X The Story of The Town Whose Name Was Stolen*.

[wpbsx]: https://en.wikipedia.org/wiki/Satellaview

The control cartridge had a slot that accepted
rewritable "memory paks",
so that people could store the games and data they downloaded.
A small number of games that did not use the Satellaview modem
also had a memory pak slot,
so the game's publishers could
publish extra content for the game
via the Satellaview service
after the game's release.
For the benefit of people who didn't own a Satellaview
some read-only memory paks
were sold in retail stores
containing extra content for specific games.

Importing a game that has a slot for a memory pak
is just like [importing a regular game](#importing-and-playing-regular-games).

Importing a memory pak is like importing a regular game,
but the name of the memory pak file *must* end in `.bs`
(if it's in a `.zip` file,
that's OK,
but the name *inside* the `.zip` file
must end in `.bs`)
in order for it to be successfully imported.
Sometimes memory pak filenames end in `(BSROM).sfc`,
which will make higan try to import them as
regular Super Famicom games,
and fail miserably.
Rename the file and it should work beautifully.

Playing a game that has a slot for a memory pak
is just like playing a regular game,
but after you have selected which game you want to play
higan will open another
[filesystem browser](#the-filesystem-browser)
to let you pick which previously-imported memory pak
you want to insert into the game.
If you press "Cancel" at this point,
the game will load without any cartridge in its memory pak slot.

If you load the control cartridge into higan,
make sure the emulated Satellaview
is connected to the emulated Super Famicom's expansion port
by going to the "Super Famicom" menu,
selecting the "Expansion Port" sub-menu,
and choosing "Satellaview".
If the expansion port was previously
configured with a different option,
power-cycle the Super Famicom
(also in the "Super Famicom" menu)
to make sure the control cartridge will find the Satellaview
when it starts up.
Note that higan's Satellaview emulation is not very accurate,
so the control cartridge may not work as it should.

Playing a memory pak on its own doesn't make much sense,
it's not a standalone cartridge.
Play a game with a memory pak slot,
and choose which memory pak you want when higan asks for it.

For more information about the Satellaview service,
a translation patch for the control cartridge
and emulators that do a better job of Satellaview emulation,
see [the BS-X Project](https://bsxproj.superfamicom.org/).

Importing and playing Sufami Turbo games
----------------------------------------

The [Sufami Turbo][wpst]
was a special cartridge released
for the Super Famicom in Japan.
The Sufami Turbo on its own does nothing,
but it has two slots in the top
that accept Sufami Turbo mini-cartridges.
The game in slot A is the one that actually plays,
but some games can make use of additional data
from a game in slot B.

Importing the Sufami Turbo cartridge
is just like [importing a regular game](#importing-and-playing-regular-games).

Importing a mini-cartridge is like importing a regular game,
but the name of the memory pak file *must* end in `.st`
(if it's in a `.zip` file,
that's OK,
but the name *inside* the `.zip` file
must end in `.st`)
in order for it to be successfully imported.
Sometimes memory pak filenames end in `(ST).sfc`,
which will make higan try to import them as
regular Super Famicom games,
and fail miserably.
Rename the file and it should work beautifully.

To play a Sufami Turbo game,
load the Sufami Turbo cartridge like any other game.
higan will open another
[filesystem browser](#the-filesystem-browser)
to let you pick which previously-imported mini-cartridge
you want to insert into slot A.
If you press "Cancel" at this point,
the Sufami Turbo cartridge will boot without anything in slot A,
which just displays an image telling you
to turn off your Super Famicom,
insert a game into slot A,
and try again.
If you chose a cartridge for slot A,
higan will yet open another
filesystem browser
to let you choose a mini-cartridge for slot B.
If you press "Cancel" at this point,
the Sufami Turbo cartridge will boot without anything in slot B.

[wpst]: https://en.wikipedia.org/wiki/Sufami_Turbo

Importing and playing Super Game Boy games
------------------------------------------

The Super Game Boy was a special cartridge
released for the Super Famicom
(and all its regional variants around the world)
that allowed Game Boy games to be played
via the Super Famicom's controllers and video output.
The Super Game Boy 2 was released in Japan,
and had some minor extra features
beyond the original Super Game Boy,
but importing and playing games
works the same way in higan.

The Super Game Boy cartrige includes
the complete hardware of an original
(black-and-white)
Game Boy,
so it needs a boot ROM:

<table>
    <thead>
        <tr>
            <th>Cartridge</th>
            <th>Filename</th>
            <th>Size (bytes)</th>
            <th>SHA256</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th scope="row">SGB</th>
            <td><code>sgb.boot.rom</code></td>
            <td>256</td>
            <td><code>0e4ddff32fc9d1eeaae812a157dd246459b00c9e14f2f61751f661f32361e360</code></td>
        </tr>
        <tr>
            <th scope="row">SGB2</th>
            <td><code>sgb.boot.rom</code></td>
            <td>256</td>
            <td><code>fd243c4fb27008986316ce3df29e9cfbcdc0cd52704970555a8bb76edbec3988</code></td>
        </tr>
    </tbody>
</table>

Yes,
the SGB and SGB2 have different firmware,
but higan expects the same filename for both.

To import the SGB base cartridge,
you must copy the required firmware file
into the same directory.
Then you may import it just like
[a regular game](#importing-and-playing-regular-games).

To play a Game Boy game in Super Game Boy mode,
load the Super Game Boy cartridge like any other game.
higan will open another
[filesystem browser](#the-filesystem-browser)
to let you pick which previously-imported Game Boy game
you want to insert into the Super Game Boy.
If you press "Cancel" at this point,
higan will crash, so don't do that.

Note that only games for the original, black-and-white Game Boy
can be used with the Super Game Boy.
Some games designed for the Game Boy Color
were backward compatible with the original Game Boy
and hence the Super Game Boy;
see [Playing Game Boy Colour games in Game Boy mode][blackcarts]
for details.

[blackcarts]: #playing-game-boy-color-games-in-game-boy-mode

Importing and playing MSU-1 games
---------------------------------

The MSU-1 is a fictional expansion chip
invented by higan's author byuu
for use with Super Famicom games,
designed to allow streaming data and audio.
Although the MSU-1 is not specific
to any particular storage medium,
it gives the Super Famicom similar capabilities
to CD-based add-ons
like the Mega Drive's Mega CD
and the PC Engine's CD-ROM²,
such as CD-quality music and full-motion video.

One thing to be aware of
when importing an MSU-1 game
is that early firmware versions
of the [SD2SNES][sd2snes] programmable cartridge
had a bug that caused MSU-1 music to play too quietly.
Skipping over [the full details][msu1vol],
the short version is this:

  - If offered the choice between "boosted" or non-boosted audio,
    you want the non-boosted version.
  - If an MSU-1 mod for a commercial game offers
    "emulator" and "hardware" versions of the patch file,
    it means the audio tracks are already boosted.
      - Some
        [third](https://www.zeldix.net/t1265-#18320)
        [parties](https://www.zeldix.net/t1339-#19818)
        have created replacement, non-boosted audio tracks
        for the most popular MSU-1 mods.
        If the mod you want to play has a replacement pack,
        use it with the "hardware" version of the patch.
      - Even without access to non-boosted audio tracks,
        it may be that the existing audio is only slightly boosted,
        so try the "hardware" version first, for best quality.
      - If the audio tracks are heavily boosted,
        the "hardware" patch may sound terrible,
        distorting and clipping,
        in which case try the "emulator" patch.

To import an MSU-1 game:

 1. If you have a single, large file
    with the `.msu1` extension,
    that is a pack for use with [Mercurial Magic][mermag],
    which can automatically set up a game folder
    in the correct format.
    Go read Mercurial Magic's documentation
    instead of these instructions.
 2. Otherwise,
    import the Super Famicom ROM with icarus,
    [like a regular game](#importing-and-playing-regular-games).
      - If this is a homebrew game with MSU-1 support,
        there will probably be an ordinary ROM
        whose name ends in `.sfc`,
        which is the file you want to import.
      - If this is a commercial game modded for MSU-1 support,
        there will probably be a patch file
        whose name ends in `.ips` or `.bps`.
        Get a copy of the correct version of the commercial game,
        apply the patch with a tool like [Flips][flips],
        then import the patched file.
      - If there's "hardware" and "emulator" versions of the patch,
        see "One thing to be aware of..." above.
 3. Find the game folder in [the game library](#the-game-library)
    that icarus created when it imported the game.
 4. Copy the MSU-1 data file into the game folder.
      - This should be named `msu1.rom`
      - If there's no file by that name,
        look for a file with a `.msu` extension
        and rename it to `msu1.rom`.
      - If there's no file ending in `.msu` either,
        create an empty file named `msu1.rom`.
 5. Copy the audio tracks into the game folder.
      - If you have to choose between two sets of audio files,
        see "One thing to be aware of..." above.
      - These should be named
        `track-1.pcm`,
        `track-2.pcm`,
        ... `track-9.pcm`,
        `track-10.pcm`,
        etc.
      - If there's no files with those names,
        there should be other numbered `.pcm` files
        that you can rename to match what higan expects.
      - If the `.pcm` files have no numbers in the filenames,
        there maybe a `.bml` or `.xml` file that lists
        which number goes with which file.
      - If there's no `.pcm` files at all,
        that's OK,
        this game probably just doesn't use the audio-playback feature.

Once the game folder is set up,
playing an MSU-1 game is just like
[a regular game](#importing-and-playing-regular-games).

[sd2snes]: https://sd2snes.de/
[flips]: http://www.romhacking.net/utilities/1040/
[msu1vol]: http://blog.qwertymodo.com/2017/07/the-msu-1-volume-fiasco-explained.html
[mermag]: https://github.com/hex-usr/Mercurial-Magic/

Importing and playing patched games
-----------------------------------

The console emulation community
has a long and vibrant history of game modding,
or [ROM hacking][rhdn],
including fan-translations,
new levels for existing games,
and more.
Since distributing the modified versions of existing games
would be copyright infringement,
the changes are typically distributed as "patches",
a file containing a list of modifications to make,
that can be automatically applied by a "patcher" tool
like [Flips][flips].

higan does not support soft-patching,
so if you want to play a patched game in higan,
you will need to use a patcher to apply it yourself,
creating a new, patched copy of the game.

Then you can import and play the patched game just like
[a regular game](#importing-and-playing-regular-games).

[rhdn]: http://www.romhacking.net/

Moving the game library
-----------------------

Moving the game library is a little complicated,
because there's two parts to it:
telling icarus where to put imported games,
and telling higan where to find them.

 1. If necessary,
    create the folder you want higan to use
    as its game library.
 1. Launch icarus,
    then click the "Settings ..." button in the lower-right,
    to open the Settings dialog.
 1. Click the "Change ..." button on the right.
    A [filesystem browser](#the-filesystem-browser) window will appear,
    allowing you to choose
    where imported games will be stored.
 1. Launch higan,
    then from the Settings menu,
    choose "Configuration ..."
    to open [the Configuration dialog](#the-configuration-dialog).
 1. Click the Advanced tab
    then click the "Change ..." button.
    A [filesystem browser](#the-filesystem-browser) will appear,
    allowing you to choose the same directory again.

Why game folders?
-----------------

A game is more than just
the raw data originally encased in a game's ROM chip.
If a game allows you to save your progress,
that information needs to be stored somewhere.
If you use an emulator's [save states](#save-states),
those save states need to be stored somewhere.
If you use Game Genie or Pro Action Replay codes,
information about what codes exist,
what codes are enabled,
and what they do
needs to be stored somewhere.

On the technical side,
a physical game cartridge contains a circuit board
that makes the game data available to the console,
and different games used circuit boards that work differently.
That circuit-layout information needs to be stored somewhere.
Some games included custom processors
to do calculations the base console could not do quickly enough
(like the SuperFX chip used in _StarFox_ for the Super Famicom)
and information about extra chips needs to be stored somewhere.
Some of those custom processors require extra data to work
that's not part of the main game data
(like the DSP chip used in Super Mario Kart for the Super Famicom)
and that data needs to be stored somewhere too.

higan keeps all this game-related information together
in a single place:
a game folder in the higan library.

For a more detailed motivation for game folders,
see [Game Paks on the higan website][gp]

[gp]: https://byuu.org/emulation/higan/game-paks

What's in a game folder?
------------------------

As mentioned [above](#why-game-folders),
a game folder collects all the information relevant
to emulating a particular game.
Not all of the following files
are relevant to every emulated console,
or to every game on a given console,
but they may be relevantunder particular circumstances.

The most important file in a game folder is `manifest.bml`,
which describes how all the other files should be wired together
to create a runnable game cartridge.
However,
the manifest format has occasionally changed
as new emulation details were uncovered
that could not be represented in the old format.
Therefore,
icarus [defaults](#the-icarus-settings-dialog)
to not writing out manifests when it imports games,
and higan [defaults](#the-configuration-dialog)
to ignoring manifests that are present.
Instead,
when higan loads a game,
it will ask icarus to generate a temporary manifest in the latest format,
based on the files present in the game folder
and how they are likely to go together.
You can view this temporary manifest
in [the Manifest Viewer](#the-manifest-viewer).

All the files directly in the game folder
are expected to be useful
to all emulators that support them:

  - `program.rom`:
    For most consoles,
    this contains
    the executable instructions and graphics data
    from the cartridge's ROM chips.
    For the Famicom,
    this contains only the executable instructions.
  - `character.rom`:
    For the Famicom,
    this contains only the graphics data
    from the cartridge's ROM chips.
  - `ines.rom`:
    While other consoles typically include enough hints
    in `program.rom` for icarus to generate a manifest,
    the Famicom does not.
    Famicom games not stored in game folders
    typically include an "iNES header" to store that information,
    which icarus preserves after import as `ines.rom`.
  - `save.ram`:
    Games that include a save feature
    will create this file.
    Note that it is only written to disk
    when higan exits gracefully,
    if higan crashes or is forced to quit,
    in-game saves may be lost.
    Other emulators sometimes call this an "SRAM file",
    even though the same filename is used
    for cartridges that use EEPROM or Flash storage,
    not just battery-backed Static RAM.
  - `rtc.ram`:
    Games that include a calendar or real-time clock
    will create this file.
  - `*.data.rom`, `*.program.rom`:
    Files named like this are usually
    [co-processor firmware](#importing-and-playing-games-with-co-processor-firmware).
  - `msu1.rom`:
    Holds streamable data for
    [the MSU-1](#importing-and-playing-MSU-1-games).
  - `track-*.pcm`:
    Holds streamable audio for
    [the MSU-1](#importing-and-playing-MSU-1-games).

Files that are only useful to higan specifically
are placed in a `higan` subdirectory:

  - `cheats.bml`:
    All information present in
    [the Cheat Editor](#the-cheat-editor)
    is stored here.
  - `states/quick/slot-*.bst`:
    All the save states made to
    [Quick state slots](#quick-states).
  - `states/managed/slot-*.bst`:
    All the save states made with
    [the State Manager](#the-state-manager).
