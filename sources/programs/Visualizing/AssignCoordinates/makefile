assign_coordinates: assign_coordinates.cpp ../../../opengt/simple_xml.o ../../../opengt/opengt.o
	g++ -O3 -o assign_coordinates \
		../../../opengt/simple_xml.o \
		../../../opengt/opengt.o \
		assign_coordinates.cpp

.PHONY: force
force:
	touch assign_coordinates.cpp
	make
