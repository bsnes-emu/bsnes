#ifdef CARTRIDGE_CPP

void Cartridge::parse_xml(const lstring &list) {
  mapping.reset();
  parse_xml_cartridge(list[0]);

  if(mode == Mode::BsxSlotted) {
    parse_xml_bsx(list[1]);
  } else if(mode == Mode::Bsx) {
    parse_xml_bsx(list[1]);
  } else if(mode == Mode::SufamiTurbo) {
    parse_xml_sufami_turbo(list[1], 0);
    parse_xml_sufami_turbo(list[2], 1);
  } else if(mode == Mode::SuperGameBoy) {
    parse_xml_gameboy(list[1]);
  }
}

void Cartridge::parse_xml_cartridge(const char *data) {
  xml_element document = xml_parse(data);
  if(document.element.size() == 0) return;

  foreach(head, document.element) {
    if(head.name == "cartridge") {
      foreach(attr, head.attribute) {
        if(attr.name == "region") {
          if(attr.content == "NTSC") region = Region::NTSC;
          if(attr.content == "PAL") region = Region::PAL;
        }
      }

      foreach(node, head.element) {
        if(node.name == "rom") xml_parse_rom(node);
        if(node.name == "ram") xml_parse_ram(node);
        if(node.name == "icd2") xml_parse_icd2(node);
        if(node.name == "superfx") xml_parse_superfx(node);
        if(node.name == "sa1") xml_parse_sa1(node);
        if(node.name == "upd7725") xml_parse_upd7725(node);
        if(node.name == "upd96050") xml_parse_upd96050(node);
        if(node.name == "bsx") xml_parse_bsx(node);
        if(node.name == "sufamiturbo") xml_parse_sufamiturbo(node);
        if(node.name == "srtc") xml_parse_srtc(node);
        if(node.name == "sdd1") xml_parse_sdd1(node);
        if(node.name == "spc7110") xml_parse_spc7110(node);
        if(node.name == "cx4") xml_parse_cx4(node);
        if(node.name == "obc1") xml_parse_obc1(node);
        if(node.name == "setadsp") xml_parse_setadsp(node);
        if(node.name == "setarisc") xml_parse_setarisc(node);
        if(node.name == "msu1") xml_parse_msu1(node);
        if(node.name == "serial") xml_parse_serial(node);
      }
    }
  }
}

void Cartridge::parse_xml_bsx(const char *data) {
}

void Cartridge::parse_xml_sufami_turbo(const char *data, bool slot) {
}

void Cartridge::parse_xml_gameboy(const char *data) {
}

