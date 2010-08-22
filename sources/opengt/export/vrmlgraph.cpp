
void ExportToVRMLGRAPH(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The VRMLgraph fileformat doesn\'t support hypergraphs\n";
	if(G.Mixed())
		throw "The VRMLgraph fileformat doesn\'t support mixed graphs\n";
	if(G.Directed())
		throw "The VRMLgraph fileformat doesn\'t support mixed graphs\n";

	os << "# www.Open-Graphtheory.org\n";
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
		os << e.First().GetID() << " " << e.Second().GetID() << "\n";
}
