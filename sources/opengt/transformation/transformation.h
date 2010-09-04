
void scale(Graph& G, float width, float height) {
	float width=100, height=100;
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

	float scaleX = width / (maxx-minx);
	float scaleY = height / (maxy-miny);
	float scale = scaleX < scaleY ? scaleX : scaleY;
	for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++) {
		v.SetX( scale * (v.GetX() - minx));
		v.SetY( scale * (v.GetY() - miny));
	}
}
