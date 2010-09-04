#include "../../opengt/opengt.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

void usage(char* argv0);
int main(int argc, char** argv) {
	try {
		Graph G;

		/// select input source
		if(!isatty(fileno(stdin)))
			cin >> G;
		else
			if(argc > 2)
				G.LoadFromFile(argv[2]);
			else
				throw "no input source specified";

		/// select output destination
		ostream *os;
		bool mustdelete = false;
		if(argc > 3) {
			os = new ofstream(argv[3]);
			mustdelete = true;
		} else {
			os = &cout;
		}

		/// export
		string format = (argc > 1) ? argv[1] : "--help";

		*os << "\n";
		if(format == "") {
			// cant happen, just for commenting convenience
//		} else if(format == "mpost") { // todo
//			ExportToMPOST(G, *os);
//		} else if(format == "svg") { // todo
//			ExportToSVG(G, *os);
		} else if(format == "gastex") { // todo
			ExportToGASTEX(G, *os);
//		} else if(format == "latex") { // todo
//			ExportToLATEX(G, *os);
		} else if(format == "vrmlgraph") {
			ExportToVRMLGRAPH(G, *os);
		} else if(format == "tgf") {
			ExportToTGF(G, *os);
		} else if(format == "tei") {
			ExportToTEI(G, *os);
		} else if(format == "gml") {
			ExportToGML(G, *os);
//		} else if(format == "rgml") { // todo
//			ExportToRGML(G, *os);
		} else if(format == "xgmml") {
			ExportToXGMML(G, *os);
		} else if(format == "dot") {
			ExportToDOT(G, *os);
		} else if(format == "graphml") { // todo
			ExportToGRAPHML(G, *os);
		} else if(format == "gxl") { // todo
			ExportToGXL(G, *os);
		} else {
			usage(argv[0]);
		}
		*os << "\n";

		if(mustdelete)
			delete os;
	}
	catch(const char* s) {
		cerr << argv[0] << " ERROR: " << s << "\n";
		usage(argv[0]);
		return 1;
	}
	return 0;
}

void usage(char* argv0) {
	cerr << "usage: " << argv0 << " format [sourcefile] [destinationfile]\n"
	     << "     where format can be\n"
//	     << "          mpost      metapost\n"
//	     << "          svg        scalable vector graphics\n"
	     << "          gastex     latex package \"gastex\", see also jastex\n"
//	     << "          latex      native LaTeX drawing routines\n\n"

	     << "          vrmlgraph  Virtual Reality Modeling Language\n"
	     << "          tgf        trivial graph format\n"
	     << "          tei        text encoding initiative\n"
	     << "          gml        graph modeling language\n"
//	     << "          rgml       resource description framework (RDF) graph modeling language\n"
	     << "          xgmml      extensible graph markup and modeling language\n"
	     << "          dot        graphviz fileformat\n"
	     << "          graphml    \n"
	     << "          gxl        graph exchange language\n"
	;
}