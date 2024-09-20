all: parser

# Rule for generating the parser code from the Bison grammar
parser.tab.c parser.tab.h: parser.y
	@bison -t -v -d parser.y

# Rule for generating the lexer code from the Flex input file
lex.yy.c: lexer.l parser.tab.h
	@flex lexer.l

# Compile everything directly into the final executable
parser: lex.yy.c parser.tab.c parser.tab.h symbolTable.c main.c
	@gcc -o parser parser.tab.c lex.yy.c symbolTable.c main.c
	@./parser input.txt

# Clean up all generated files
clean:
	@rm -f parser parser.tab.c lex.yy.c parser.tab.h parser.output output.s
	@ls -l
