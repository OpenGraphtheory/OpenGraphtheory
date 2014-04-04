all: tikz_lexer.cpp tikz_parser.cpp

tikz_lexer.cpp: tikz_lexer.l
	flex -o tikz_lexer.cpp tikz_lexer.l

tikz_parser.cpp: tikz_parser.yy
	bison -o tikz_parser.cpp tikz_parser.yy

.PHONY: clean
clean:
	rm tikz_lexer.cpp tikz_parser.cpp tikz_parser.hpp location.hh position.hh stack.hh
