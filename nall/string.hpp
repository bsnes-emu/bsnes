#ifndef NALL_STRING_HPP
#define NALL_STRING_HPP

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <initializer_list>
#include <memory>

#include <nall/platform.hpp>
#include <nall/atoi.hpp>
#include <nall/crc32.hpp>
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>
#include <nall/sha256.hpp>
#include <nall/stdint.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>

#include <nall/windows/utf8.hpp>

#define NALL_STRING_INTERNAL_HPP
#include <nall/string/char.hpp>
#include <nall/string/base.hpp>
#include <nall/string/ref.hpp>
#include <nall/string/cast.hpp>
#include <nall/string/core.hpp>
#include <nall/string/datetime.hpp>
#include <nall/string/file.hpp>
#include <nall/string/filename.hpp>
#include <nall/string/format.hpp>
#include <nall/string/list.hpp>
#include <nall/string/platform.hpp>
#include <nall/string/replace.hpp>
#include <nall/string/split.hpp>
#include <nall/string/utility.hpp>
#include <nall/string/variadic.hpp>
#include <nall/string/wrapper.hpp>
#include <nall/string/eval/node.hpp>
#include <nall/string/eval/literal.hpp>
#include <nall/string/eval/parser.hpp>
#include <nall/string/eval/evaluator.hpp>
#include <nall/string/markup/node.hpp>
#include <nall/string/markup/bml.hpp>
#include <nall/string/markup/xml.hpp>
#include <nall/string/markup/document.hpp>
#undef NALL_STRING_INTERNAL_HPP

#endif