void Cartridge::xml_parse_rom(xml_element &root) {
  foreach(leaf, root.element) {
    if(leaf.name == "map") {
      Mapping m(memory::cartrom);
      foreach(attr, leaf.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
        if(attr.name == "mode") xml_parse_mode(m, attr.content);
        if(attr.name == "offset") m.offset = hex(attr.content);
        if(attr.name == "size") m.size = hex(attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_ram(xml_element &root) {
  foreach(attr, root.attribute) {
    if(attr.name == "size") ram_size = hex(attr.content);
  }

  foreach(leaf, root.element) {
    if(leaf.name == "map") {
      Mapping m(memory::cartram);
      foreach(attr, leaf.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
        if(attr.name == "mode") xml_parse_mode(m, attr.content);
        if(attr.name == "offset") m.offset = hex(attr.content);
        if(attr.name == "size") m.size = hex(attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_icd2(xml_element &root) {
  if(mode != Mode::SuperGameBoy) return;

  foreach(attr, root.attribute) {
    if(attr.name == "revision") {
      if(attr.content == "1") supergameboy_version = SuperGameBoyVersion::Version1;
      if(attr.content == "2") supergameboy_version = SuperGameBoyVersion::Version2;
    }
  }

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m((Memory&)icd2);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_superfx(xml_element &root) {
  has_superfx = true;

  foreach(node, root.element) {
    if(node.name == "rom") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::fxrom);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "ram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::fxram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(superfx);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_sa1(xml_element &root) {
  has_sa1 = true;

  foreach(node, root.element) {
    if(node.name == "rom") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::vsprom);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "iram") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::cpuiram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "bwram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::cc1bwram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(sa1);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_upd7725(xml_element &root) {
  has_upd7725 = true;

  bool program = false;
  bool sha256 = false;
  string xml_hash;
  string rom_hash;

  for(unsigned n = 0; n < 2048; n++) upd7725.programROM[n] = 0;
  for(unsigned n = 0; n < 1024; n++) upd7725.dataROM[n] = 0;

  foreach(attr, root.attribute) {
    if(attr.name == "program") {
      file fp;
      fp.open(string(dir(basename()), attr.content), file::mode::read);
      if(fp.open() && fp.size() == 8192) {
        program = true;

        for(unsigned n = 0; n < 2048; n++) {
          upd7725.programROM[n] = fp.readm(3);
        }
        for(unsigned n = 0; n < 1024; n++) {
          upd7725.dataROM[n] = fp.readm(2);
        }

        fp.seek(0);
        uint8 data[8192];
        fp.read(data, 8192);
        fp.close();

        sha256_ctx sha;
        uint8 shahash[32];
        sha256_init(&sha);
        sha256_chunk(&sha, data, 8192);
        sha256_final(&sha);
        sha256_hash(&sha, shahash);
        foreach(n, shahash) rom_hash.append(hex<2>(n));
      }
    } else if(attr.name == "sha256") {
      sha256 = true;
      xml_hash = attr.content;
    }
  }

  foreach(node, root.element) {
    if(node.name == "dr") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(upd7725dr);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "sr") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(upd7725sr);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }

  if(program == false) {
    system.interface->message("Warning: uPD7725 program is missing.");
  } else if(sha256 == true && xml_hash != rom_hash) {
    system.interface->message({
      "Warning: uPD7725 program SHA256 is incorrect.\n\n"
      "Expected:\n", xml_hash, "\n\n"
      "Actual:\n", rom_hash
    });
  }
}

void Cartridge::xml_parse_upd96050(xml_element &root) {
  has_upd96050 = true;

  bool program = false;
  bool sha256 = false;
  string xml_hash;
  string rom_hash;

  for(unsigned n = 0; n < 16384; n++) upd96050.programROM[n] = 0;
  for(unsigned n = 0; n <  2048; n++) upd96050.dataROM[n] = 0;

  foreach(attr, root.attribute) {
    if(attr.name == "program") {
      file fp;
      fp.open(string(dir(basename()), attr.content), file::mode::read);
      if(fp.open() && fp.size() == 52 * 1024) {
        program = true;

        for(unsigned n = 0; n < 16384; n++) {
          upd96050.programROM[n] = fp.readm(3);
        }
        for(unsigned n = 0; n <  2048; n++) {
          upd96050.dataROM[n] = fp.readm(2);
        }

        fp.seek(0);
        uint8 data[52 * 1024];
        fp.read(data, 52 * 1024);
        fp.close();

        sha256_ctx sha;
        uint8 shahash[32];
        sha256_init(&sha);
        sha256_chunk(&sha, data, 52 * 1024);
        sha256_final(&sha);
        sha256_hash(&sha, shahash);
        foreach(n, shahash) rom_hash.append(hex<2>(n));
      }
    } else if(attr.name == "sha256") {
      sha256 = true;
      xml_hash = attr.content;
    }
  }

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m(upd96050);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }

  if(program == false) {
    system.interface->message("Warning: uPD96050 program is missing.");
  } else if(sha256 == true && xml_hash != rom_hash) {
    system.interface->message({
      "Warning: uPD96050 program SHA256 is incorrect.\n\n"
      "Expected:\n", xml_hash, "\n\n"
      "Actual:\n", rom_hash
    });
  }
}

void Cartridge::xml_parse_bsx(xml_element &root) {
  if(mode != Mode::BsxSlotted && mode != Mode::Bsx) return;

  foreach(node, root.element) {
    if(node.name == "slot") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(memory::bsxflash);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(bsxcart);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_sufamiturbo(xml_element &root) {
  if(mode != Mode::SufamiTurbo) return;

  foreach(node, root.element) {
    if(node.name == "slot") {
      bool slotid = 0;
      foreach(attr, node.attribute) {
        if(attr.name == "id") {
          if(attr.content == "A") slotid = 0;
          if(attr.content == "B") slotid = 1;
        }
      }

      foreach(slot, node.element) {
        if(slot.name == "rom") {
          foreach(leaf, slot.element) {
            if(leaf.name == "map") {
              Mapping m(slotid == 0 ? memory::stArom : memory::stBrom);
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
                if(attr.name == "mode") xml_parse_mode(m, attr.content);
                if(attr.name == "offset") m.offset = hex(attr.content);
                if(attr.name == "size") m.size = hex(attr.content);
              }
              if(m.memory->size() > 0) mapping.append(m);
            }
          }
        } else if(slot.name == "ram") {
          foreach(leaf, slot.element) {
            if(leaf.name == "map") {
              Mapping m(slotid == 0 ? memory::stAram : memory::stBram);
              foreach(attr, leaf.attribute) {
                if(attr.name == "address") xml_parse_address(m, attr.content);
                if(attr.name == "mode") xml_parse_mode(m, attr.content);
                if(attr.name == "offset") m.offset = hex(attr.content);
                if(attr.name == "size") m.size = hex(attr.content);
              }
              if(m.memory->size() > 0) mapping.append(m);
            }
          }
        }
      }
    }
  }
}

void Cartridge::xml_parse_srtc(xml_element &root) {
  has_srtc = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m(srtc);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_sdd1(xml_element &root) {
  has_sdd1 = true;

  foreach(node, root.element) {
    if(node.name == "mcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m((Memory&)sdd1);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m((MMIO&)sdd1);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_spc7110(xml_element &root) {
  has_spc7110 = true;

  foreach(node, root.element) {
    if(node.name == "dcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(spc7110dcu);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mcu") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(spc7110mcu);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "offset") spc7110_data_rom_offset = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(spc7110);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "ram") {
      foreach(attr, node.attribute) {
        if(attr.name == "size") ram_size = hex(attr.content);
      }

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(spc7110ram);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
            if(attr.name == "mode") xml_parse_mode(m, attr.content);
            if(attr.name == "offset") m.offset = hex(attr.content);
            if(attr.name == "size") m.size = hex(attr.content);
          }
          mapping.append(m);
        }
      }
    } else if(node.name == "rtc") {
      has_spc7110rtc = true;

      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(spc7110);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_cx4(xml_element &root) {
  has_cx4 = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m(cx4);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_obc1(xml_element &root) {
  has_obc1 = true;

  foreach(node, root.element) {
    if(node.name == "map") {
      Mapping m(obc1);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_setadsp(xml_element &root) {
  unsigned program = 0;

  foreach(attr, root.attribute) {
    if(attr.name == "program") {
      if(attr.content == "ST-0010") {
        program = 1;
        has_st0010 = true;
      } else if(attr.content == "ST-0011") {
        program = 2;
        has_st0011 = true;
      }
    }
  }

  Memory *map[3] = { 0, &st0010, 0 };

  foreach(node, root.element) {
    if(node.name == "map" && map[program]) {
      Mapping m(*map[program]);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_setarisc(xml_element &root) {
  unsigned program = 0;

  foreach(attr, root.attribute) {
    if(attr.name == "program") {
      if(attr.content == "ST-0018") {
        program = 1;
        has_st0018 = true;
      }
    }
  }

  MMIO *map[2] = { 0, &st0018 };

  foreach(node, root.element) {
    if(node.name == "map" && map[program]) {
      Mapping m(*map[program]);
      foreach(attr, node.attribute) {
        if(attr.name == "address") xml_parse_address(m, attr.content);
      }
      mapping.append(m);
    }
  }
}

void Cartridge::xml_parse_msu1(xml_element &root) {
  has_msu1 = true;

  foreach(node, root.element) {
    if(node.name == "mmio") {
      foreach(leaf, node.element) {
        if(leaf.name == "map") {
          Mapping m(msu1);
          foreach(attr, leaf.attribute) {
            if(attr.name == "address") xml_parse_address(m, attr.content);
          }
          mapping.append(m);
        }
      }
    }
  }
}

void Cartridge::xml_parse_serial(xml_element &root) {
  has_serial = true;
}

void Cartridge::xml_parse_address(Mapping &m, const string &data) {
  lstring part;
  part.split(":", data);
  if(part.size() != 2) return;

  lstring subpart;
  subpart.split("-", part[0]);
  if(subpart.size() == 1) {
    m.banklo = hex(subpart[0]);
    m.bankhi = m.banklo;
  } else if(subpart.size() == 2) {
    m.banklo = hex(subpart[0]);
    m.bankhi = hex(subpart[1]);
  }

  subpart.split("-", part[1]);
  if(subpart.size() == 1) {
    m.addrlo = hex(subpart[0]);
    m.addrhi = m.addrlo;
  } else if(subpart.size() == 2) {
    m.addrlo = hex(subpart[0]);
    m.addrhi = hex(subpart[1]);
  }
}

void Cartridge::xml_parse_mode(Mapping &m, const string& data) {
       if(data == "direct") m.mode = Bus::MapMode::Direct;
  else if(data == "linear") m.mode = Bus::MapMode::Linear;
  else if(data == "shadow") m.mode = Bus::MapMode::Shadow;
}

Cartridge::Mapping::Mapping() {
  memory = 0;
  mmio = 0;
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

Cartridge::Mapping::Mapping(Memory &memory_) {
  memory = &memory_;
  mmio = 0;
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

Cartridge::Mapping::Mapping(MMIO &mmio_) {
  memory = 0;
  mmio = &mmio_;
  mode = Bus::MapMode::Direct;
  banklo = bankhi = addrlo = addrhi = offset = size = 0;
}

#endif
