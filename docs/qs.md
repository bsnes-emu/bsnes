If this is your first time using higan,
welcome!
Here's a brief guide to getting started:

Install higan
-------------

On Windows,
this should be as easy as downloading
the latest release archive from
[the higan homepage](https://byuu.org/emulation/higan/)
and extracting it.
[More information...](install/windows.md)

On Linux,
if your distribution doesn't already include the latest version,
you'll need to compile it yourself — but don't worry,
it's not too complex.
[More information...](install/linux.md)

Configuring inputs
------------------

Once higan's installed,
start it up.
Open the "Settings" menu,
and choose "Input ..."
to open [the Input settings](interface/higan-settings.md#input).

higan supports a *lot* of different controllers
for a lot of different consoles,
so you don't need to configure everything immediately.
For most consoles,
it's enough to just configure
whatever shows up in the list
when you choose that console.
For example,
when you pick the Super Famicom,
you're shown the inputs
for a Gamepad
in Controller Port 1.

Once you've configured the controls
just the way you want them,
higan will remember them
— you don't need to re-configure them
every time you start higan,
or every time you load a game.

Load a game
-----------

From
[the Library menu](interface/higan.md#the-library-menu),
choose "Load ROM File ..."
to open [a filesystem browser](interface/common.md#the-filesystem-browser),
and choose the game you want to play.
It will be added to
[the Game Library](concepts/game-library.md),
and it will start running immediately.

In the future,
if you want to play this game again,
you can choose "Load ROM File ..." as you did before,
or you can choose the appropriate console name
from the Library menu,
which will list all the games for that console
in the Game Library.

Connect a controller
--------------------

Once the game is running,
a new menu will appear
named after the appropriate console.
For example,
if you're playing Super Mario World,
that's a Super Famicom game
so there will be a Super Famicom menu.

Open [the console menu](interface/higan.md#the-console-menu),
and if there is a submenu for a controller port,
make sure the port is using
the controller you [set up previously](#configuring-inputs).
If you configured inputs for a Gamepad in Controller Port 1,
the Controller Port 1 submenu
should be set to Gamepad.
Like a real console,
higan's controller ports
start out with nothing plugged in,
so if the game you're playing needs a gamepad connected,
you'll have to connect it!

This doesn't apply to handheld consoles
like the Game Boy and WonderSwan,
since the "controller" is always connected.
This *does* apply to the Famicom,
even though the Famicom's controllers are hard-wired,
because higan uses the name "Famicom"
to refer to Nintendo's 8-bit home console
in all territories,
including the Nintendo Entertainment System.
