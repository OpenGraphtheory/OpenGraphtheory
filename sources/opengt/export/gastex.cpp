
void ExportToGASTEX(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The GasTeX fileformat doesn\'t support hypergraphs\n";

	os << "% www.Open-Graphtheory.org\n";
	os << "\\begin{picture}(100,100)(0,0)\n";
	
	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
		os << "\t\\node(n" << v.GetID() << ")(" << v.GetX() << "," << v.GetY() << "){" << v.GetLabel() << "}\n";

	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
		os << "\t\\drawedge[AHnb=0](n" << e.First().GetID() << ",n" << e.Second().GetID() << "){" << e.GetLabel() << "}\n";
	
	/// write arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++)
		os << "\t\\drawedge(n" << a.From().GetID() << ",n" << a.To().GetID() << "){" << a.GetLabel() << "}\n";
	
	os << "\\end{picture}\n";
}
