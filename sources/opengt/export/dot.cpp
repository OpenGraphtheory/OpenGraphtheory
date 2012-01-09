
void ExportToDOT(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The DOT fileformat doesn\'t support hypergraphs\n";

	os << "// www.open-graphtheory.org\n";
	os << "graph ogt2dot_" << G.GetID() << " {\n";

	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "\tv" << v.GetID() << " [ label=\"" << v.GetLabel() << "\", "
		   << "pos=\"" << v.GetX() << "," << v.GetY() << "\" ];\n";
	}

	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		os << "\tv" << e.First().GetID() << " -- v" << e.Second().GetID()
		   << " [ label=\"" << e.GetLabel() << "\" dir=\"none\" ];\n";
	}

	/// or arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		os << "\tv" << a.From().GetID() << " -> v" << a.To().GetID()
		   << " [ label=\"" << a.GetLabel() << "\" dir=\"forward\" ];\n";
	}

	os << "}\n";
}
