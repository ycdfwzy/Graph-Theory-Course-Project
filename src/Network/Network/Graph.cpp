#include "Graph.h"



Graph::Graph() {
	N = M = 0;
}


Graph::~Graph() {
}

void Graph::load_csv(int threshold) {
	CSVHandler moivecsv, usercsv;
	moivecsv.load("input/movie.csv");
	usercsv.load("input/user.csv");
	auto& uc = usercsv.getCSV();
	auto& mc = moivecsv.getCSV();


	int movie_cnt = 0, user_cnt = 0;
	for (auto& c : uc) {
		if (c.size() < 2) {
			continue;
		}
		if (movieid.find(c[0]) == movieid.end()) {
			movieid.insert(make_pair(c[0], ++movie_cnt));
		}
		if (userid.find(c[1]) == userid.end()) {
			userid.insert(make_pair(c[1], ++user_cnt));
		}
	}
	idmovie = new string[movie_cnt + 1];
	iduser = new string[user_cnt + 1];
	for (auto& c : movieid)
		idmovie[c.second] = c.first;
	for (auto& c : userid)
		iduser[c.second] = c.first;

	moviescores = new int[movie_cnt + 1];
	userscores = new tensor[user_cnt + 1];

	movieuser = new vector<int>[movie_cnt + 1];
	usermovie = new vector<int>[user_cnt + 1];

	for (int i = 1; i <= movie_cnt; ++i)
		moviescores[i] = 0, movieuser[i].clear();
	for (int i = 1; i <= user_cnt; ++i)
		userscores[i].clear(), usermovie[i].clear();

	for (auto& c : mc) {
		if (c.size() < 3) {
			cerr << "one line error!" << endl;
			continue;
		}
		if (movieid.find(c[1]) == movieid.end())
			continue;
		int t = (int)(atof(c[2].c_str()) + 0.5);
		moviescores[movieid[c[1]]] = t;
	}

	for (auto& c : uc) {
		if (c.size() < 2) {
			continue;
		}
		if (movieid.find(c[0]) == movieid.end()) {
			cout << "No movie!" << endl;
			continue;
		}
		if (userid.find(c[1]) == userid.end()) {
			cout << "No user!" << endl;
			continue;
		}
		int mi = movieid[c[0]];
		int ui = userid[c[1]];
		movieuser[mi].push_back(ui);
		usermovie[ui].push_back(mi);
		userscores[ui].x[moviescores[mi]]++;
	}

	for (int i = 1; i <= movie_cnt; ++i) {
		sort(movieuser[i].begin(), movieuser[i].end());
		vector<int> tmp(movieuser[i]);
		movieuser[i].clear();
		int n = tmp.size();
		for (int j = 0; j < n; ++j)
		if (j == n - 1 || tmp[j] != tmp[j+1])
			movieuser[i].push_back(tmp[j]);
		tmp.clear();
	}
	for (int i = 1; i <= user_cnt; ++i) {
		sort(usermovie[i].begin(), usermovie[i].end());
		vector<int> tmp(usermovie[i]);
		usermovie[i].clear();
		int n = tmp.size();
		for (int j = 0; j < n; ++j)
			if (j == n - 1 || tmp[j] != tmp[j + 1])
				usermovie[i].push_back(tmp[j]);
		tmp.clear();
		userscores[i].normalization();
	}

	N = userid.size();
	edges = new vector<pair<int, double>>[N + 1];

	int *mp = new int[N + 1];
	for (int i = 1; i <= N; ++i) {
		memset(mp, 0, sizeof(int)*(N+1));
		for (auto& m1 : usermovie[i]) {
			for (auto& c : movieuser[m1])
				if (c < i) mp[c]++;
		}
		for (int j = 1; j < i; ++j)
			if (mp[j] >= threshold) {
				M++;
				double s = userscores[i].dot(userscores[j]);
				s += mp[j];
				edges[i].push_back(make_pair(j, s));
				edges[j].push_back(make_pair(i, s));
			}
	}
	delete[] mp;
	for (int i = 1; i <= movie_cnt; ++i)
		movieuser[i].clear();
	delete[] movieuser;
	delete[] moviescores;
	for (int i = 1; i <= user_cnt; ++i)
		usermovie[i].clear();
	delete[] usermovie;
	delete[] userscores;
}

void Graph::connected_component() {
	int group = 0;
	Group = new int[N+1];
	int *q = new int[N + 1];
	memset(Group, 0, sizeof(int)*(N+1));
	for (int i = 1; i <= N; ++i)
	if (Group[i] == 0) {
		Group[i] = ++group;
		int l = 1, r = 1;
		q[1] = i;
		while (l <= r) {
			int x = q[l++];
			for (auto& c : edges[x])
			if (Group[c.first] == 0) {
				Group[c.first] = group;
				q[++r] = c.first;
			}
		}
	}
}

void Graph::print_text() {
	ofstream fout("output/graph.txt");
	if (!fout.is_open()) {
		cerr << "opening graph.txt failed!" << endl;
		return;
	}
	fout << N << " " << M << endl;
	for (int i = 1; i <= N; ++i)
		for (auto& c : edges[i])
			if (c.first > i)
				fout << i << " " << c.first << " " << c.second << endl;
	fout.close();
}

void Graph::print_json() {
	ofstream fout("output/graph.json");
	if (!fout.is_open()) {
		cerr << "opening graph.json failed!" << endl;
		return;
	}
	/*
	nodes = json::array();
	for (int i = 1; i <= N; ++i) {
		json node_json;
		node_json.clear();
		node_json["name"] = iduser[i];
		node_json["group"] = Group[i];
		nodes.push_back(std::move(node_json));
	}
	links = json::array();
	for (int i = 1; i <= N; ++i)
		for (auto& c : edges[i])
		if (c.first > i) {
			json link_json;
			link_json.clear();
			link_json["source"] = iduser[i];
			link_json["target"] = iduser[c.first];
			link_json["weight"] = c.second;
			links.push_back(std::move(link_json));
		}
	graph_json = json({
		{ "nodes", std::move(nodes) },
		{ "links", std::move(links) }
	});
	fout << graph_json << endl;*/
	fout << "{" << endl;
	fout << "\"nodes\":[" << endl;
	for (int i = 1; i <= N; ++i) {
		fout << "    {\"name\":\"" << iduser[i] << "\",\"group\":" << Group[i] << "}";
		if (i > 1) fout << "," << endl;
		else fout << endl;
	}
	fout << "]," << endl;
	fout << "\"links\":[" << endl;
	bool flag = true;
	for (int i = 1; i <= N; ++i)
		for (auto& c : edges[i])
			if (c.first > i) {
				if (!flag) fout << "," << endl;
				else flag = false;
				fout << "    {\"source\":\"" << iduser[i] << "\",\"target\":\"" << iduser[c.first] << "\",\"weight\":" << c.second << "}";
			}
	fout << endl << "]" << endl;
	fout << "}" << endl;
	fout.close();
}
