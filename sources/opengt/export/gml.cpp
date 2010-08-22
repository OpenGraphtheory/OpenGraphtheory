
void ExportToGML(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The GML fileformat doesn\'t support hypergraphs\n";

	os << "# www.Open-Graphtheory.org\n";
	os << "graph [ id " << G.GetID() << " label \"" << G.GetLabel() << "\"\n";

	/// write vertices: node [ id label graphics [ x y ] ]
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "\tnode [ id " << v.GetID() << " label \"" << v.GetLabel() << "\"\n";
		os << "\t\tgraphics [ x " << v.GetX() << " y " << v.GetY() << " ]\n";
		os << "\t]\n";
	}

	/// write edges: edge [ source target label graphics [ type line ] ]
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		os << "\tedge [ source " << e.First().GetID() << " target " << e.Second().GetID()
		   << " label \"" << e.GetLabel() << "\"\n";
		os << "\t\tgraphics [ type line ]\n";
		os << "\t]\n";
	}

	/// write arcs: edge [ source target label graphics [ type arc ] ]
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		os << "\tedge [ source " << a.From().GetID() << " target " << a.To().GetID()
		   << " label \"" << a.GetLabel() << "\"\n";
		os << "\t\tgraphics [ type arc ]\n";
		os << "\t]\n";
	}

	os << "]\n";
}