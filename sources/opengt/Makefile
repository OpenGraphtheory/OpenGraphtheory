all: opengt.o simple_xml.o

opengt.o: opengt.h opengt.cpp
	g++ -O3 -c opengt.cpp

simple_xml.o: simple_xml.cpp simple_xml.h
	g++ -O3 -c simple_xml.cpp

.PHONY: force
force:
	touch simple_xml.cpp
	touch opengt.cpp
	make