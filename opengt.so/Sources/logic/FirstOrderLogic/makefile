all: fo_lexer.cpp fo_parser.cpp

fo_lexer.cpp: fo_lexer.l
	flex -o fo_lexer.cpp fo_lexer.l

fo_parser.cpp: fo_parser.yy
	bison -o fo_parser.cpp fo_parser.yy

.PHONY: clean
clean:
	rm fo_lexer.cpp fo_parser.cpp fo_parser.hpp location.hh position.hh stack.hh
