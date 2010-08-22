
void ExportToTGF(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The TGF fileformat doesn\'t support hypergraphs\n";
	if(G.Mixed())
		throw "The TGF fileformat doesn\'t support mixed graphs\n";

	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
		os << v.GetID() << " " << v.GetLabel() << "\n";

	os << "#\n";

	/// since mixed graphs are excluded, only one loop generates output
	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
		os << e.First().GetID() << " " << e.Second().GetID() << " " << e.GetLabel() << "\n";

	/// or arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++)
		os << a.From().GetID() << " " << a.To().GetID() << " " << a.GetLabel() << "\n";
}
