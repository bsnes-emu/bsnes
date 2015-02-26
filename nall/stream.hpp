#ifndef NALL_STREAM_HPP
#define NALL_STREAM_HPP

#include <algorithm>
#include <memory>

#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/decode/gzip.hpp>
#include <nall/decode/zip.hpp>

#define NALL_STREAM_INTERNAL_HPP
#include <nall/stream/stream.hpp>
#include <nall/stream/memory.hpp>
#include <nall/stream/mmap.hpp>
#include <nall/stream/file.hpp>
#include <nall/stream/gzip.hpp>
#include <nall/stream/zip.hpp>
#include <nall/stream/auto.hpp>
#undef NALL_STREAM_INTERNAL_HPP

#endif
