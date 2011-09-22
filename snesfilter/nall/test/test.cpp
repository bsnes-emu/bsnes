#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/base64.hpp>
#include <nall/bmp.hpp>
#include <nall/compositor.hpp>
#include <nall/concept.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/dsp.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/gzip.hpp>
#include <nall/http.hpp>
#include <nall/inflate.hpp>
#include <nall/input.hpp>
#include <nall/ips.hpp>
#include <nall/lzss.hpp>
#include <nall/png.hpp>
#include <nall/priorityqueue.hpp>
#include <nall/public_cast.hpp>
#include <nall/reference_array.hpp>
#include <nall/resource.hpp>
#include <nall/serializer.hpp>
#include <nall/stack.hpp>
#include <nall/static.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/zip.hpp>
#include <nall/bps/delta.hpp>
#include <nall/bps/linear.hpp>
#include <nall/bps/metadata.hpp>
#include <nall/bps/patch.hpp>
#include <nall/snes/cpu.hpp>
#include <nall/snes/smp.hpp>
#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>
using namespace nall;

int main(int main, char **argv) {
  lstring a = { "hey", "hi" }, b = { "hey", "hi" };
  print(a == b, "\n");

  return 0;
}
