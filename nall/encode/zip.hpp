#pragma once

//creates uncompressed ZIP archives

#include <nall/string.hpp>
#include <nall/hash/crc32.hpp>

namespace nall { namespace Encode {

struct ZIP {
  ZIP(const string& filename) {
    fp.open(filename, file::mode::write);
    time_t currentTime = time(nullptr);
    tm* info = localtime(&currentTime);
    dosTime = (info->tm_hour << 11) | (info->tm_min << 5) | (info->tm_sec >> 1);
    dosDate = ((info->tm_year - 80) << 9) | ((1 + info->tm_mon) << 5) + (info->tm_mday);
  }

  //append path: append("path/");
  //append file: append("path/file", data, size);
  auto append(string filename, const uint8_t* data = nullptr, unsigned size = 0u) -> void {
    filename.transform("\\", "/");
    uint32_t checksum = Hash::CRC32(data, size).digest().hex();
    directory.append({filename, checksum, size, fp.offset()});

    fp.writel(0x04034b50, 4);         //signature
    fp.writel(0x0014, 2);             //minimum version (2.0)
    fp.writel(0x0000, 2);             //general purpose bit flags
    fp.writel(0x0000, 2);             //compression method (0 = uncompressed)
    fp.writel(dosTime, 2);
    fp.writel(dosDate, 2);
    fp.writel(checksum, 4);
    fp.writel(size, 4);               //compressed size
    fp.writel(size, 4);               //uncompressed size
    fp.writel(filename.length(), 2);  //file name length
    fp.writel(0x0000, 2);             //extra field length
    fp.print(filename);               //file name

    fp.write(data, size);             //file data
  }

  ~ZIP() {
    //central directory
    unsigned baseOffset = fp.offset();
    for(auto& entry : directory) {
      fp.writel(0x02014b50, 4);               //signature
      fp.writel(0x0014, 2);                   //version made by (2.0)
      fp.writel(0x0014, 2);                   //version needed to extract (2.0)
      fp.writel(0x0000, 2);                   //general purpose bit flags
      fp.writel(0x0000, 2);                   //compression method (0 = uncompressed)
      fp.writel(dosTime, 2);
      fp.writel(dosDate, 2);
      fp.writel(entry.checksum, 4);
      fp.writel(entry.size, 4);               //compressed size
      fp.writel(entry.size, 4);               //uncompressed size
      fp.writel(entry.filename.length(), 2);  //file name length
      fp.writel(0x0000, 2);                   //extra field length
      fp.writel(0x0000, 2);                   //file comment length
      fp.writel(0x0000, 2);                   //disk number start
      fp.writel(0x0000, 2);                   //internal file attributes
      fp.writel(0x00000000, 4);               //external file attributes
      fp.writel(entry.offset, 4);             //relative offset of file header
      fp.print(entry.filename);
    }
    unsigned finishOffset = fp.offset();

    //end of central directory
    fp.writel(0x06054b50, 4);                 //signature
    fp.writel(0x0000, 2);                     //number of this disk
    fp.writel(0x0000, 2);                     //disk where central directory starts
    fp.writel(directory.size(), 2);           //number of central directory records on this disk
    fp.writel(directory.size(), 2);           //total number of central directory records
    fp.writel(finishOffset - baseOffset, 4);  //size of central directory
    fp.writel(baseOffset, 4);                 //offset of central directory
    fp.writel(0x0000, 2);                     //comment length

    fp.close();
  }

protected:
  file fp;
  uint16_t dosTime, dosDate;
  struct entry_t {
    string filename;
    uint32_t checksum;
    uint32_t size;
    uint32_t offset;
  };
  vector<entry_t> directory;
};

}}
