#define AF r.af.word
#define BC r.bc.word
#define DE r.de.word
#define HL (prefix == Prefix::ix ? r.ix.word : prefix == Prefix::iy ? r.iy.word : r.hl.word)

#define A r.af.byte.hi
#define F r.af.byte.lo
#define B r.bc.byte.hi
#define C r.bc.byte.lo
#define D r.de.byte.hi
#define E r.de.byte.lo
#define H (prefix == Prefix::ix ? r.ix.byte.hi : prefix == Prefix::iy ? r.iy.byte.hi : r.hl.byte.hi)
#define L (prefix == Prefix::ix ? r.ix.byte.lo : prefix == Prefix::iy ? r.iy.byte.lo : r.hl.byte.lo)

#define _HL r.hl.word  //true HL (ignores IX/IY prefixes)
#define _H r.hl.byte.hi
#define _L r.hl.byte.lo

#define AF_ r.af_.word  //shadow registers
#define BC_ r.bc_.word
#define DE_ r.de_.word
#define HL_ r.hl_.word

#define A_ r.af_.byte.hi
#define F_ r.af_.byte.lo
#define B_ r.bc_.byte.hi
#define C_ r.bc_.byte.lo
#define D_ r.de_.byte.hi
#define E_ r.de_.byte.lo
#define H_ r.hl_.byte.hi
#define L_ r.hl_.byte.lo

#define SP r.sp
#define PC r.pc

#define IX r.ix.word
#define IY r.iy.word
#define IR r.ir.word

#define I r.ir.byte.hi
#define R r.ir.byte.lo

#define CF r.af.byte.lo.bit(0)
#define NF r.af.byte.lo.bit(1)
#define PF r.af.byte.lo.bit(2)
#define VF r.af.byte.lo.bit(2)
#define XF r.af.byte.lo.bit(3)
#define HF r.af.byte.lo.bit(4)
#define YF r.af.byte.lo.bit(5)
#define ZF r.af.byte.lo.bit(6)
#define SF r.af.byte.lo.bit(7)

#define EI r.ei
#define HALT r.halt
#define IFF1 r.iff1
#define IFF2 r.iff2
#define IM r.im
