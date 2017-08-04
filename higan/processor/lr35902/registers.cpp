#define AF r.af.word
#define BC r.bc.word
#define DE r.de.word
#define HL r.hl.word
#define SP r.sp.word
#define PC r.pc.word

#define A r.af.byte.hi
#define F r.af.byte.lo
#define B r.bc.byte.hi
#define C r.bc.byte.lo
#define D r.de.byte.hi
#define E r.de.byte.lo
#define H r.hl.byte.hi
#define L r.hl.byte.lo

#define CF r.af.byte.lo.bit(4)
#define HF r.af.byte.lo.bit(5)
#define NF r.af.byte.lo.bit(6)
#define ZF r.af.byte.lo.bit(7)
