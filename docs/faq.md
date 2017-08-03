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

This game is running too fast!
------------------------------

TODO

games run in fast-forward if there's no audio device
https://board.byuu.org/viewtopic.php?p=44138#p44138

Can I run higan on a phone or tablet?
-------------------------------------
