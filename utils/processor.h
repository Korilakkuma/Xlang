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

static const char *processor_enum_names[] = {
  "Intel32",
  "Intel64",
  "ARM32",
  "ARM64",
  "Others"
};
