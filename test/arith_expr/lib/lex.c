#include "lex.h"
struct lex_token* flex_token;


void compute_alloc_realloc_size(FILE *input_file, uint32_t *alloc_size, uint32_t *realloc_size)
{
  fseek(input_file, 0, SEEK_END);
  *alloc_size = ((float) ftell(input_file)) / TOKEN_SIZE;
  *realloc_size = *alloc_size/10;
  fseek(input_file, 0, SEEK_SET);
}

void append_token_node(lex_token *token, token_node **token_builder, parsing_ctx *ctx, token_node_stack *stack, uint32_t realloc_size)
{
  token_node *tok = push_token_node_on_stack(stack, token->token, token->semantic_value, realloc_size);
  if (ctx->token_list == NULL) {
    ctx->token_list = tok;
    *token_builder = tok;
  } else {
    (*token_builder)->next = tok;
    *token_builder = tok;
  }
}

void perform_lexing(char *file_name, parsing_ctx *ctx)
{
  uint32_t token_list_length = 0, alloc_size = 0, realloc_size = 0, i;
  int8_t flex_return_code;
  token_node *token_builder = NULL;
  token_node_stack stack;

  yyin = fopen(file_name, "r");
  if (yyin == NULL) {
    DEBUG_STDOUT_PRINT("ERROR> could not open input file. Aborting.\n")
    exit(1);
  }

  compute_alloc_realloc_size(yyin, &alloc_size, &realloc_size);
  DEBUG_STDOUT_PRINT("LEXER> alloc_size %d, realloc_size %d\n", alloc_size, realloc_size)
  ctx->token_list = NULL;
  init_token_node_stack(&stack, alloc_size);
  flex_token = (lex_token*) malloc(sizeof(lex_token));
  flex_return_code = yylex();
  while (flex_return_code != END_OF_FILE) {
    if (flex_return_code == LEX_CORRECT) {
      append_token_node(flex_token, &token_builder, ctx, &stack, realloc_size);
      ++token_list_length;
      flex_return_code = yylex();
    }
    else {//flex_return_code is ERROR
      DEBUG_STDOUT_PRINT("Lexer scanned erroneous input. Abort.\n")
      fclose(yyin);
      exit(1);
    }
  }

  ctx->token_list_length = token_list_length;
  fclose(yyin);

  //Empty input file (only with spaces)
  if(ctx->token_list_length == 0) {
    fprintf(stdout, "Input file is empty. Exit.\n");
    exit(1);
  }

  #ifdef DEBUG
  DEBUG_STDOUT_PRINT("ctx->token_list_length is %d\n", ctx->token_list_length)
  token_node * temp = ctx->token_list;
  for(i = 0; i<ctx->token_list_length; i++)
  {
    if (temp->token == NUMBER)
      DEBUG_STDOUT_PRINT("token number %d is %s = %x with semantic_value = %d\n", i, gr_token_to_string(temp->token), temp->token, *((uint32_t*)temp->value))
    else
      DEBUG_STDOUT_PRINT("token number %d is %s = %x with semantic_value = %c\n", i, gr_token_to_string(temp->token), temp->token, *((char*)temp->value))
    temp = temp->next;
  }
  #endif
}