#include "GerwGraph.h"
#include "CSVHandler.h"
#include <iostream>


GerwGraph::GerwGraph()
{
	movie_cnt = 0;
}


GerwGraph::~GerwGraph()
{
}

int GerwGraph::get_movie_id(const string& movieName) {
	if (!movieID.count(movieName)) {
		return -1;
	}
	return movieID[movieName];
}

void GerwGraph::set_movie_id(const string& movieName) {
	if (movieID.count(movieName)) {
		return;
	}
	idMovie.push_back(movieName);
	movieID[movieName] = movie_cnt++;
}

void GerwGraph::load_csv() {
	CSVHandler user, movie;
	user.load("user.csv");
	movie.load("movie.csv");
	movie_cnt = 0;
	movieID.clear();
	idMovie.clear();

	{
		auto& csv = movie.getCSV();
		for (int row = 0; row < csv.size(); row++) {
			set_movie_id(csv[row][1]);
		}
	}

	node_attr.clear();
	node_attr.resize(movie_cnt);

	G.clear();
	G.resize(movie_cnt);
	edge_attr.clear();
	edge_attr.resize(movie_cnt);
	for (int i = 0; i < movie_cnt; i++) {
		G[i].resize(movie_cnt);
		edge_attr[i].resize(movie_cnt);
	}

	{
		auto& csv = user.getCSV();
		watch_list.clear();
		for (int row = 0; row < csv.size(); row++) {
			if (csv[row].size() < 2) {
				cerr << "not enough elements at row " << row + 1 << endl;
				continue;
			}
			const string& mov = csv[row][0];
			const string& usr = csv[row][1];
			int id = get_movie_id(mov);
			if (id == -1) {
				cerr << mov << " does not show in movie.csv" << endl;
			}
			else {
				watch_list[usr].push_back(id);
			}
		}
	}

	for (auto& p : watch_list) {
		auto& lst = p.second;
		for (auto& a : lst) {
			for (auto& b : lst) {
				if (a != b) {
					G[a][b]++;
				}
			}
		}
	}
}

void GerwGraph::from_simple_json(const json& graph_json) {
	const auto& nodes = graph_json["nodes"];
	const auto& links = graph_json["links"];
	movie_cnt = static_cast<int>(nodes.size());

	node_attr.clear();
	node_attr.resize(movie_cnt);

	movieID.clear();
	idMovie.clear();
	idMovie.resize(movie_cnt);
	for (auto& node : nodes) {
		const string& name = node["id"];
		int id = node["x"];
		movieID[name] = id;
		idMovie[id] = name;
	}
	G.clear();
	G.resize(movie_cnt);
	edge_attr.clear();
	edge_attr.resize(movie_cnt);
	for (int i = 0; i < movie_cnt; i++) {
		G[i].resize(movie_cnt);
		edge_attr[i].resize(movie_cnt);
	}
	for (auto& link : links) {
		int s = link["s"], t = link["t"], w = link["w"];
		G[s][t] = G[t][s] = w;
	}
}

int GerwGraph::Minimum_Spanning_Tree() {
	int edge_cnt = 0;
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) ++edge_cnt;
	Graph graph(movie_cnt, edge_cnt);
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) {
				graph.addedge(i, j, G[i][j]);
				graph.addedge(j, i, G[i][j]);
			}

	int *d = new int[movie_cnt+1];
	int *e = new int[movie_cnt+1];
	int ret = graph.MST(d, e);

	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (!(e[i] == j || e[j] == i))
				G[i][j] = G[j][i] = 0;
	delete[] d;
	delete[] e;
	return ret;
}

int GerwGraph::Single_Source_Shortest_Path(int S, int T, std::string& msg) {
	int edge_cnt = 0;
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) ++edge_cnt;
	Graph graph(movie_cnt, edge_cnt);
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) {
				graph.addedge(i, j, G[i][j]);
				graph.addedge(j, i, G[i][j]);
			}

	int *d = new int[movie_cnt + 1];
	int *pre = new int[movie_cnt + 1];
	int ret = graph.ShortestPath(S, T, d, pre);
	while (!msg.empty()) msg.pop_back();

	for (auto& node : node_attr) {
		node["status"] = 0;
	}
	/*for (int i = 0; i < movie_cnt; ++i)
		for (int j = 0; j < movie_cnt; ++j)
			edge_attr[i][j]["path"] = false;*/
	//cout << d[T] << endl;
	if (d[T] == Graph::INF) {
		msg.insert(0, u8"不存在最短路");
		ret = Graph::INF;
	}
	else
	{
		int x = T;
		while (x != S) {
			node_attr[x]["status"] = 1;
			edge_attr[pre[x]][x]["path"] = true;
			edge_attr[x][pre[x]]["path"] = true;
			msg.insert(0, " -> " + idMovie[x]);
			//std::cout << pre[x] << " " << x << endl;
			x = pre[x];
		}
		msg.insert(0, u8"最短路径是 " + idMovie[S]);
	}
	node_attr[S]["status"] = 2;
	node_attr[T]["status"] = 2;

	delete[] d;
	delete[] pre;
	return ret;
}

