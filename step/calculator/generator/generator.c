#include "generator.h"

void generate(struct Node *node) {
  if (node == NULL) {
    return;
  }

  if (node->type == NODE_NUMBER) {
    fprintf(stdout, "		push %ld\n", node->value);
    return;
  }

  generate(node->left);
  generate(node->right);

  fprintf(stdout, "		pop rdi\n");
  fprintf(stdout, "		pop rax\n");

  switch (node->type) {
    case NODE_ADD: {
      fprintf(stdout, "		add rax, rdi\n");
      break;
    }

    case NODE_SUB: {
      fprintf(stdout, "		sub rax, rdi\n");
      break;
    }

    case NODE_MUL: {
      fprintf(stdout, "		imul rax, rdi\n");
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

  fprintf(stdout, "		push rax\n");
}
