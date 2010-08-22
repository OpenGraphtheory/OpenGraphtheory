export: export.cpp ../../opengt/opengt.o ../../opengt/simple_xml.o
	g++ -O3 -o export \
		../../opengt/opengt.o \
		../../opengt/simple_xml.o \
		export.cpp

.PHONY: force
force:
	touch export.cpp
	make
