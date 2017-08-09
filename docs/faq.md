I see "tearing" when a game scrolls. How can I enable vsync?
------------------------------------------------------------

higan supports synchronizing video output
to the display's vertical-synchronization (or "vsync") signal,
but the option is hidden
because it often causes more problems than it solves
(see the next question).

To enable video synchronization:

  - Open the higan's configuration file, `settings.bml`
      - On Windows, look in `%LOCALAPPDATA%\higan`
        or beside `higan.exe`
      - On Linux, look in `~/.local/share/higan`
  - Open it in your favourite text editor
    (Windows Notepad will corrupt the file,
    use WordPad if you don't have anything better)
  - Find the line that says "Video"
  - Somewhere below that, find an indented line
    that says "Synchronize:false".
  - Change "false" to "true"
  - Save your changes to `settings.bml`
    and restart higan

Why is video synchronization a problem for higan?
-------------------------------------------------

**The short version:**
Turning on video synchronization
cleans up video tearing,
turning on audio synchronization
cleans up audio glitches,
but turning on both
makes audio glitches worse.

**The long version:**
Enabling video synchronization
locks the frame-rate of the emulated console
to the frame-rate of your computer's display.
If your display's refresh rate exactly matches
the emulated console's,
games play at the correct speed
and everything's fine.

However,
modern 60Hz displays do not always match
the emulated console's refresh rate:

  - The Super Famicom usually runs a little faster than 60Hz
  - the PAL variants of most consoles run at 50Hz
  - the WonderSwan runs at 75Hz
  - While the Game Boy does run its LCD at 60Hz
    it can turn it off and on at any time,
    requiring emulation to pause
    until it can get back in sync
    with the computer display.

Because of these frame-rate differences,
enabling video synchronization
can force games to run
faster or slower than intended.

The consoles that higan emulates
produce video frames and audio samples at a particular rate.
If video synchronization causes
the emulated console to run, say, 5% faster than intended,
that means audio samples are also being produced 5% faster.
You might not notice the changed game speed,
but you'll almost certainly notice
the game's audio glitching constantly
as your sound card tries to keep up.

Enabling
[audio synchronization](interface/higan.md#the-settings-menu)
normally fixes this kind of audio glitching,
but with video synchronization it makes things worse:
audio is likely to glitch
while higan waits for a video frame to be shown,
and video is likely to stutter
while higan waits for an audio buffer to complete.

Games run too fast
------------------

higan runs as fast as it can,
but it will pause and wait
for the audio and video drivers to catch up
if [Synchronize Audio](interface/higan.md#the-settings-menu)
and [video synchronization][vsync]
are enabled, respectively.
If games are running way too fast, here's some things to check:

  - Make sure "Synchronize Audio" is ticked in
    [the Settings menu](interface/higan.md#the-settings-menu)
  - Make sure the Audio driver is not set to "None"
    in [the Advanced settings](interface/higan-config.md#advanced)
    (remember to restart higan if you change driver settings)
  - Make sure your computer has speakers or headphones connected
    (some computers disable all audio if no ouputs are available)
      - If you want the game to be silent,
        tick "Mute Audio" in
        [the Settings menu](interface/higan.md#the-settings-menu)

[vsync]: #i-see-tearing-when-a-game-scrolls-how-can-i-enable-vsync

Games run too slow
------------------

Of all the consoles higan can emulate,
higan's Super Famicom emulation
is the most resource intensive.
Full-speed emulation for the Super Famicom base unit
requires an Intel Core 2 Duo (or AMD equivalent),
full-speed for games with the SuperFX chip
requires an Intel Ivy Bridge (or equivalent),
full-speed for the wireframe animations in Mega Man X2
requires an even faster computer.
Low-power CPUs like ARM chips,
or Intel Atom and Celeron CPUS
generally aren't fast enough to emulate the Super Famicom with higan,
although other emulated consoles may work.

If your computer meets the general speed requirements
but games run too slowly,
try choosing a different
[audio driver](interface/higan-config.md#advanced),
since that's usually what drives higan's timing.

On some computers,
the operating system's power-management system
may be confused by higan's unusual pattern of CPU usage
(it runs furiously to calculate the next video frame
and the next few milliseconds of audio,
then stops dead as it waits for output to complete).
If holding down
the [fast forward hotkey](interface/higan-config.md#hotkeys)
runs too fast but the game normally runs too slow,
try disabling "power saver" mode
or enabling "performance" mode.

On Linux specifically,
if your desktop environment doesn't provide a way
to change the CPU usage governor,
you can temporarily force your CPUs to run at full-speed
by running the following command in a terminal:

    echo "performance" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

To put things back to normal,
reboot the computer, or run this command instead:

    echo "ondemand" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

Why is higan so much slower than other emulators?
-------------------------------------------------

The consoles higan emulates
generally contain different devices
(say, a CPU, an audio chip and a video chip)
that do their own thing,
but can interrupt each other at any time.
Games can and do depend on timing details like
"when the audio chip is done playing this sound,
it will interrupt the CPU at exactly the right time
for the CPU to fiddle with the video chip".
higan is therefore very cautious about timing:
while it's emulating the audio chip (for example),
at every point the emulated CPU *might* interrupt
the emulated audio chip,
higan switches to emulating the CPU up to the same point
to find out whether the CPU *will* interrupt it.

In this way,
higan is a little bit like
an office-worker trying to do the jobs of three other people
by running from desk to desk,
sending the same emails
that those three people would send to each other,
leaving themselves a note at each desk to remind themselves
where they were up to when they come back.
Although this constant switching
is slow and inefficient,
higan does it
to ensure the emulated console
always matches the behaviour
of the original.

Other emulators,
especially ones that are popular on phones and tablets,
typically work differently.
Although emulated devices *can* interrupt each other at any time,
usually they don't,
and often the only exceptions are a handful of unpopular games.
So it's pretty safe
(and much, much more efficient)
to let certain devices run "ahead" of the others.
For example,
letting the video chip render an entire scanline at a time
instead of working pixel-by-pixel.
The downside is that some games don't work properly
in these emulators,
with game-breaking bugs,
minor visual glitches,
or anything in between.

Some emulators try to recognise
when the user loads a specific problematic game,
and will adjust the shortcuts the emulator takes
so that the game runs correctly.
This is a good way to increase game compatibility
without losing efficiency,
but it means the emulator does not accurately reflect
how the original console worked,
and it requires the emulator authors to diagnose and work around
each individual supported game,
instead of having one emulator that works for everything.

Why can't higan use multiple CPU cores?
---------------------------------------

These days,
most computers contain multiple CPU cores,
allowing them to run different programs,
or different parts of the same program
at the same time.
Since higan requires high CPU performance,
sometimes people suggest that it should split its work
into multiple threads
so it can run across multiple cores
instead of stressing one core while the others lie idle.
After all,
the original consoles higan emulates
had separate devices running independently,
so why not run each emulated device in a separate thread?

The big problem with this approach is timing.
The devices in a physical Super Famicom (for example)
run at whatever speed they run at,
and talk to each other whenever they feel like.
Because every Super Famicom uses the exact same components,
every Super Famicom runs compatible games at the exact same speed,
and games can blindly assume that
when operation X is complete on device A,
device B will have finished operation Y
and be ready to do something new.
Meanwhile, higan's emulated components
take an unpredictable amount of time to do their work,
so without deliberate synchronization
things would break almost immediately.

It's not practical to make higan's emulated devices
do their work in exactly the same amount of time
as their hardware counterparts.
The problem is forty years of technology
designed to make programs run as fast as possible:
optimizing compilers and superscalar, out-of-order CPU architectures
change programs to make them faster,
speeding up some programs more than others
in ways that are very difficult to understand and predict.
Even if higan's emulated devices
ran at the exact, correct speed
on one particular computer,
they'd still run differently on any other computer,
or with a smarter compiler,
or with a smarter CPU.

Since higan needs its emulated components
to run at particular speeds,
and they won't run at those speeds naturally,
it must force them manually.
An emulated device runs for a little while,
then all the others are run until they catch up.
It's this careful management,
regular stopping and starting,
that makes higan slow,
not the actual emulation of each device,
and so it doesn't make sense
for higan to be multi-threaded.
