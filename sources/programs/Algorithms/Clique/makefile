clique: clique.cpp ../../../opengt/opengt.o ../../../opengt/simple_xml.o
	g++ -O3 -pthread -o clique \
	       	../../../opengt/opengt.o \
		../../../opengt/simple_xml.o \
		clique.cpp

.PHONY: force
force:
	touch clique.cpp
	make
