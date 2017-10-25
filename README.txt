
higan - "Now you're playing with fire!"
=======================================

higan is a multi-system emulator that began development on October 14th, 2004.
It currently plays games for the following systems:

   * Nintendo Famicom (NES), Super Famicom (SNES)
   * Nintendo Game Boy, Game Boy Color + Game Boy Advance
   * Sega Master System, Game Gear + Mega Drive (Genesis)
   * NEC PC Engine (TurboGrafx) + SuperGrafx
   * Bandai WonderSwan + WonderSwan Color


Supported Systems
-----------------

   * FreeBSD 10+
   * Windows 7, 8, 10
   * Linux 3.2+
   * OS X 10.7 Lion or above

You'll need a fast CPU, with clock speed being vastly more important.
Dual-core is perfectly adequate, since higan doesn't make significant use
of more than one core.

For Intel, you're looking at a 3.5 GHz Haswell architecture or better,
whilst AMD users will want a similarly specced Ryzen build.


Controller Setup
----------------

First, you'll want to configure your controllers. Choose Settings -> Input and
pick the system you'd like to configure. If you have two players, or a special
game (eg. Mario Paint), you can pick the controller port and device here.

To assign inputs, double click the name and press the stick or button on your
controller. You can have multiple assignments, for example both keyboard and
joypad. The Erase button clears the assignments for one input; Reset clears
them all in one go.

   (Normally you only need to do this once. But because of how USB works,
   it's impossible to tell identical controllers apart. This means if you move
   one to another port, it counts as a new device, and you'll have to reassign
   the buttons or move it back.)


Loading Games
-------------

After this you can go to Library -> Load ROM File and select a game. higan
adds it to your library, and it should start immediately. (Game Boy Advance
titles need one more step; please see the FAQ below.)

To add games en masse, you can use Library -> Import ROM Files. This opens
icarus, where you can choose a folder of ROMs, then select the ones you want
to import.

In both cases, if you choose a system under the Library submenus, all games
added will show up in a file browser under the Emulation folder in your user
profile. The path can be changed under Settings -> Advanced if desired.


Controller Ports
----------------

If you're emulating a console, you need to plug the controllers in, since
there's no connection by default.

Usually this means selecting eg. Super Famicom -> Controller Port 1 -> Gamepad,
for example. However, some games require other peripherals like the SNES Mouse.


Troubleshooting & FAQ
---------------------

Q: What's the Library?

A: higan loads folders containing all the files needed to run the game.
   Odds are you have PC games and music albums organised the same way.
   This does mean that to play the games, you have to import them first.
   
   If you're familiar with iTunes or Steam, you already know how this works!


Q: Importing vs. loading? What's the difference?

A: The "Library -> Load ROM File" menu is a shortcut. It adds the game to your
   library, then opens it without the manual import process.
   
   However, if you have lots of games to add at once, you'll want
   "Import ROM Files" instead.


Q: Why's higan say I'm missing a file ("Game Boy Advance.sys/bios.rom")?

A: This is the ROM for the startup screen you see when you switch on the
   Game Boy Advance. Games require it to run, but like other ROMs, it's
   copyrighted and therefore not provided with higan.
   
   Having acquired a copy, you'll have to drop it in the requested folder,
   then rename it to bios.rom.


Q: Where are the games imported? Where did all my save files go?

A: Check the path under Settings -> Advanced. On Windows it'll probably be
   something like C:\Users\<name>\Emulation, organised by system. The saves
   are typically named save.ram.


Q: Where can I find the settings?

