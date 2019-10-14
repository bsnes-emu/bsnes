Contributing
============

Code contributions are most welcome and highly appreciated!

But first, please note that although bsnes is licensed under the GPLv3 license,
in order to be merged upstream, any code contributions must be provided under
the ISC source code license.

This is *not* a CLA (community license agreement), no legal contract needs to be
signed, and you will maintain full and exclusive copyright ownership over any
contributed source code.

There are two reasons for this requirement:

GPLv4+
------

bsnes is currently licensed under the GPLv3 license only. I do not license bsnes
under the GPLv3 or later license, because there is no way of knowing what the
GPLv4 and later licenses will change, and if they will be in the best interests
of emulator development and video game preservation.

Although I put a good deal of trust into the FSF, no one is an oracle that can
predict the future. Would *you* agree to a license before being able to read it?

However, the GPLv4 may prove beneficial, and close important holes in the GPLv3
license, just as the GPLv3 license closed the GPLv2's TiVoization loophole. And
so it is important that bsnes retains the option of relicensing to the GPLv4+ in
the future.

As a point of interest, there have been projects with similar concerns about
using a GPLv2 or later clause, that are now permanently stuck on the GPLv2
license. There have also been projects that did use a GPLv2 or later clause,
only to disagree with the changes introduced in the GPLv3.

ISC
---

The more important reason for this requirement is that it is my intention to
release the entirety of bsnes under the ISC license once official upstream
development has ceased.

The reason I would want to relicense bsnes to the ISC license upon its official
discontinuation is because once again, no one is an oracle, and I cannot predict
what future issues bsnes permanently remaining under the GPLv3 license may
cause.

For instance, imagine a world where a certain vendor took over the world, and
the only way to distribute applications was with their approval, and their store
rules forbade GPLv3 software. Or perhaps a world where the GPL was abandoned in
favor of the new OSSv1 license. But GPLv3 software was incompatible with the
OSSv1 license. Other open source developers would not be able to use bsnes in
that scenario.

It would be very disappointing if all of our work ended up unusable 50+ years
into the future because it was permanently bound to the GPLv3 license.

GPLv3
-----

The reason I use the GPLv3 license currently is because it is a balance between
altruism and self-interest. The GPLv3 allows other vendors to sell my own code
without sharing revenue with me, and indeed this has already happened. But the
GPLv3 also prevents other vendors from improving upon bsnes without sharing
their work with everyone else as I have.

While I am actively developing bsnes, I do not wish to compete against myself.

As such, I believe the GPLv3 is the best license during active development, and
the ISC is the best license once bsnes is officially discontinued.

Considerations
--------------

This is the part that should concern you as a contributor: I am not requesting
contributed source code to be released under the ISC so that I personally may
sell GPLv3 commercial license exemptions to your work, but in the future when
bsnes is released under the ISC license, that will open the door for anyone to
sell the work commercially in a closed source form.

If this is not acceptable to you, I wholly understand and I welcome you to
release your work under the GPLv3 in the form of a bsnes fork. And if your work
is not an essential part of the core emulation -- that is to say, it may be
optionally disabled -- then I am still willing to work with you in merging such
work upstream anyway under the full GPLv3 license, but please reach out to me
first before developing under the assumption your work will be merged upstream.

Thank you very much for reading and hopefully for your understanding.
