// snes_spc 0.9.5 user configuration file. Don't replace when updating library.

#ifndef BLARGG_CONFIG_H
#define BLARGG_CONFIG_H

// Uncomment to use zlib for transparent decompression of gzipped files
//#define HAVE_ZLIB_H

// Uncomment to enable platform-specific (and possibly non-portable) optimizations.
//#define BLARGG_NONPORTABLE 1

// Uncomment if automatic byte-order determination doesn't work
//#define BLARGG_BIG_ENDIAN 1

// Uncomment to enable the normal behavior of getting an out-of-memory exception
// when new fails while creating an object of a class from this library. Note
// that this does not enable exceptions when calling library functions, which
// ALWAYS report errors by blargg_err_t.
//#define BLARGG_DISABLE_NOTHROW

// Use standard config.h if present
#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#endif
