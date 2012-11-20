all: ctl_lexer.cpp ctl_parser.cpp

ctl_lexer.cpp: ctl_lexer.l
	lex -o ctl_lexer.cpp ctl_lexer.l

ctl_parser.cpp: ctl_parser.yy
	bison -o ctl_parser.cpp ctl_parser.yy

.PHONY: clean
clean:
	rm ctl_lexer.cpp
	rm ctl_parser.cpp
