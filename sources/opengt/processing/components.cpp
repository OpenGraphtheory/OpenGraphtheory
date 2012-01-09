

	set<set<Graph::VertexIterator>*>* Components(Graph G) {
		if(!G.Undirected())
			throw "Components are only defined for undirected graphs. For mixed- and directed graphs, use StrongComponents!";

		for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
			v.SetTag(-1);

		set<set<Graph::VertexIterator>*>* result = new set<set<Graph::VertexIterator>*>();
		int currentcomponent = 0;
		vector<Graph::VertexIterator> queue();
		
		for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
			if(v.GetTag() != -1)
				continue;

			set<Graph::VertexIterator> *CurrentComponent = new set<Graph::VertexIterator>();

			v.SetTag(currentcomponent);
			CurrentComponent->insert(v);
			queue.push(v);
			
			while(!queue.empty())
			{
				Graph::VertexIterator u = queue.front();
				
				for(VertexIterator w = u.BeginNeighborhood(); w != u.EndNeighborhood; w++) {
					if(w.GetTag() != -1)
						continue;
					
					w.SetTag(currentcomponent);
					CurrentComponent->insert(w);
					queue.push(w);
				}
				
				queue.pop();
			}
			
			result.insert(CurrentComponent);
			currentcomponent++;
		}


		return result;
	}
