
void ExportToGXL(Graph& G, ostream& os) {
	/// write header
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	os << "<!DOCTYPE gxl SYSTEM \"http://www.gupro.de/GXL/gxl-1.0.dtd\">\n";
	os << "<!-- www.open-graphtheory.org -->\n";
	os << "<gxl xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n";
	os << "\t<graph id=\"g" << G.GetID() << "\" edgeids=\"true\" hypergraph=\"true\" edgemode=\"defaultundirected\">\n";

	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "\t\t<node id=\"n" << v.GetID() << "\"/>\n";
	}

	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		if(!e.IsHyperedge()) {
			os << "\t\t<edge id=\"e" << e.GetID() << "\" isdirected=\"false\" from=\"n"
			   << e.First().GetID() << "\" to=\"n" << e.Second().GetID() << "\"/>\n";
		} else {
			os << "\t\t<rel id=\"e" << e.GetID() << "\" isdirected=\"false\">\n";
			for(Graph::VertexIterator inc = e.BeginIncidentVertices(); inc != e.EndIncidentVertices(); inc++)
				os << "\t\t\t<relend target=\"n" << inc.GetID() << "\" direction=\"none\"/>\n";
			os << "\t\t</rel>\n";
		}
	}

	/// write arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		if(!a.IsHyperarc()) {
			os << "\t\t<edge id=\"a" << a.GetID() << "\" isdirected=\"true\" from=\"n"
			   << a.From().GetID() << "\" to=\"n" << a.To().GetID() << "\"/>\n";
		} else {
			os << "\t\t<rel id=\"a" << a.GetID() << "\" isdirected=\"true\">\n";
			for(Graph::VertexIterator inc = a.BeginNegativeIncidentVertices(); inc != a.EndNegativeIncidentVertices(); inc++)
				os << "\t\t\t<relend target=\"n" << inc.GetID() << "\" direction=\"in\"/>\n";
			for(Graph::VertexIterator inc = a.BeginPositiveIncidentVertices(); inc != a.EndPositiveIncidentVertices(); inc++)
				os << "\t\t\t<relend target=\"n" << inc.GetID() << "\" direction=\"out\"/>\n";
			os << "\t\t</rel>\n";
			
		}
	}

	/// write footer
	os << "\t</graph>\n";
	os << "</gxl>\n";
}
