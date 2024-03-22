#include "../includes.h"
#include "../types.h"

/*
static const char *processor_enum_names[] = {
  "Intel32",
  "Intel64",
  "ARM32",
  "ARM64",
  "Others"
};
*/

static const REGISTERS_MAP registers_map = {
  { "eax", "ebx", "ecx", "edx" },
  { "rax", "rbx", "rcx", "rdx" },
  { "r0",  "r1",  "r2",  "r3"  },
  { "w0",  "w1",  "w2",  "w3"  },
  { "w0",  "w1",  "w2",  "w3"  },
  { "",    "",    "",    ""    }
};

PROCESSORS get_processor_code(void);
const char *get_processor_enum_name(PROCESSORS processor_code);
void get_registers(REGISTERS registers);
void get_r0_register_name(char *r0);
