#
# makefile for the www.Open-Graphtheory.org C++ Framework for Graph-Theory
# Viktor Engelmann
#

# required packages:
#    flex
#    bison
#    libX11-dev (Ubuntu)
#


DEBUGPARAMS           := -O0 -g -Wall
RELEASEPARAMS         := -O3 -fexpensive-optimizations
GCCPARAMS             := -Iopengt.so/Headers
GCCPARAMS_LIB         := $(GCCPARAMS) -fPIC
GCCPARAMS_BIN_RELEASE := $(GCCPARAMS) -Llib/Release -lopengt
GCCPARAMS_BIN_DEBUG   := $(GCCPARAMS) -Llib/Debug -lopengtdbg




MODULES       := $(subst opengt.so/Sources/,,$(shell find opengt.so/Sources -mindepth 1 -maxdepth 1 -type d -not -name "visualize"))
MODULE_CPP    := $(foreach module,$(MODULES),opengt.so/Sources/$(module)/$(module).cpp)

BISON_YY      := $(shell find opengt.so/Sources -name '*.yy')
BISON_CPP     := $(BISON_YY:%.yy=%.cpp)

FLEX_L        := $(shell find opengt.so/Sources -name '*.l')
FLEX_CPP      := $(FLEX_L:%.l=%.cpp)

SOURCES       := $(MODULE_CPP) $(BISON_CPP) $(FLEX_CPP) \
                 $(filter-out $(MODULE_CPP) $(BISON_CPP) $(FLEX_CPP),$(shell find opengt.so/Sources -depth -name '*.cpp'))

PROGRAMS_ALL   := $(subst programs/,,$(shell find programs -mindepth 1 -maxdepth 1 -type d))
PROGRAMS_NONQT := $(filter-out OGoCreator,$(PROGRAMS_ALL))




soobjRelease  := $(subst opengt.so/, obj/Release/, $(SOURCES:%.cpp=%.o))
objRelease    := $(soobjRelease) $(foreach program,$(PROGRAMS_NONQT),obj/Release/$(program).o)
soobjDebug    := $(subst opengt.so/, obj/Debug/, $(SOURCES:%.cpp=%.o))
objDebug      := $(soobjDebug) $(foreach program,$(PROGRAMS_NONQT),obj/Debug/$(program).o)

binRelease    := $(foreach program,$(PROGRAMS_ALL),bin/Release/$(program))
binDebug      := $(foreach program,$(PROGRAMS_ALL),bin/Debug/$(program))



.PHONY: debug
debug: $(binDebug)
.PHONY: release
release: $(binRelease)
.PHONY: all
all: debug release




# bison and flex
.PRECIOUS: opengt.so/Sources/%.cpp
opengt.so/Sources/%.cpp: opengt.so/Sources/%.yy
	bison -o $@ $<
opengt.so/Sources/%.cpp: opengt.so/Sources/%.l
	flex -o $@ $<




# obj
obj/Release/Sources/%.o: opengt.so/Sources/%.cpp
	mkdir -p $(@D)
	g++ $(RELEASEPARAMS) $(GCCPARAMS_LIB) -o $@ -c $<
obj/Debug/Sources/%.o: opengt.so/Sources/%.cpp
	mkdir -p $(@D)
	g++ $(DEBUGPARAMS) $(GCCPARAMS_LIB) -o $@ -c $<
define PROGRAM_OBJ_template
obj/Release/$(1).o: programs/$(1)/$(1).cpp lib/Release/libopengt.so
	mkdir -p $$(@D)
	g++ $(RELEASEPARAMS) $(GCCPARAMS) -o $$@ -c $$< 
obj/Debug/$(1).o: programs/$(1)/$(1).cpp lib/Debug/libopengtdbg.so
	mkdir -p $$(@D)
	g++ $(DEBUGPARAMS) $(GCCPARAMS) -o $$@ -c $$< 
endef
$(foreach prog,$(PROGRAMS_NONQT),$(eval $(call PROGRAM_OBJ_template,$(prog))))




# lib
.PRECIOUS: lib/Release/libopengt.so lib/Debug/libopengtdbg.so
lib/Release/libopengt.so: $(soobjRelease)
	mkdir -p $(@D)
	g++ $(RELEASEPARAMS) $(GCCPARAMS) -o $@ -shared -pthread $(soobjRelease) -lX11
lib/Debug/libopengtdbg.so: $(soobjDebug)
	mkdir -p $(@D)
	g++ $(DEBUGPARAMS) $(GCCPARAMS) -o $@ -shared -pthread $(soobjDebug) -lX11
lib/Release/libogographviewplugin.so: programs/OGoCreator/OGoWidget/ogographview.cpp \
					programs/OGoCreator/OGoWidget/ogographviewplugin.cpp \
					lib/Release/libopengt.so
	mkdir -p $(@D)
	cd $(<D); qmake && make # qmake gets confused when it runs outside the project directory
lib/Debug/libogographviewplugindbg.so: programs/OGoCreator/OGoWidget/ogographview.cpp \
					programs/OGoCreator/OGoWidget/ogographviewplugin.cpp \
					lib/Debug/libopengtdbg.so
	mkdir -p $(@D)
	cd $(<D); qmake -config debug && make # qmake gets confused when it runs outside the project directory




# bin
define PROGRAM_template
bin/Release/$(1): obj/Release/$(1).o
	mkdir -p $$(@D)
	g++ -o $$@ $$< $(RELEASEPARAMS) $(GCCPARAMS_BIN_RELEASE)
bin/Debug/$(1): obj/Debug/$(1).o
	mkdir -p $$(@D)
	g++ -o $$@ $$< $(DEBUGPARAMS) $(GCCPARAMS_BIN_DEBUG)
endef
$(foreach prog,$(PROGRAMS_NONQT),$(eval $(call PROGRAM_template,$(prog))))
bin/Release/OGoCreator: programs/OGoCreator/OGoCreator/main.cpp \
			programs/OGoCreator/OGoCreator/mainwindow.cpp \
			programs/OGoCreator/OGoCreator/EditAction.cpp \
			lib/Release/libogographviewplugin.so \
			lib/Release/libopengt.so
	mkdir -p $(@D)
	cd $(<D); qmake && make # qmake gets confused when it runs outside the project directory
bin/Debug/OGoCreator: programs/OGoCreator/OGoCreator/main.cpp \
			programs/OGoCreator/OGoCreator/mainwindow.cpp \
			programs/OGoCreator/OGoCreator/EditAction.cpp \
			lib/Debug/libogographviewplugindbg.so \
			lib/Debug/libopengtdbg.so
	mkdir -p $(@D)
	cd $(<D); qmake -config debug && make # qmake gets confused when it runs outside the project directory




.PHONY: clean
clean:
	$(shell find opengt.so/Sources -name '*_lexer.cpp' -delete)
	$(shell find opengt.so/Sources -name '*_parser.cpp' -delete)
	$(shell find opengt.so/Sources -name '*_parser.hpp' -delete)
	$(shell find opengt.so/Sources -name 'location.hh' -delete)
	$(shell find opengt.so/Sources -name 'position.hh' -delete)
	$(shell find opengt.so/Sources -name 'stack.hh' -delete)
	make -C programs/OGoCreator/OGoWidget distclean || true
	make -C programs/OGoCreator/OGoCreator distclean || true
	rm -rf obj bin lib
