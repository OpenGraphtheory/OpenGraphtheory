all: opengt.o simple_xml.o thread.o

opengt.o: opengt.h opengt.cpp
	g++ -O3 -c opengt.cpp

simple_xml.o: simple_xml.cpp simple_xml.h
	g++ -O3 -c simple_xml.cpp

thread.o: thread.cpp thread.h
	g++ -O3 -c thread.cpp

.PHONY: force
force:
	touch simple_xml.cpp
	touch opengt.cpp
	touch thread.cpp
	make