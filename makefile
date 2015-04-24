
DEBUGPARAMS = -O0 -g -m64 -Wall
RELEASEPARAMS = -O3 -march=native -m64 -fexpensive-optimizations
TARGETPARAMS = $(DEBUGPARAMS)
%TARGETPARAMS = $(RELEASEPARAMS)
GCCPARAMS = $(TARGETPARAMS) -Iopengt.so/Headers
GCCPARAMS_LIB = $(GCCPARAMS) -fPIC
GCCPARAMS_BIN = $(GCCPARAMS) -Llib/Release -lopengt

objectsR = obj/Release/Sources/set_helper.o \
           obj/Release/Sources/thread.o \
           obj/Release/Sources/opengt.o \
           obj/Release/Sources/factory.o \
           obj/Release/Sources/simple_xml.o \
           obj/Release/Sources/attributes.o \
           obj/Release/Sources/VertexEdgeFilter.o \
           obj/Release/Sources/visualize/renderer.o \
           obj/Release/Sources/visualize/vectornd.o \
           obj/Release/Sources/visualize/color.o \
           obj/Release/Sources/visualize/vector2d.o \
           obj/Release/Sources/visualize/DisplayWindow.o \
           obj/Release/Sources/visualize/GraphWindow.o \
           obj/Release/Sources/export/stringtranslator.o \
           obj/Release/Sources/export/export.o \
           obj/Release/Sources/export/xgmml.o \
           obj/Release/Sources/export/gastex.o \
           obj/Release/Sources/export/dot.o \
           obj/Release/Sources/export/mpost.o \
           obj/Release/Sources/export/graphml.o \
           obj/Release/Sources/export/tikz.o \
           obj/Release/Sources/export/pdf.o \
           obj/Release/Sources/export/tgf.o \
           obj/Release/Sources/export/gml.o \
           obj/Release/Sources/export/tei.o \
           obj/Release/Sources/export/prolog.o \
           obj/Release/Sources/export/rgml.o \
           obj/Release/Sources/export/povray.o \
           obj/Release/Sources/export/vrmlgraph.o \
           obj/Release/Sources/export/postscript.o \
           obj/Release/Sources/export/svg.o \
           obj/Release/Sources/logic/logic.o \
           obj/Release/Sources/logic/FirstOrderLogic/fo_lexer.o \
           obj/Release/Sources/logic/FirstOrderLogic/fo_parser.o \
           obj/Release/Sources/logic/FirstOrderLogic/fo.o \
           obj/Release/Sources/logic/ComputationTreeLogic/ctl_lexer.o \
           obj/Release/Sources/logic/ComputationTreeLogic/ctl_parser.o \
           obj/Release/Sources/logic/ComputationTreeLogic/ctl.o \
           obj/Release/Sources/logic/ModalLogic/ml_lexer.o \
           obj/Release/Sources/logic/ModalLogic/ml_parser.o \
           obj/Release/Sources/logic/ModalLogic/ml.o \
           obj/Release/Sources/transform/transformation.o \
           obj/Release/Sources/transform/scalefactor.o \
           obj/Release/Sources/transform/aspectscale.o \
           obj/Release/Sources/transform/spring_embedder.o \
           obj/Release/Sources/transform/shift.o \
           obj/Release/Sources/transform/spring_embedder3d.o \
           obj/Release/Sources/transform/scale.o \
           obj/Release/Sources/transform/linear.o \
           obj/Release/Sources/transform/rotate.o \
           obj/Release/Sources/intermediatesteps/intermediate.o \
           obj/Release/Sources/intermediatesteps/stdout.o \
           obj/Release/Sources/intermediatesteps/writefile.o \
           obj/Release/Sources/intermediatesteps/show.o \
           obj/Release/Sources/algorithms/algorithm.o \
           obj/Release/Sources/algorithms/dominatingset/dominatingset.o \
           obj/Release/Sources/algorithms/clique/clique.o \
           obj/Release/Sources/algorithms/path/path.o \
           obj/Release/Sources/algorithms/maximummatching/maximalmatching.o \
           obj/Release/Sources/algorithms/maximummatching/maximummatchingbipartite.o \
           obj/Release/Sources/algorithms/maximummatching/maximummatching.o \
           obj/Release/Sources/algorithms/color/color.o \
           obj/Release/Sources/algorithms/maximumflow/maximumflow.o \
           obj/Release/Sources/algorithms/oddcircle/oddcircle.o \
           obj/Release/Sources/algorithms/minimumcut/minimumcut.o \
           obj/Release/Sources/algorithms/independentset/independentset.o \
           obj/Release/Sources/algorithms/vertexcover/vertexcover.o \
           obj/Release/Sources/algorithms/vertexdisjointpaths/vertexdisjointpaths.o \
           obj/Release/Sources/algorithms/vertexseparator/vertexseparator.o \
           obj/Release/Sources/algorithms/directedtreewidth/directedtreewidth.o \
           obj/Release/Sources/import/import.o \
           obj/Release/Sources/import/gml/gml_lexer.o \
           obj/Release/Sources/import/gml/gml.o \
           obj/Release/Sources/import/xgmml.o \
           obj/Release/Sources/import/graphml.o \
           obj/Release/Sources/import/tikz/tikz_lexer.o \
           obj/Release/Sources/import/tikz/tikz_parser.o \
           obj/Release/Sources/import/tikz/tikz.o \
           obj/Release/Sources/import/tgf.o \
           obj/Release/Sources/import/tei.o \
           obj/Release/Sources/import/rgml.o \
           obj/Release/Sources/import/vrmlgraph.o \
           obj/Release/Sources/import/dot/dot_lexer.o \
           obj/Release/Sources/import/dot/dot_parser.o \
           obj/Release/Sources/import/dot/dot.o \
           obj/Release/Sources/generate/generate.o \
           obj/Release/Sources/generate/random.o \
           obj/Release/Sources/generate/circle.o \
           obj/Release/Sources/generate/grid.o \
           obj/Release/Sources/generate/complete.o \
           obj/Release/Sources/generate/wheel.o \
           obj/Release/Sources/generate/tree.o \
           obj/Release/Sources/generate/star.o \
           obj/Release/Sources/generate/path.o

           
