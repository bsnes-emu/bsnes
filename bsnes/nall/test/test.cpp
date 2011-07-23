#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/base64.hpp>
#include <nall/concept.hpp>
#include <nall/config.hpp>
#include <nall/dictionary.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/input.hpp>
#include <nall/lzss.hpp>
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
#include <nall/snes/cpu.hpp>
#include <nall/snes/smp.hpp>
#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>
using namespace nall;

#include "bsnes-logo.hpp"

int main(int argc, char **argv) {
//  resource::encode("bsnes-logo.hpp", "bsnes-logo.bmp");
  resource logo;
  logo.decode(data, size);
  file fp;
  fp.open("output.bmp", file::mode::write);
  fp.write(logo.data, logo.size);
  fp.close();
  return 0;
}
