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
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>
#include <nall/memory.hpp>
#include <nall/primitives.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/stdint.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/hash/crc16.hpp>
#include <nall/hash/crc32.hpp>
#include <nall/hash/sha256.hpp>

#define NALL_STRING_INTERNAL_HPP
#include <nall/string/base.hpp>
#include <nall/string/view.hpp>
#include <nall/string/atoi.hpp>
#include <nall/string/cast.hpp>
#include <nall/string/compare.hpp>
#include <nall/string/convert.hpp>
#include <nall/string/core.hpp>
#include <nall/string/datetime.hpp>
#include <nall/string/find.hpp>
#include <nall/string/format.hpp>
#include <nall/string/hash.hpp>
#include <nall/string/list.hpp>
#include <nall/string/match.hpp>
#include <nall/string/path.hpp>
#include <nall/string/platform.hpp>
#include <nall/string/replace.hpp>
#include <nall/string/split.hpp>
#include <nall/string/trim.hpp>
#include <nall/string/utility.hpp>
#include <nall/string/eval/node.hpp>
#include <nall/string/eval/literal.hpp>
#include <nall/string/eval/parser.hpp>
#include <nall/string/eval/evaluator.hpp>
#include <nall/string/markup/node.hpp>
#include <nall/string/markup/find.hpp>
#include <nall/string/markup/bml.hpp>
#include <nall/string/markup/xml.hpp>
#include <nall/string/transform/cml.hpp>
#include <nall/string/transform/dml.hpp>
#undef NALL_STRING_INTERNAL_HPP

#endif
