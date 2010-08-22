
void ExportToTEI(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The TEI fileformat doesn\'t support hypergraphs\n";
	if(G.Mixed())
		throw "The TEI fileformat doesn\'t support mixed graphs\n";
	bool directed = G.Directed();

	os << "<!-- www.Open-Graphtheory.org -->\n";
	os << "<graph xml:id=\"" << G.GetID() << "\" type=\"" << (directed?"":"un") << "directed\">\n";
	os << "\t<label>" << G.GetLabel() << "</label>\n";
	
	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "\t<node xml:id=\"" << v.GetID() << "\">\n";
		os << "\t\t<label>" << v.GetLabel() << "</label>\n";
		os << "\t</node>\n";
	}

	/// since mixed graphs are excluded, only one loop generates output
	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		os << "\t<arc from=\"#" << e.First().GetID() << "\" to=\"#" << e.Second().GetID() << "\">\n";
		os << "\t\t<label>" << e.GetLabel() << "</label>\n";
		os << "\t</arc>\n";
	}

	/// or arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		os << "\t<arc from=\"#" << a.From().GetID() << "\" to=\"#" << a.To().GetID() << "\">\n";
		os << "\t\t<label>" << a.GetLabel() << "</label>\n";
		os << "\t</arc>\n";
	}

	os << "</graph>\n";
}
