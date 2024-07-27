#include "generator.h"

static void generate_local_variables(struct Node *node);

INT label = 0;

void generate(struct Node *node) {
  if (node == NULL) {
    return;
  }

  // PROCESSORS processor_code = get_processor_code();

  REGISTERS registers;

  get_registers(registers);

  if ((node->type == NODE_IF) || (node->type == NODE_ELSE)) {
    fprintf(stdout, "		pop rax\n");
    fprintf(stdout, "		cmp rax, 0\n");
    fprintf(stdout, "		je .Lelse%ld:\n", label);

    generate(node->then);

    fprintf(stdout, "		jmp .Lend%ld:\n", (label + 1));
    fprintf(stdout, ".Lelse%ld:\n", label);

    generate(node->el);

    fprintf(stdout, ".Lend%ld:\n", (label + 1));

    ++label;

    return;
  }


  if (node->type == NODE_IF) {
    fprintf(stdout, "		pop rax\n");
    fprintf(stdout, "		cmp rax, 0\n");
    fprintf(stdout, "		je .Lend%ld:\n", label);
    fprintf(stdout, ".Lend%ld:\n", label);

    ++label;

    return;
  }

  if (node->type == NODE_WHILE) {
    fprintf(stdout, ".Lbegin%ld:\n", label);
    fprintf(stdout, "		pop rax\n");
    fprintf(stdout, "		cmp rax, 0\n");
    fprintf(stdout, "		je .Lend%ld:\n", label);

    generate(node->loop_statement);

    fprintf(stdout, "		jmp .Lbegin%ld:\n", label);
    fprintf(stdout, ".Lend%ld:\n", label);

    ++label;

    return;
  }

  if (node->type == NODE_FOR) {
    generate(node->init_statement);

    fprintf(stdout, ".Lbegin%ld:\n", label);
    fprintf(stdout, "		pop rax\n");
    fprintf(stdout, "		cmp rax, 0\n");
    fprintf(stdout, "		je .Lend%ld:\n", label);

    generate(node->loop_statement);
    generate(node->update_statement);

    fprintf(stdout, "		jmp .Lbegin%ld:\n", label);
    fprintf(stdout, ".Lend%ld:\n", label);

    ++label;

    return;
  }

  if (node->type == NODE_RETURN) {
    generate(node->left);

    fprintf(stdout, "		pop rax\n");
    fprintf(stdout, "		mov rsp, rbp\n");
    fprintf(stdout, "		pop rbp\n");
    fprintf(stdout, "		ret\n");

    return;
  }

  switch (node->type) {
    case NODE_NUMBER: {
      fprintf(stdout, "		push %ld\n", node->value);
      return;
    }

    case NODE_LOCAL_VARIABLE: {
      generate_local_variables(node);

      fprintf(stdout, "		pop rax\n");
      fprintf(stdout, "		mov rax, [rax]\n");
      fprintf(stdout, "		push rax\n");
      return;
    }

    case NODE_ASSIGN: {
      generate_local_variables(node->left);
      generate(node->right);

      fprintf(stdout, "		pop rdi\n");  // Right value
      fprintf(stdout, "		pop rax\n");  // Left value
      fprintf(stdout, "		mov [rax], rdi\n"); // Left value (variable) = Right value
      fprintf(stdout, "		push rax\n");
      return;
    }

    default: {
      break;
    }
  }

  generate(node->left);
  generate(node->right);

  fprintf(stdout, "		 pop rdi\n");
  fprintf(stdout, "		 pop %s\n", registers[0]);

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

    default: {
      break;
    }
  }

  fprintf(stdout, "		push %s\n", registers[0]);
}

static void generate_local_variables(struct Node *node) {
  if (node->type != NODE_LOCAL_VARIABLE) {
    fprintf(stderr, "variable (%d) is not left value", node->type);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "		mov rax, rbp\n");
  fprintf(stdout, "		sub rax, %ld\n", node->offset);
  fprintf(stdout, "		push rax\n");
}
