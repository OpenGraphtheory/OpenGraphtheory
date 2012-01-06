/*
<Node rdf:ID="n1"/>
<Node rdf:ID="n2"/>
<Node rdf:ID="n3"/>

<Edge rdf:ID="e1">
    <source rdf:resource="#n1"/>
    <target rdf:resource="#n2"/>
</Edge>
<Edge rdf:ID="e2">
    <source rdf:resource="#n1"/>
    <target rdf:resource="#n3"/>
</Edge>

</rdf:RDF> 
*/

void ExportToRGML(Graph& G, ostream& os) {
	if(G.Hypergraph())
		throw "The XGMML fileformat does not support hypergraphs\n";

	os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	os << "<!-- www.Open-Graphtheory.org -->\n\n";

	os << "<rdf:RDF\n"
	   << "  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
	   << "  xmlns=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
	   << "  xmlns:rgml=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
	   << ">\n\n";
	   
	os << "  <Graph rdf:ID=\"g" << G.GetID() << "\" rgml:directed=\"false\">\n";

	/// declare vertices
	os << "    <nodes>\n";
	os << "      <rdf:Bag>\n";
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
		os << "        <rdf:li rdf:resource=\"#n" << v.GetID() << "\"/>\n";
	os << "      </rdf:Bag>\n";
	os << "    </nodes>\n\n";

	/// declare edges and arcs
	os << "    <edges>\n";
	os << "      <rdf:Bag>\n";
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
		os << "        <rdf:li rdf:resource=\"#e" << e.GetID() << "\"/>\n";
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++)
		os << "        <rdf:li rdf:resource=\"#e" << a.GetID() << "\"/>\n";
	os << "      </rdf:Bag>\n";
	os << "    </edges>\n";
         
	os << "  </Graph>\n\n";
	
	/// write vertices
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
		os << "  <Node rdf:ID=\"n" << v.GetID() << "\"/>\n";

	/// write edges
	for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++) {
		os << "\n  <Edge rdf:ID=\"e" << e.GetID() << "\">\n";
		os << "    <source rdf:resource=\"#n" << e.First().GetID() << "\"/>\n";
		os << "    <target rdf:resource=\"#n" << e.Second().GetID() << "\"/>\n";
		os << "  </Edge>\n";
	}

	/// write arcs
	for(Graph::ArcIterator a = G.BeginArcs(); a != G.EndArcs(); a++) {
		os << "\n  <Edge rdf:ID=\"" << a.GetID() << "\" directed=\"true\">\n";
		os << "    <source rdf:resource=\"#n" << a.From().GetID() << "\"/>\n";
		os << "    <target rdf:resource=\"#n" << a.To().GetID() << "\"/>\n";
		os << "  </Edge>\n";
	}

	os << "\n</rdf:RDF>\n";
}

