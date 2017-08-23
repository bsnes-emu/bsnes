A real game console
is a complex piece of hardware,
with electricity flowing through it
in complex patterns that can't easily
be paused or recorded.
However,
an emulated console is pure software:
it only changes when the emulation software
deliberately updates it,
so the emulator software can save
the entire state of the emulated console
to disk,
and weeks or months later
bring it back to life
as though nothing had ever happened.

Save states versus in-game saves
--------------------------------

Some games include their own systems
for saving and restoring the player's progress.
Here are some of the differences
between save states and in-game saves:

<table>
<thead>
<tr>
<th>Save states</th>
<th>In-game saves</th>
</tr>
</thead>
<tbody>
<tr>
	<td>
	Work exactly the same way
	in every game
	</td>
	<td>
	Works differently in different games,
	some games don't support it at all
	</td>
</tr>
<tr>
	<td>
	Save at any time,
	anywhere in the game
	</td>
	<td>
	Save only at special save-points
	</td>
</tr>
<tr>
	<td>
	Loading a save
	puts the game back exactly how it was
	when you pressed save
	</td>
	<td>
	Loading a save
	restores some things
	(like the player's inventory
	or remaining lives),
	but may
	forget others
	(like taking you back to the beginning of the level)
	</td>
</tr>
<tr>
	<td>
	You can have dozens of save states
	</td>
	<td>
	Most games limit you to about 3 saves
	</td>
</tr>
<tr>
	<td>
	Can only be loaded
	by the same version of the same emulator
	that created it
	</td>
	<td>
	Works with any version of any emulator,
	can sometimes even be copied to or from physical cartridges
	</td>
</tr>
</tbody>
</table>

**Note:**
Loading a save state
will reset the entire emulated console
to the way it was when the save state was created,
*including in-game saves*.
If you create a save state,
then make an in-game save,
then load the save state,
**your in-game save will be lost**.
Don't do that.

Quick states
------------

higan has five Quick State slots,
which can be used from
[the Tools menu](../interface/higan.md#the-tools-menu),
or with the appropriate
[hotkeys](../interface/higan-settings.md#hotkeys).

Quick states are useful
as extra checkpoints
in games that don't have them,
or where they aren't close enough together.
Map the "Save Quick State" and "Load Quick State" hotkeys
to your controller,
and you can cheese your way through just about anything.

Manager states
--------------

higan's
[State Manager](../interface/higan-tools.md#the-state-manager)
allows you to create over a hundred save states,
and add a helpful description to each one.

Manager States are more cumbersome to use than Quick States,
since they cannot be accessed with hotkeys,
but are useful when you want quick access
to many different parts of a game.
