Overall operation
-----------------
This DSP emulator fundamentally emulates the different options the DSP
performs on each clock. The pattern of operations repeats every 32
clocks (except one minor detail, which repeats every 64 clocks instead).
There are three main types of operations:

- Miscellaneous processing
- Voice processing
- Echo processing

Each is done over several clocks, and several operations are done on
each clock. Each clock is defined as a separate function, then called
from a large switch block in a loop.

Many times a value is read on one clock but not used until a later
clock, so many non-local temporary variables are used in the code to
store these values. These are named with t_ to make it clear that they
don't store long-term state.


Circular buffers
----------------
Two circular buffers are used in the code (echo history and BRR decode).
Both need efficient index-based access with wrap-around. Things are
greatly simplified by repeating the contents of buffer twice, so instead
of

	0 1 2 3 4 5 6 7

it stores

	0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7

The position in this case would always be 0 to 7, so reading up to +8
won't go outside buffer. This duplication is maintained by simply
writing data twice when filling buffer:

	0 1 2 3 4 # 6 7 0 1 2 3 4 # 6 7
new data -----^---------------^

No wrap checking needs to be done when writing either, since the above
reasoning holds. When making a state snapshot, only the first copy needs
to be saved. When restoring, simply duplicate the data twice.


Code
----
- Currently all state is in static variables. They have either a t_ or
m_ prefix to allow easy migration to a structure.

- Static state that persists over several samples or more is prefixed
with m_.

- State which is temporary to the current sample is prefixed with t_.
These are usually just overwritten with new data on the next sample.
These generally correspond to temporaries/registers in actual DSP
itself.

- Minimal stdint.h included in case your system doesn't have one.


-- 
Shay Green <gblargg@gmail.com>
