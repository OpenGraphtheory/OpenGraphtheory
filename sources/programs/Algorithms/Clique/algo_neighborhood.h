
#ifndef __ALGO_NEIGHBORHOOD_H
	#define __ALGO_NEIGHBORHOOD_H
	
	
	
	bool neighborhood(Graph *G, vector<Graph::VertexIterator>& parent_vertices, int k) {
//		for(Graph::VertexIterator it = G->BeginVertices(); it != G.EndVertices(); it++) {
	//		if()
		//}
		return false;
	}


	vector<Graph::VertexIterator> neighborhood(Graph *G, int k) {

		

		for(Graph::VertexIterator it = G->BeginVertices(); it != G->EndVertices(); it++) {
			vector<Graph::VertexIterator> temp(1, it);
			//if(neighborhood)
		}
		
		return vector<Graph::VertexIterator>(0,G->BeginVertices());
	}
	
#endif
