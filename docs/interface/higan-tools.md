The Tools window
appears when you choose
one of the items at the bottom of
[the Tools menu](higan.md#the-tools-menu).

The window has a tab for each tool:

The Cheat Editor
----------------

For some consoles,
higan supports applying temporary changes to the code of a running game.
For example,
you could disable the code that registers when the player takes damage,
resulting in an "invulnerability" mode.
Currently,
higan supports cheats for the following consoles:

  - Famicom
  - Super Famicom
  - Game Boy
  - Master System
  - PC Engine
  - Wonder Swan

A cheat code of the format `addr=data`
will cause the emulated console to obtain `data`
whenever it reads from memory address `addr`.
A cheat code of the format `addr=comp?data`
will cause reads from `addr` to obtain `data`,
but only if the true value at `addr` is `comp`.
In both formats,
`data` is a single byte expressed as two hexadecimal digits,
`comp` is also a single byte expressed as two hexadecimal digits,
and `addr` is a memory address in the emulated console,
expressed as however many hexadecimal digits are required
for the console in question
(typically 4 for 8-bit CPUs,
6 for 16-bit CPUs,
and 8 for 32-bit CPUs).

For compatibility with older versions of higan,
the older syntaxes of `addr/data` and `addr/comp/data`
are still supported.

For cheats that require more than a single-byte change,
higan allows multiple codes to be combined with `+`
so that all of them can have a single description
and be toggled with a single click.
For example,
in Super Mario World,
you can lock the time to 999 with these codes:
`7e0f31=09+7e0f32=09+7e0f33=09`.

Changes made in the Cheat Editor are saved to disk
when the game is unloaded,
or when higan exits.
higan stores the known cheats for a particular game
in `higan/cheats.bml`
inside the corresponding game folder
in [the Game Library](#the-game-library).

If your copy of higan includes a cheat database
(a file named `cheats.bml`
in the same directory as `Super Famicom.sys`
and the other `*.sys` directories),
you can click the "Find Codes ..." button in the bottom left
to load all known cheats for the currently-running game.

To add a new cheat,
select an unused row in the list,
then type the relevant codes in the "Code(s)" field at the bottom,
and a description in the "Description" field.

To enable or disable an existing cheat,
tick the checkbox in the first column of the list.
The code should take effect immediately.

To clear out an existing cheat,
select it from the list
and click the "Erase" button in the bottom right,
or just manually delete
the contents of the "Code(s)" and "Description" fields.

To clear out all existing cheats,
click the "Reset" button in the bottom right.

The State Manager
-----------------

The State Manager allows you to create,
load,
and remove Manager states.
For more information on Manager states,
quick states,
saved games
and how they compare,
see [Save States](#save-states).

To create a new manager state,
or to replace an existing one,
select the slot in the list
then click "Save" in the bottom-left corner.
You can then type a description in the "Description" field,
to help you find the state again later.

To rename a state,
select the slot in the list
and edit the "Description" field.

To load a state,
select the slot in the list
and click "Load" in the bottom-left corner,
or just double-click it.

To clear the state out of a slot,
select the slot in the list
and click "Erase" in the bottom-right corner.

To clear all the slots at once,
click "Reset" in the bottom-right corner.

The Manifest Viewer
-------------------

As mentioned in
[Why game folders?](#why-game-folders),
a game cartridge contains
more than just the raw data of the game.

higan uses a "manifest" to
describe how the various parts of a game cartridge
are wired up together,
and the Manifest Viewer lets you examine
the configuration higan is using for the currently-running game.

For some games,
an actual cartridge has been taken apart and carefully examined
and its configuration has been recorded in icarus' database,
so the manifest icarus produces
is guaranteed accurate.
For games that do not exist in icarus' database,
icarus will make a reasonable guess.
This is enough to get the game running,
but does not necessarily reflect the original cartridge.
