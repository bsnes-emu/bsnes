#include <../base.hpp>

#define SMPCORE_CPP
namespace SNES {

#include "opcode_algorithms.cpp"
#include "opcode_functions.cpp"
#include "opcode_table.cpp"
#include "disasm/disasm.cpp"

SMPcore::SMPcore() {
  initialize_opcode_table();
}

};

