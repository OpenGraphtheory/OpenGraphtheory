
bool colorize(vector<vector<bool> > &adjacency, vector<int> &color, int colors, int n, int left) {
	if(left <= 0)
		return true;

	int min_choices = n+1;
	int choices;
	int colorize_vertex;
	for(int i = 0; i < n; i++) {
		if(color[i] != -1)
			continue;
		vector<vector<bool> > used_color(n,vector<bool>(colors, false));
		choices = colors;

		for(int j = 0; j < n; j++) {
			if(!adjacency[i][j])
				continue;
			if(color[j] == -1)
				continue;
			if(!used_color[i][color[j]]) {
				used_color[i][color[j]] = true;
				choices--;
			}
		}

		if(choices < min_choices) {
			min_choices = choices;
			colorize_vertex = i;
		}
	}

	for(int c = 0; c < colors; c++) {
		if(!used_colors[colorize_vertex][c]) {
			color[colorize_vertex] = c;
			if(colorize(adjacency, color, colors, n, left-1))
				return true;
		}
	}

	color[colorize_vertex] = -1;
	return false;
}

vector<int> colorize(vector<vector<bool> > &adjacency, int colors) {
	vector<int> color(colors, -1);
	colorize(adjacency, color, colors, adjacency.size(), adjacency.size());
	return(color); // returns [-1,-1,...,-1] if impossible
}
