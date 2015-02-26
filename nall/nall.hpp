/* nall
 * author: byuu
 * license: ISC
 *
 * nall is a header library that provides both fundamental and useful classes
 * its goals are portability, consistency, minimalism and reusability
 */

#ifndef NALL_HPP
#define NALL_HPP

//include the most common nall headers with one statement
//does not include the most obscure components with high cost and low usage

#include <nall/platform.hpp>

#include <nall/algorithm.hpp>
#include <nall/any.hpp>
#include <nall/atoi.hpp>
#include <nall/base64.hpp>
#include <nall/bit.hpp>
#include <nall/bitvector.hpp>
#include <nall/bmp.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/function.hpp>
#include <nall/group.hpp>
#include <nall/hashset.hpp>
#include <nall/hid.hpp>
#include <nall/image.hpp>
#include <nall/interpolation.hpp>
#include <nall/intrinsics.hpp>
#include <nall/invoke.hpp>
#include <nall/map.hpp>
#include <nall/matrix.hpp>
#include <nall/maybe.hpp>
#include <nall/memory.hpp>
#include <nall/property.hpp>
#include <nall/random.hpp>
#include <nall/range.hpp>
#include <nall/serializer.hpp>
#include <nall/set.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/sort.hpp>
#include <nall/stdint.hpp>
#include <nall/storage.hpp>
#include <nall/stream.hpp>
#include <nall/string.hpp>
#include <nall/thread.hpp>
#include <nall/traits.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/zip.hpp>
#include <nall/decode/bmp.hpp>
#include <nall/decode/gzip.hpp>
#include <nall/decode/inflate.hpp>
#include <nall/decode/png.hpp>
#include <nall/decode/zip.hpp>
#include <nall/hash/crc16.hpp>
#include <nall/hash/crc32.hpp>
#include <nall/hash/sha256.hpp>

#if defined(PLATFORM_WINDOWS)
  #include <nall/windows/registry.hpp>
  #include <nall/windows/utf8.hpp>
#endif

#if defined(PLATFORM_XORG)
  #include <nall/serial.hpp>
#endif

#endif
