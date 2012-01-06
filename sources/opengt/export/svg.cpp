
void ExportToSVG(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The SVG fileformat does not support hypergraphs\n";

	Graph::VertexIterator v1 = G.BeginVertices();
	int maxx = v1.GetX(), minx = v1.GetX(), maxy = v1.GetY(), miny = v1.GetY();
	for(v1++; v1 != G.EndVertices(); v1++) {
		if(v1.GetX() < minx)
			minx = v1.GetX();
		if(v1.GetX() > maxx)
			maxx = v1.GetX();
		if(v1.GetY() < miny)
			miny = v1.GetY();
		if(v1.GetY() > maxy)
			maxy = v1.GetY();
	}

	/// header
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	os << "<svg xmlns=\"http://www.w3.org/2000/svg\"\n";
	os << "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
	os << "     xmlns:ev=\"http://www.w3.org/2001/xml-events\"\n";
	os << "     version=\"1.1\" baseProfile=\"full\"\n";
	//os << "     width=\"" << << "mm\" height=\"" << << "mm\"\n";
	os << "     viewBox=\"" << minx << " " << miny << " " << maxx << " " << maxy<< "\">\n\n";

	os << "<!-- www.Open-Graphtheory.org -->\n\n";

	/// draw vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		os << "  <circle cx=\"" << v.GetX() << "\" cy=\"" << v.GetY() << "\" r=\"" << 2*v.GetWeight() << "\"/>\n";
		os << "  <text />\n";
	}

	/// draw edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
		os << "  <line x1=\"" << e.First().GetX() << "\" y1=\"" << e.First().GetY()
		   << "\" x2=\"" << e.Second().GetX() << "\" y2=\"" << e.Second().GetY()
		   << "\" stroke-width=\"" << e.GetWeight() << "px\"/>\n";

	/// draw arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++)
		os << "  <line x1=\"" << a.From().GetX() << "\" y1=\"" << a.From().GetY()
		   << "\" x2=\"" << a.To().GetX() << "\" y2=\"" << a.To().GetY()
		   << "\" stroke-width=\"" << a.GetWeight() << "px\"/>\n";

	/// footer
	os << "</svg>\n";
}
