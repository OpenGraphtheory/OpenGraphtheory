all: gml_lexer.cpp

gml_lexer.cpp: gml_lexer.l
	flex -o gml_lexer.cpp gml_lexer.l

.PHONY: clean
clean:
	rm gml_lexer.cpp
