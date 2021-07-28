#ifndef NALL_WINDOWS_GUARD_HPP
#define NALL_WINDOWS_GUARD_HPP

#define boolean WindowsBoolean
#define interface WindowsInterface

#undef UNICODE
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#undef PATH_MAX

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define PATH_MAX 260

#else
#undef NALL_WINDOWS_GUARD_HPP

#undef boolean
#undef interface

#undef far
#undef near

#endif
