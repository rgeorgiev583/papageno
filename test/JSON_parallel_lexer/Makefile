CC := gcc 
LFLAGS = -lrt -lpthread 
INCLUDES := -I./include -I./lexer -I../include
CFLAGS := -O3 -pipe -g -march=native -UDEBUG -Wall $(INCLUDES)
#CFLAGS := -O3 -pipe -lpthread -Wall $(INCLUDES)
SRCDIR := lib
SRC := $(wildcard $(SRCDIR)/*.c)
ODIR := obj

FOUT := lexer/flex.yy.c
FOBJ := obj/flex.yy.o
OBJ := $(patsubst $(SRCDIR)/%.c, $(ODIR)/%.o, $(SRC))

FLEX := lexer/par_json.l
GENERATED_FILES = include/rewrite_rules.h include/reduction_tree.h include/grammar_tokens.h include/grammar_semantics.h lib/grammar_semantics.c include/grammar.h lib/grammar.c include/matrix.h 

all: $(FOBJ) $(OBJ)
	@gcc $(OBJ) $(FOBJ) $(LFLAGS) -o bin/json_parser
	strip bin/json_parser

$(FOBJ): $(FOUT)
	$(CC) -c $< -o $@ $(CFLAGS)

$(FOUT): $(FLEX)
	flex -Pyy --header-file=lexer/flex.yy.h -o $@ $<

$(ODIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	@rm -f $(FOUT)
	@rm -f $(patsubst %.c, %.h, $(FOUT))
	@rm -f $(ODIR)/*.o
	@rm -f bin/json_parser

clean-gen:
	@rm $(GENERATED_FILES)
