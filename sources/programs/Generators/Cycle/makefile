gen_cycle: gen_cycle.cpp ../../../opengt/simple_xml.o ../../../opengt/opengt.o
	g++ -O3 -o gen_cycle \
		../../../opengt/simple_xml.o \
		../../../opengt/opengt.o \
		gen_cycle.cpp

.PHONY: force
force:
	touch gen_cycle.cpp
	make