programsR = bin/Release/ogtgenerate \
            bin/Release/ogtsudoku \
            bin/Release/ogtexport \
            bin/Release/ogttransform \
            bin/Release/ogtdisplay \
            bin/Release/ogtlogic \
            bin/Release/ogtalgorithm \
            bin/Release/ogtimport \
            bin/Release/OGoCreator



.PHONY: all
all: lib/Release/libopengt.so $(programsR)



# lib
lib/Release/libopengt.so: $(objectsR)
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -shared -pthread $(objectsR) -lX11
lib/Release/libogographviewplugin.so: programs/OGoCreator/OGoWidget/ogographview.cpp \
					programs/OGoCreator/OGoWidget/ogographviewplugin.cpp \
					lib/Release/libopengt.so
	mkdir -p $(@D)
	cd $(<D); qmake && make # qmake gets confused when it runs outside the project directory



# bin
.PHONY: programs
programs: $(programsR)
bin/Release/ogtalgorithm: obj/Release/ogtalgorithm.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtdisplay: obj/Release/ogtdisplay.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtexport: obj/Release/ogtexport.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtgenerate: obj/Release/ogtgenerate.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtimport: obj/Release/ogtimport.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtlogic: obj/Release/ogtlogic.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogtsudoku: obj/Release/ogtsudoku.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/ogttransform: obj/Release/ogttransform.o
	mkdir -p $(@D)
	g++ -o $@ $< $(GCCPARAMS_BIN)
bin/Release/OGoCreator: programs/OGoCreator/OGoCreator/main.cpp \
			programs/OGoCreator/OGoCreator/mainwindow.cpp \
			programs/OGoCreator/OGoCreator/EditAction.cpp \
			lib/Release/libogographviewplugin.so \
			lib/Release/libopengt.so
	mkdir -p $(@D)
	cd $(<D); qmake && make # qmake gets confused when it runs outside the project directory



# obj
obj/Release/Sources/%.o: opengt.so/Sources/%.cpp
	mkdir -p $(@D)
	g++ $(GCCPARAMS_LIB) -o $@ -c $<
obj/Release/ogtalgorithm.o: programs/ogtalgorithm/ogtalgorithm.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $< 
obj/Release/ogtdisplay.o: programs/ogtdisplay/ogtdisplay.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogtexport.o: programs/ogtexport/ogtexport.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogtgenerate.o: programs/ogtgenerate/ogtgenerate.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogtimport.o: programs/ogtimport/ogtimport.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogtlogic.o: programs/ogtlogic/ogtlogic.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogtsudoku.o: programs/ogtsudoku/ogtsudoku.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<
obj/Release/ogttransform.o: programs/ogttransform/ogttransform.cpp lib/Release/libopengt.so
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<


.PHONY: clean
clean:
	make -C programs/OGoCreator/OGoWidget distclean || true
	make -C programs/OGoCreator/OGoCreator distclean || true
	rm -rf obj bin lib


