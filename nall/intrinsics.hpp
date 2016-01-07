#pragma once

namespace nall {
  struct Intrinsics {
    enum class Compiler : unsigned { Clang, GCC, VisualCPP, Unknown };
    enum class Platform : unsigned { Windows, MacOSX, Linux, BSD, Unknown };
    enum class API : unsigned { Windows, Posix, Unknown };
    enum class Display : unsigned { Windows, Quartz, Xorg, Unknown };
    enum class Processor : unsigned { x86, amd64, ARM, PPC32, PPC64, Unknown };
    enum class Endian : unsigned { LSB, MSB, Unknown };

    static inline auto compiler() -> Compiler;
    static inline auto platform() -> Platform;
    static inline auto api() -> API;
    static inline auto display() -> Display;
    static inline auto processor() -> Processor;
    static inline auto endian() -> Endian;
  };
}

/* Compiler detection */

namespace nall {

#if defined(__clang__)
  #define COMPILER_CLANG
  auto Intrinsics::compiler() -> Compiler { return Compiler::Clang; }

  #pragma clang diagnostic ignored "-Wunknown-pragmas"
  #pragma clang diagnostic ignored "-Wempty-body"
  #pragma clang diagnostic ignored "-Wparentheses"
  #pragma clang diagnostic ignored "-Wreturn-type"
  #pragma clang diagnostic ignored "-Wswitch"
  #pragma clang diagnostic ignored "-Wswitch-bool"
  #pragma clang diagnostic ignored "-Wtautological-compare"
  #pragma clang diagnostic ignored "-Wabsolute-value"

  //temporary
  #pragma clang diagnostic ignored "-Winconsistent-missing-override"
  #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
  #define COMPILER_GCC
  auto Intrinsics::compiler() -> Compiler { return Compiler::GCC; }

  #pragma GCC diagnostic ignored "-Wunknown-pragmas"
  #pragma GCC diagnostic ignored "-Wpragmas"
  #pragma GCC diagnostic ignored "-Wswitch-bool"
#elif defined(_MSC_VER)
  #define COMPILER_VISUALCPP
  auto Intrinsics::compiler() -> Compiler { return Compiler::VisualCPP; }

  #pragma warning(disable:4996)  //libc "deprecation" warnings
#else
  #warning "unable to detect compiler"
  #define COMPILER_UNKNOWN
  auto Intrinsics::compiler() -> Compiler { return Compiler::Unknown; }
#endif

}

/* Platform detection */

namespace nall {

#if defined(_WIN32)
  #define PLATFORM_WINDOWS
  #define API_WINDOWS
  #define DISPLAY_WINDOWS
  auto Intrinsics::platform() -> Platform { return Platform::Windows; }
  auto Intrinsics::api() -> API { return API::Windows; }
  auto Intrinsics::display() -> Display { return Display::Windows; }
#elif defined(__APPLE__)
  #define PLATFORM_MACOSX
  #define API_POSIX
  #define DISPLAY_QUARTZ
  auto Intrinsics::platform() -> Platform { return Platform::MacOSX; }
  auto Intrinsics::api() -> API { return API::Posix; }
  auto Intrinsics::display() -> Display { return Display::Quartz; }
#elif defined(linux) || defined(__linux__)
  #define PLATFORM_LINUX
  #define API_POSIX
  #define DISPLAY_XORG
  auto Intrinsics::platform() -> Platform { return Platform::Linux; }
  auto Intrinsics::api() -> API { return API::Posix; }
  auto Intrinsics::display() -> Display { return Display::Xorg; }
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__)
  #define PLATFORM_BSD
  #define API_POSIX
  #define DISPLAY_XORG
  auto Intrinsics::platform() -> Platform { return Platform::BSD; }
  auto Intrinsics::api() -> API { return API::Posix; }
  auto Intrinsics::display() -> Display { return Display::Xorg; }
#else
  #warning "unable to detect platform"
  #define PLATFORM_UNKNOWN
  #define API_UNKNOWN
  #define DISPLAY_UNKNOWN
  auto Intrinsics::platform() -> Platform { return Platform::Unknown; }
  auto Intrinsics::api() -> API { return API::Unknown; }
  auto Intrinsics::display() -> Display { return Display::Unknown; }
#endif

}

#if defined(PLATFORM_MACOSX)
  #include <machine/endian.h>
#elif defined(PLATFORM_LINUX)
  #include <endian.h>
#elif defined(PLATFORM_BSD)
  #include <sys/endian.h>
#endif

/* Processor Detection */

namespace nall {

#if defined(__i386__) || defined(_M_IX86)
  #define PROCESSOR_X86
  auto Intrinsics::processor() -> Processor { return Processor::x86; }
#elif defined(__amd64__) || defined(_M_AMD64)
  #define PROCESSOR_AMD64
  auto Intrinsics::processor() -> Processor { return Processor::amd64; }
#elif defined(__arm__)
  #define PROCESSOR_ARM
  auto Intrinsics::processor() -> Processor { return Processor::ARM; }
#elif defined(__ppc64__) || defined(_ARCH_PPC64)
  #define PROCESSOR_PPC64
  auto Intrinsics::processor() -> Processor { return Processor::PPC64; }
#elif defined(__ppc__) || defined(_ARCH_PPC) || defined(_M_PPC)
  #define PROCESSOR_PPC32
  auto Intrinsics::processor() -> Processor { return Processor::PPC32; }
#else
  #warning "unable to detect processor"
  #define PROCESSOR_UNKNOWN
  auto Intrinsics::processor() -> Processor { return Processor::Unknown; }
#endif

}

/* Endian detection */

namespace nall {

#if (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN__) || defined(__i386__) || defined(__amd64__) || defined(_M_IX86) || defined(_M_AMD64)
  #define ENDIAN_LSB
  auto Intrinsics::endian() -> Endian { return Endian::LSB; }
#elif (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && __BYTE_ORDER == __BIG_ENDIAN) || defined(__BIG_ENDIAN__) || defined(__powerpc__) || defined(_M_PPC)
  #define ENDIAN_MSB
  auto Intrinsics::endian() -> Endian { return Endian::MSB; }
#else
  #warning "unable to detect endian"
  #define ENDIAN_UNKNOWN
  auto Intrinsics::endian() -> Endian { return Endian::Unknown; }
#endif

}
