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
#include <nall/bmp.hpp>
#include <nall/config.hpp>
#include <nall/crc16.hpp>
#include <nall/crc32.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/function.hpp>
#include <nall/gzip.hpp>
#include <nall/http.hpp>
#include <nall/image.hpp>
#include <nall/inflate.hpp>
#include <nall/interpolation.hpp>
#include <nall/intrinsics.hpp>
#include <nall/invoke.hpp>
#include <nall/map.hpp>
#include <nall/matrix.hpp>
#include <nall/png.hpp>
#include <nall/property.hpp>
#include <nall/random.hpp>
#include <nall/serializer.hpp>
#include <nall/set.hpp>
#include <nall/sha256.hpp>
#include <nall/sort.hpp>
#include <nall/stdint.hpp>
#include <nall/stream.hpp>
#include <nall/string.hpp>
#include <nall/thread.hpp>
#include <nall/traits.hpp>
#include <nall/unzip.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/zip.hpp>

#if defined(PLATFORM_WINDOWS)
  #include <nall/windows/registry.hpp>
  #include <nall/windows/utf8.hpp>
#endif

#if defined(PLATFORM_X)
  #include <nall/serial.hpp>
#endif

#endif
