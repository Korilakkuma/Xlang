#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

typedef enum {
  Intel32 = 0,
  Intel64,
  ARM32,
  ARM64,
  Others
} PROCESSORS;

// 4 registers x 5 processors x string (char [])
typedef char REGISTERS[5][4];
typedef REGISTERS REGISTERS_MAP[];

static const char *processor_enum_names[] = {
  "Intel32",
  "Intel64",
  "ARM32",
  "ARM64",
  "Others"
};

static const REGISTERS_MAP registers_map = {
  { "eax", "ebx", "ecx", "edx" },
  { "rax", "rbx", "rcx", "rdx" },
  { "r0",  "r1",  "r2",  "r3"  },
  { "w0",  "w1",  "w2",  "w3"  },
  { "w0",  "w1",  "w2",  "w3"  },
  { "",    "",    "",    ""    }
};
