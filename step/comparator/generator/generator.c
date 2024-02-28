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

    case NODE_EQ: {
      fprintf(stdout, "		cmp %s, rdi\n", registers[0]);
      fprintf(stdout, "		sete al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_NOT_EQ: {
      fprintf(stdout, "		cmp %s, rdi\n", registers[0]);
      fprintf(stdout, "		setne al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_LESS: {
      fprintf(stdout, "		cmp %s, rdi\n", registers[0]);
      fprintf(stdout, "		setl al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_LESS_OR_EQUAL: {
      fprintf(stdout, "		cmp %s, rdi\n", registers[0]);
      fprintf(stdout, "		setle al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_GREATER: {
      fprintf(stdout, "		cmp rdi, %s\n", registers[0]);
      fprintf(stdout, "		setl al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_GREATER_OR_EQUAL: {
      fprintf(stdout, "		cmp rdi, %s\n", registers[0]);
      fprintf(stdout, "		setle al\n");
      fprintf(stdout, "		movzb %s, al\n", registers[0]);
      break;
    }

    case NODE_NUMBER: {
      break;
    }
  }

  fprintf(stdout, "		push %s\n", registers[0]);
}
