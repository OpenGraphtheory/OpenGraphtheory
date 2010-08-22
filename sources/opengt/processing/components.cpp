

	set<set<Graph::VertexIterator> > Components(Graph G) {
		if(!G.Undirected())
			throw "Components are only defined for undirected graphs. For mixed- and directed graphs, use StrongComponents!";

		for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
			v.SetTag(-1);

		set<set<Graph::VertexIterator> > result;

		int currentcomponent = 0;
		for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
			if(v.GetTag() != -1)
				continue;

			for(VertexIterator w = v.BeginNeighborhood(); w != v.EndNeighborhood; w++) {
				
			}
		}


		return result;
	}

