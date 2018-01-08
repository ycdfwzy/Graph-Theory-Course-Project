#pragma once
#include <vector>
#include <map>
#include <string>
#include "json.hpp"
#include "Graph.h"
using namespace std;
using json = nlohmann::json;

/*!
 * @brief 用于建立图，电影为结点，用户为边，共同看过这两部电影的用户数为边权
 */
class GerwGraph
{
public:
	vector<vector<int>> G;
	vector<vector<json>>edge_attr;///<@brief 加入到json的边属性
	vector<json>node_attr;///<@brief 加入到json的点属性
	map<string, vector<int>> watch_list;
	map<string, int> movieID;///<@brief 电影到标号的映射
	vector<string>idMovie;
	int movie_cnt;

	GerwGraph();
	virtual ~GerwGraph();

	int get_movie_id(const string& movieName);

	void set_movie_id(const string& movieName);

	void load_csv();

	void from_simple_json(const json& graph_json);

	int Minimum_Spanning_Tree();

	int Single_Source_Shortest_Path(int, int, std::string&);

	void Centrality();

	json to_display_json();

	json to_simple_json();
};

