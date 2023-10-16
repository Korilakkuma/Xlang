#include "processor.h"

PROCESSORS get_processor_code(void) {
  struct utsname name;

  if (uname(&name) == -1) {
    perror("uname");
    return Others;
  }

  const char *machine = name.machine;

  // 64 bits
  if (strncmp(machine, "x64", 3) == 0) {
    return Intel64;
  }

  if (strncmp(machine, "x86_64", 6) == 0) {
    return Intel64;
  }

  if (strncmp(machine, "arm64", 6) == 0) {
    return ARM64;
  }

  if (strncmp(machine, "aarch64", 7) == 0) {
    return ARM64;
  }

  // 32 bits
  if (strncmp(machine, "x86", 3) == 0) {
    return Intel32;
  }

  if (strncmp(machine, "arm", 3) == 0) {
    return ARM32;
  }

  if (strncmp(machine, "aarch32", 7) == 0) {
    return ARM32;
  }

  return Others;
}

const char *get_processor_enum_name(PROCESSORS processor_code) {
  return processor_enum_names[processor_code];
}

void get_registers(REGISTERS registers) {
  PROCESSORS processor_code = get_processor_code();

  for (int i = 0; i < 4; i++) {
    const char *r = registers_map[processor_code][i];

    strncpy(registers[i], r, (strlen(r) + 1));
  }
}

void get_r0_register_name(char *r0) {
  PROCESSORS processor_code = get_processor_code();

  strncpy(r0, registers_map[processor_code][0], (strlen(registers_map[processor_code][0]) + 1));
}
