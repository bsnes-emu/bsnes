#ifndef NALL_INTRINSICS_HPP
#define NALL_INTRINSICS_HPP

namespace nall {

struct Intrinsics {
  enum class Compiler : unsigned { Clang, GCC, CL, Unknown };
  enum class Platform : unsigned { Windows, MacOSX, Linux, BSD, Unknown };
  enum class Architecture : unsigned { x86, amd64, Unknown };
  enum class Endian : unsigned { LSB, MSB, Unknown };

  static inline Compiler compiler();
  static inline Platform platform();
  static inline Architecture architecture();
  static inline Endian endian();
};

}

/* Compiler detection */

namespace nall {

#if defined(__clang__)
  #define COMPILER_CLANG
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::Clang; }

  #pragma clang diagnostic ignored "-Wunknown-pragmas"
  #pragma clang diagnostic ignored "-Wempty-body"
  #pragma clang diagnostic ignored "-Wparentheses"
  #pragma clang diagnostic ignored "-Wreturn-type"
  #pragma clang diagnostic ignored "-Wswitch"
  #pragma clang diagnostic ignored "-Wswitch-bool"
  #pragma clang diagnostic ignored "-Wtautological-compare"
#elif defined(__GNUC__)
  #define COMPILER_GCC
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::GCC; }
#elif defined(_MSC_VER)
  #define COMPILER_CL
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::CL; }

  #pragma warning(disable:4996)  //disable libc "deprecation" warnings
#else
  #warning "unable to detect compiler"
  #define COMPILER_UNKNOWN
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::Unknown; }
#endif

}

/* Platform detection */

namespace nall {

#if defined(_WIN32)
  #define PLATFORM_WINDOWS
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::Windows; }
#elif defined(__APPLE__)
  #define PLATFORM_MACOSX
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::MacOSX; }
#elif defined(linux) || defined(__linux__)
  #define PLATFORM_LINUX
  #define PLATFORM_XORG
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::Linux; }
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__)
  #define PLATFORM_BSD
  #define PLATFORM_XORG
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::BSD; }
#else
  #warning "unable to detect platform"
  #define PLATFORM_UNKNOWN
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::Unknown; }
#endif

}

/* Architecture Detection */

#if defined(PLATFORM_MACOSX)
  #include <machine/endian.h>
#elif defined(PLATFORM_LINUX)
  #include <endian.h>
#elif defined(PLATFORM_BSD)
  #include <sys/endian.h>
#endif

namespace nall {

#if defined(__i386__) || defined(_M_IX86)
  #define ARCH_X86
  Intrinsics::Architecture Intrinsics::architecture() { return Intrinsics::Architecture::x86; }
#elif defined(__amd64__) || defined(_M_AMD64)
  #define ARCH_AMD64
  Intrinsics::Architecture Intrinsics::architecture() { return Intrinsics::Architecture::amd64; }
#else
  #warning "unable to detect architecture"
  #define ARCH_UNKNOWN
  Intrinsics::Architecture Intrinsics::architecture() { return Intrinsics::Architecture::Unknown; }
#endif

}

/* Endian detection */

namespace nall {

#if (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN__) || defined(__i386__) || defined(__amd64__) || defined(_M_IX86) || defined(_M_AMD64)
  #define ENDIAN_LSB
  Intrinsics::Endian Intrinsics::endian() { return Intrinsics::Endian::LSB; }
#elif (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && __BYTE_ORDER == __BIG_ENDIAN) || defined(__BIG_ENDIAN__) || defined(__powerpc__) || defined(_M_PPC)
  #define ENDIAN_MSB
  Intrinsics::Endian Intrinsics::endian() { return Intrinsics::Endian::MSB; }
#else
  #warning "unable to detect endian"
  #define ENDIAN_UNKNOWN
  Intrinsics::Endian Intrinsics::endian() { return Intrinsics::Endian::Unknown; }
#endif

}

#endif
