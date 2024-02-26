#include "generator.h"

void generate(struct Node *node) {
  if (node == NULL) {
    return;
  }

  // PROCESSORS processor_code = get_processor_code();

  REGISTERS registers;

  get_registers(registers);

  if (node->type == NODE_NUMBER) {
    fprintf(stdout, "		push %ld\n", node->value);
    return;
  }

  generate(node->left);
  generate(node->right);

  fprintf(stdout, "		pop rdi\n");
  fprintf(stdout, "		pop %s\n", registers[0]);

  switch (node->type) {
    case NODE_ADD: {
      fprintf(stdout, "		add %s, rdi\n", registers[0]);
      break;
    }

    case NODE_SUB: {
      fprintf(stdout, "		sub %s, rdi\n", registers[0]);
      break;
    }

    case NODE_MUL: {
      fprintf(stdout, "		imul %s, rdi\n", registers[0]);
      break;
    }

    case NODE_DIV: {
      fprintf(stdout, "		cqo\n");
      fprintf(stdout, "		idiv rdi\n");
      break;
    }

    case NODE_NUMBER: {
      break;
    }
  }

  fprintf(stdout, "		push %s\n", registers[0]);
}