A: There's a few possible locations for settings.bml.
   
   1) In the same folder as the higan executable.
   
   2) In the older location if you previously installed higan:
      
      C:\Users\<name>\AppData\Roaming\higan            (Windows)
      /home/<name>/.config/higan                       (BSD, Linux)
   
   3) In the new location (created if the others aren't found):
      
      C:\Users\<name>\AppData\Local\higan              (Windows)
      /home/<name>/.local/share/higan                  (BSD, Linux)
      /Users/<name>/Library/Application Support/higan  (Mac)
   
   higan checks these in order, so you can make a portable install if you like.
   
      (macOS normally hides the Library folder. To open it, switch to Finder,
      hold the Option key and select Go -> Library from the menu.)


Q: I set up my gamepads, but they don't work!

A: Try configuring the ports found in the system menu (eg.
   Super Famicom -> Controller Port 1 -> Gamepad). Like a real console,
   fresh higan installs come without any controllers plugged in.


Q: I upgraded higan, why do I get a black screen? What's "Ignore Manifests?"

A: higan looks at a file called "manifest.bml" to get the information needed
   to run each game. However, the format has changed over time, making older
   manifests incompatible with newer higan releases.
   
   If you tick "Settings -> Advanced -> Ignore Manifests," you might find this
   resolves the problem. This can be useful for developers and testers.
   However, it breaks a few titles that require manifests to work!
   
   Should you find yourself in this situation, consider removing manifest.bml.
   
      (By default, no manifests are created; higan looks at the files in the
      game folder, and with the help of a database, tries to regenerate the
      correct one each time you load the game.)


Q: I have "Ignore Manifests" ticked, but the game won't run?

A: A few games have especially quirky setups that require manifests for
   the time being, so you'll need to untick this option:
   
      * Far East of Eden: Tengai Makyou Zero  (English translation only)
      * Campus Challenge '92
      * PowerFest '94


Q: Why's the audio lag, stutter, distort, or sound robotic?

A: If you have an Atom, certain Celeron models, or an older AMD processor
   (or even an especially old Intel such as a Core 2 Duo)... then these aren't
   fast enough, sorry. :(
   
   Try going into the Settings -> Advanced menu, then pick a different audio
   driver and restart higan. WASAPI can be fussy on some devices.
   
   Select Settings -> Audio and experiment with the latency. Larger values
   should be more reliable, with the downside of laggier game controls.
   
   Occasionally software that hooks into the system or other apps, for example
   mouse settings panels, can cause lag and other problems.
   
   Because higan is CPU-intensive and single-threaded, it can interact badly
   with video capture which is yet another burden on the system. If you're
   trying to stream or broadcast, and you have Windows 7, consider disabling
   DWM. Also, look up how to configure hardware encoding (eg. QuickSync).


Q: Can I get smoother video?

A: Try Settings -> Video -> Exclusive mode, then switch to fullscreen. This
   currently requires the Direct3D video driver under Settings -> Advanced
   in order to work.
   
      (Exclusive fullscreen is pretty experimental at the moment.
      There are cases where it fails badly, so save your work!)

   Exclusive mode will normally yield what's known as "tearing." If this
   bothers you, there's an alternative... albeit one with serious gotchas,
   which is why it's hidden away.
   
   Close higan, then open up settings.bml and look for the following:
   
      Video
        Driver:Direct3D
        Synchronize:false
        ...
   
   Change false to true, save the file, then start higan and untick
   Settings -> Synchronize Audio.
   
   Keep in mind that this setting can and will reduce sound quality, as GPUs
   and sound cards in modern PCs generally are not synchronised with each
   other. The second big consideration is that your refresh rate needs to
   match the game.
   
   PAL and NTSC titles run at 50 Hz and 60 Hz, respectively. This applies to
   all console systems. Of the handhelds: Game Boy, Game Boy Color and
   Game Boy Advance run at 60 Hz, while WonderSwan runs at 75 Hz.
   
   This means you'll need a monitor that supports these frequencies, set to
   the appropriate display mode. Not all of them do. If your refresh rate
   doesn't match, games will run at the wrong speed.


Online Resources
----------------

Official homepage + forum:

   https://byuu.org/emulation/higan
   https://board.byuu.org/viewforum.php?f=4

Unoffical source code repository + documentation:

   https://gitlab.com/higan/higan
   https://higan.readthedocs.io

Info on game folders and firmware:

   https://byuu.org/emulation/higan/game-paks
   https://byuu.org/emulation/higan/firmware

Donations:

   https://patreon.com/byuu

Commercial use:

   https://byuu.org/emulation/higan/licensing


Credits
-------

Original author:

   byuu

We'd like to acknowledge many invaluable contributions made to higan
by the following individuals:

   Andreas Naive         Hendricks266          Overload
   Ange Albertini        hex_usr               p4plus2
   anomie                jchadwick             quequotion
   AWJ                   Jonas Quinn           RedDwarf
   Bisqwit               kode54                Richard Bannister
   blargg                krom                  Ryphecha
   Ćukasz Krawczyk       Lioncash              segher
   Cydrak                Lord Nightmare        Sintendo
   Danish                lowkey                SuperMikeMan
   DMV27                 MerryMage             tetsuo55
   Dr. Decapitator       Matthew Callis        TmEE
   endrift               mightymo              TRAC
   Fatbag                Nach                  wareya
   FitzRoy               ncbncb                zones
   gekkio                neviksti
   GIGO                  OV2

It's been a long, wild ride... apologies to anyone we've missed!

For more information, please see:

   https://board.byuu.org/viewtopic.php?f=4&t=1631&p=41575#p41575


License
-------

higan is provided under the GNU General Public License, version 3.
However, certain libraries may be used under the more permissive ISC license.
Please see LICENSE.txt for details.
