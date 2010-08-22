gen_completegraph: gen_completegraph.cpp ../../../opengt/simple_xml.o ../../../opengt/opengt.o
	g++ -O3 -o gen_completegraph \
		../../../opengt/simple_xml.o \
		../../../opengt/opengt.o \
		gen_completegraph.cpp

.PHONY: force
force:
	touch gen_completegraph.cpp
	make
