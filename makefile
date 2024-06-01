all: main cleanpl

main: bison flex
	gcc -o main main.c parser.tab.c lex.yy.c AST.c

flex:
	flex lexer.l

bison:
	bison -d parser.y

clean: cleanpl
	rm -f main

cleanpl:
	rm -f parser.tab.c parser.tab.h lex.yy.c

