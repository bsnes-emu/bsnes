
namespace Bit {
	  
    template<typename T, int w = sizeof(T)*8/2>
    constexpr int count(T n) {
        return n == 0 || w == 0? n
            : count<w/2>(n>>w & (1<<w)-1)
            + count<w/2>(n    & (1<<w)-1);
    }
    
// These functions deal with binary string literals like:
//   "1000110.... // xxxx...0000"
//   
//   Firstly, spaces are ignored. Dots '.' are don't-care bits.
//   An / is shorthand for a blank nibble (4 bits or "....").
//   
//   binary() returns a mask of the 1s.
//   mask() returns a mask of the 1s and 0s.
//   field() returns a mask of letters for use with collect().
//   match() uses the 1s and 0s; anything else is wildcard.
//   
constexpr bool isfield(char c)       { return c!='.'&& c!='/'&& c!='0'&& c!='1'; }

constexpr long bit(int n)            { return n < 32? 1<<n : 0; }
constexpr int bitpos(const char *s)  { return !*s? -1 : *s=='/'? 4 + bitpos(s+1) : (*s!=' ') + bitpos(s+1); }
constexpr int fieldpos(const char *s){ return !*s? -1 : (*s!=' ' && isfield(*s)) + fieldpos(s+1); }

// Finds index of the lowest bit set in a compile-time constant.
template<int s = 16> constexpr long lowest_bit_no(long x) {
    return !x? -1 : !s? 0 :
               x & (1<<s)-1?     lowest_bit_no<s/2>(x)
                           : s + lowest_bit_no<s/2>(x >> s);
}

constexpr long binary(const char *s) {
	return !*s? 0 : (*s=='1')              << bitpos(s) | binary(s+1);
}
constexpr long mask(const char *s) {
	return !*s? 0 : (*s=='0'||*s=='1')     << bitpos(s) | mask(s+1);
}
constexpr long field(const char *s) {
	return !*s? 0 : (*s!=' '&& isfield(*s)) << bitpos(s) | field(s+1);
}
constexpr bool match(long data, const char *s) {
	return (data & mask(s)) == binary(s);
}

// Masks all 'bits' from the input and shifts them down into one contiguous
// value. This inlines to a sequence of &, >> and + instructions.
// Example:
//   collect<mask("xxxx....yyyy....zzzz")>(i)
//     joins i's 1st, 3rd, and 5th nibbles (0x12345 => 0x135).
template<long bits>            constexpr long collect(long data);
template<long bits, int nmask> constexpr long collect_field(long data) {
	return (data & (1<<nmask)-1) * (bits&1)
	  + (collect<((unsigned long)bits >> nmask)>(data >> nmask) << nmask*(bits&1));
}

template<long bits> constexpr long collect(long data) {
	return collect_field<bits, lowest_bit_no(bits & 1? ~bits : bits)>(data);
}
template<> constexpr long collect<0>(long data) {
	return 0;
}

};
