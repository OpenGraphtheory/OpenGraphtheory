display: display.cpp vector2d.h DisplayWindow.h ../../../opengt/simple_xml.o ../../../opengt/opengt.o
	g++ -o display -O3 -lpthread -lX11 \
		../../../opengt/simple_xml.o \
		../../../opengt/opengt.o \
		display.cpp

.PHONY: force
force:
	touch display.cpp
	make
