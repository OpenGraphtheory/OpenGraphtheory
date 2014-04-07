all: dot_lexer.cpp dot_parser.cpp

dot_lexer.cpp: dot_lexer.l
	flex -o dot_lexer.cpp dot_lexer.l

dot_parser.cpp: dot_parser.yy
	bison -o dot_parser.cpp dot_parser.yy

.PHONY: clean
clean:
	rm dot_lexer.cpp dot_parser.cpp dot_parser.hpp location.hh position.hh stack.hh
