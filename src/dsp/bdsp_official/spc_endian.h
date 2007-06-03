// Byte order handling

#ifndef SPC_ENDIAN_H
#define SPC_ENDIAN_H

//#include <stdint.h>

static inline unsigned get_le16( void const* p )
{
	return  ((uint8_t const*) p) [1] * 0x100u +
			((uint8_t const*) p) [0];
}

static inline int get_le16s( void const* p )
{
	return  ((int8_t const*) p) [1] * 0x100 +
			((uint8_t const*) p) [0];
}

static inline void set_le16( void* p, unsigned n )
{
	((uint8_t*) p) [1] = (uint8_t) (n >> 8);
	((uint8_t*) p) [0] = (uint8_t) n;
}

// *A versions are used where data is aligned
// Sometimes BIG_ENDIAN is defined to 0x1234 or something, so treat values
// other than 1 as false.
#if BIG_ENDIAN != 1
	#define GET_LE16A( addr )       (*(uint16_t const*) (addr))
	#define GET_LE16SA( addr )      (*( int16_t const*) (addr))
	#define SET_LE16A( addr, data ) (void) (*(uint16_t*) (addr) = (data))
	
#else
	#define GET_LE16A( addr )       get_le16 ( addr )
	#define GET_LE16SA( addr )      get_le16s( addr )
	#define SET_LE16A( addr, data ) set_le16 ( addr, data )
#endif

#define GET_LE16( addr )        GET_LE16A( addr )
#define SET_LE16( addr, data )  SET_LE16A( addr, data )

#endif
