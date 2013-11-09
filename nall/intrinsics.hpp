#ifndef NALL_INTRINSICS_HPP
#define NALL_INTRINSICS_HPP

namespace nall {

struct Intrinsics {
  enum class Compiler : unsigned { Clang, GCC, VisualCPP, Unknown };
  enum class Platform : unsigned { Windows, MacOSX, X, Unknown };  //X = Linux, BSD, etc
  enum class Architecture : unsigned { x86, amd64, Unknown };
  enum class Endian : unsigned { LSB, MSB, Unknown };

  static inline Compiler compiler();
  static inline Platform platform();
  static inline Architecture architecture();
  static inline Endian endian();
};

/* Compiler detection */

#if defined(__clang__)
  #define COMPILER_CLANG
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::Clang; }
#elif defined(__GNUC__)
  #define COMPILER_GCC
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::GCC; }
#elif defined(_MSC_VER)
  #define COMPILER_VISUALCPP
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::VisualCPP; }
#else
  #warning "unable to detect compiler"
  #define COMPILER_UNKNOWN
  Intrinsics::Compiler Intrinsics::compiler() { return Intrinsics::Compiler::Unknown; }
#endif

/* Platform detection */

#if defined(_WIN32)
  #define PLATFORM_WINDOWS
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::Windows; }
#elif defined(__APPLE__)
  #define PLATFORM_MACOSX
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::MacOSX; }
#elif defined(linux) || defined(__linux__) || defined(__sun__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__GNU__)
  #define PLATFORM_X
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::X; }
#else
  #warning "unable to detect platform"
  #define PLATFORM_UNKNOWN
  Intrinsics::Platform Intrinsics::platform() { return Intrinsics::Platform::Unknown; }
#endif

/* Architecture Detection */

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

/* Endian detection */

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
