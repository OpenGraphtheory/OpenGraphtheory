gen_random: gen_random.cpp ../../../opengt/simple_xml.o ../../../opengt/opengt.o
	g++ -O3 -o gen_random \
		../../../opengt/simple_xml.o \
		../../../opengt/opengt.o \
		gen_random.cpp

.PHONY: force
force:
	touch gen_random.cpp
	make
