#include "parser.h"
#include "../../utils/processor.c"

void generate(struct Node *node, PROCESSORS processor_code, const char *r0, const char *r3) {
  if (node->type == NODE_NUMBER) {
    fprintf(stdout, "		push %ld\n", node->value);
    return;
  }

  generate(node->left, processor_code, r0, r3);
  generate(node->right, processor_code,  r0, r3);

  fprintf(stdout, "		pop %s\n", r3);
  fprintf(stdout, "		pop %s\n", r0);

  switch (node->type) {
    case NODE_ADD: {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		add %s, %s\n", r0, r3);
      } else {
        fprintf(stdout, "		add %s, %s\n", r0, r3);
      }

      break;
    }

    case NODE_SUB: {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		sub %s, %s\n", r0, r3);
      } else {
        fprintf(stdout, "		sub %s, %s\n", r0, r3);
      }

      break;
    }

    case NODE_MUL: {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		imul %s, %s\n", r0, r3);
      } else {
        fprintf(stdout, "		imul %s, %s\n", r0, r3);
      }

      break;
    }

    case NODE_DIV: {
      if ((processor_code == Intel32) || (processor_code == Intel64)) {
        fprintf(stdout, "		cqo\n");
        fprintf(stdout, "		idiv %s\n", r3);
      } else {
        fprintf(stdout, "		cqo\n");
        fprintf(stdout, "		idiv %s\n", r3);
      }

      break;
    }
  }

  fprintf(stdout, "		push %s\n", r0);
}
