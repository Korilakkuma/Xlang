#include "utils.h"

int is_kanji(const int ch) {
  int uch = (unsigned char)ch;

  static int kanji_mode = -1;

  if (kanji_mode == -1) {
    kanji_mode = get_kanji_mode();
  }

  switch (kanji_mode) {
  case SHIFT_JIS:
    return (((uch >= 0x81) && (uch <= 0x9f)) ||
            ((uch >= 0xe0) && (uch <= 0xfc)))
               ? 2
               : 0;
  case EUCJP:
    return (uch == 0x8f) ? 3 : (uch >= 0x80 ? 2 : 0);
  case UTF8:
    return (uch >= 0xe0) ? 3 : (uch >= 0xc0 ? 2 : 0);
  default:
    return 0;
  }
}

int get_kanji_mode(void) {
  static unsigned char kanji[] = "漢";

  if ((kanji[0] == 0x8a) && (kanji[1] == 0xbf)) {
    return SHIFT_JIS;
  }

  if ((kanji[0] == 0xb4) && (kanji[1] == 0xc1)) {
    return EUCJP;
  }

  if ((kanji[0] == 0xe6) && (kanji[1] == 0xbc) && (kanji[2] == 0xa2)) {
    return UTF8;
  }

  return UNKNOWN;
}

char *string_malloc(const char *s) {
  char *p = (char *)malloc(strlen(s) + 1);

  if (p == NULL) {
    fputs("Out of memory (string_malloc)\n", stderr);
    exit(EXIT_FAILURE);
  }

  strcpy(p, s);

  return p;
}

void increment(int *var, int size, const char *error_message) {
  if (*var >= size) {
    display_formatted_error(error_message, size);
    return;
  }

  ++(*var);
}

void display_error(const char *error_message1, const char *error_message2) {
  extern int counter_for_error;

  static int prev_error_lineno = -1;

  if (prev_error_lineno == get_line_number()) {
    return;
  }

  if (++counter_for_error > NUMBER_OF_ERROR_MESSAGES) {
    fprintf(stderr, "number of errors exceeds (over %d)\n",
            NUMBER_OF_ERROR_MESSAGES);
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "#%d error: ", get_line_number());

  if (*error_message1 == *error_message2) {
    fprintf(stderr, "%s\n", error_message1);
  } else {
    fprintf(stderr, "%s (%s)\n", error_message1, error_message2);
  }

  prev_error_lineno = get_line_number();
}

void display_single_error(const char *error_message) {
  display_error(error_message, "");
}

void display_formatted_error(const char *format, int id) {
  char error_message[100];

  sprintf(error_message, format, id);

  display_error(error_message, "");
}
