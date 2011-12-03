#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/base64.hpp>
#include <nall/bmp.hpp>
#include <nall/compositor.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/dsp.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/function.hpp>
#include <nall/gzip.hpp>
#include <nall/http.hpp>
#include <nall/image.hpp>
#include <nall/inflate.hpp>
#include <nall/input.hpp>
#include <nall/intrinsics.hpp>
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

int main() {
  image x(0, 64, 65535ull << 48, 65535ull << 32, 65535ull << 16, 65535ull << 0);
  x.allocate(4, 1);

  uint64_t *data = (uint64_t*)x.data;
  data[0] = 0xffffffffffffffffull;
  data[1] = 0x0000ffff00000000ull;
  data[2] = 0x00000000ffff0000ull;
  data[3] = 0x000000000000ffffull;

  x.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  uint32_t *output = (uint32_t*)x.data;
  print(hex<8>(output[0]), "\n");
  print(hex<8>(output[1]), "\n");
  print(hex<8>(output[2]), "\n");
  print(hex<8>(output[3]), "\n");

  return 0;
}

/*
int main(int argc, char **argv) {
  string text;
  text.readfile("document.bml");
  BML::Document document(text);
  if(document.error) print(document.error, "\n");

  for(auto &a : document) {
    print("", a.name, ":", a.value, ";\n");
    for(auto &b : a) {
      print("  ", b.name, ":", b.value, ";\n");
      for(auto &c : b) {
        print("    ", c.name, ":", c.value, ";\n");
        for(auto &d : c) {
          print("      ", d.name, ":", d.value, ";\n");
        }
      }
    }
  }

//print(document["cartridge"]["title"].value, "\n");

  return 0;
}
*/
