all: ml_lexer.cpp ml_parser.cpp

ml_lexer.cpp: ml_lexer.l
	flex -o ml_lexer.cpp ml_lexer.l

ml_parser.cpp: ml_parser.yy
	bison -o ml_parser.cpp ml_parser.yy

.PHONY: clean
clean:
	rm ml_lexer.cpp
	rm ml_parser.cpp