void GerwGraph::Centrality() {
	int edge_cnt = 0;
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) ++edge_cnt;
	Graph graph(movie_cnt, edge_cnt);
	for (int i = 0; i < movie_cnt; ++i)
		for (int j = i + 1; j < movie_cnt; ++j)
			if (G[i][j] > 0) {
				graph.addedge(i, j, G[i][j]);
				graph.addedge(j, i, G[i][j]);
			}

	double *Cc = new double[movie_cnt + 1];
	double *Cb = new double[movie_cnt + 1];
	graph.Betweenness_Centrality(Cb);
	graph.Closeness_Centrality(Cc);

	for (int i = 0; i < movie_cnt; ++i) {
		node_attr[i]["closeness"] = Cc[i];
		node_attr[i]["betweenness"] = Cb[i];
	}

	delete[] Cc;
	delete[] Cb;
}

json GerwGraph::to_display_json() {
	json nodes=json::array();
	for (auto& p : movieID) {
		json node_json = node_attr[p.second];
		node_json["id"] = p.first;
		nodes.push_back(std::move(node_json));
	}
	vector<int> deg(movie_cnt);
	const int max_edgecnt = 5;///<@brief 每个点的最大度数
	typedef tuple<int, int, int> edge_type;
	vector<edge_type>edges;
	for (int i = 0; i < movie_cnt; i++) {
		for (int j = i + 1; j < movie_cnt; j++) {
			if (G[i][j] > 0) {
				edges.push_back(make_tuple(i, j, G[i][j]));
			}
		}
	}
	vector<int> fa(movie_cnt);
	for (int i = 0; i < movie_cnt; i++) {
		fa[i] = i;
	}
	sort(edges.begin(), edges.end(), [](const edge_type& a, const edge_type& b) {
		return std::get<2>(a) > std::get<2>(b);
	});
	function<int(int)> find = [&fa, &find](int x) {
		return x == fa[x] ? x : fa[x] = find(fa[x]);
	};
	json links=json::array();
	for (auto& edge : edges) {///<@brief 最大生成树
		int u = std::get<0>(edge), v = std::get<1>(edge), w = std::get<2>(edge);
		bool ok = false;
		if (edge_attr[u][v].count("path") > 0 && edge_attr[u][v].at("path").get<bool>()) {
			ok = true;
		}
		else if (find(u) != find(v)) {
			fa[find(u)] = find(v);
			ok = true;
		}
		else if (deg[u] < max_edgecnt && deg[v] < max_edgecnt) {
			ok = true;
		}
		if (ok) {
			json link_json = edge_attr[u][v];///<@note 如果是无向边的话，一定要edge_attr[u][v]和edge_attr[v][u]都赋上正确的属性
			link_json["source"] = idMovie[u];
			link_json["target"] = idMovie[v];
			link_json["value"] = w;
			links.push_back(std::move(link_json));
			deg[u]++;
			deg[v]++;
		}
	}
	return json{
		{"nodes",std::move(nodes)},
		{"links",std::move(links)}
	};
}

json GerwGraph::to_simple_json() {
	json nodes, links;
	for (auto& p : movieID) {
		json node;
		node["id"] = p.first;
		node["x"] = p.second;
		nodes.push_back(std::move(node));
	}
	for (int i = 0; i < movie_cnt; i++) {
		for (int j = i + 1; j < movie_cnt; j++) {
			if (G[i][j] > 0) {
				json link;
				link["s"] = i;
				link["t"] = j;
				link["w"] = G[i][j];
				links.push_back(std::move(link));
			}
		}
	}
	json res{
		{"nodes",std::move(nodes)},
		{"links",std::move(links)}
	};
	return res;
}
