
void ExportToGRAPHML(Graph& G, ostream& os) {
	if(G.HasHyperarcs())
		throw "The GraphML fileformat doesn\'t support hyperarcs";

	/// write header
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	os << "<!-- www.Open-Graphtheory.org -->\n";
	os << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\">\n";
	os << "\t<graph id=\"g" << G.GetID() << "\">\n";
	
	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "\t\t<node id=\"n" << v.GetID() << "\"/>\n";
	}

	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		if(!e.IsHyperedge()) {
			os << "\t\t<edge directed=\"false\" id=\"e" << e.GetID() << "\" source=\"n"
			   << e.First().GetID() << "\" target=\"n" << e.Second().GetID() << "\"/>\n";
		} else {
			os << "\t\t<hyperedge>\n";
			for(Graph::VertexIterator inc = e.BeginIncidentVertices(); inc != e.EndIncidentVertices(); inc++)
				os << "\t\t\t<endpoint node=\"n" << inc.GetID() << "\"/>\n";
			os << "\t\t</hyperedge>\n";
		}
	}

	/// write arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		os << "\t\t<edge directed=\"true\" id=\"a" << a.GetID() << "\" source=\"n"
		   << a.From().GetID() << "\" target=\"n" << a.To().GetID() << "\"/>\n";
	}

	/// write footer
	os << "\t</graph>\n";
	os << "</graphml>\n";
}
