#if defined(_WIN32)
#define DllExport   __declspec(dllexport)
#else
#define DllExport
#endif
#include "json.hpp"
#include "GerwGraph.h"
#include "Utility.h"
#include <fstream>
#include <iostream>
#include "ConnectedComponent.h"
using json = nlohmann::json;
using namespace std;

extern "C" {
	DllExport const char* get_str(const char* str) {
		return str;
	}
	DllExport void free_str(const char* str) {
		delete[] str;
	}
	DllExport const char* load_graph() {
		GerwGraph g;
		g.load_csv();
		json graph_json = g.to_simple_json();
		json res{
			{"okay",true},
			{"result",std::move(graph_json)}
		};
		return heap_str(res.dump());
	}
	DllExport const char* get_display(const char* graph_str) {
		GerwGraph g;
		g.from_simple_json(json::parse(graph_str));
		json res{
			{"okay",true},
			{"result",std::move(g.to_display_json())}
		};
		return heap_str(res.dump());
	}
	DllExport const char* connected_component(const char* graph_str, int threshold) {
		ConnectedComponent cc;
		cc.from_simple_json(json::parse(graph_str));
		json res{
			{"okay",true},
			{"result",std::move(cc.solve(threshold))}
		};
		return heap_str(res.dump());
	}
	DllExport const char* Centrality(const char* graph_str) {
		GerwGraph g;
		g.from_simple_json(json::parse(graph_str));
		g.Centrality();
		json res{
			{ "okay", true },
			{ "result", std::move(g.to_display_json()) }
		};
		return heap_str(res.dump());
	}
	DllExport const char* Single_Source_Shortest_Path(const char* graph_str, const char* S, const char* T) {
		GerwGraph g;
		g.from_simple_json(json::parse(graph_str));
		std::string msg;
		std::string s(S);
		std::string t(T);
		/*
		ofstream fout("a.out");
		for (auto& tmp : g.movieID) {
			fout << tmp.first << endl;
		}
		fout.close();
		*/
		int Sid = g.get_movie_id(s);
		int Tid = g.get_movie_id(t);
		//cout << s << " " << t << endl;
		//cout << Sid << " " << Tid << endl;
		int d = g.Single_Source_Shortest_Path(Sid, Tid, msg);
		std::string distance(d==Graph::INF?"infinite":std::to_string(d));
		json tmp(g.to_display_json());
		tmp["display"] = msg;
		tmp["distance"] = distance;
		json res{
			{ "okay", true },
			{ "result", std::move(tmp) }
		};
		return heap_str(res.dump());
	}
	DllExport const char* Minimum_Spanning_Tree(const char* graph_str) {
		GerwGraph g;
		g.from_simple_json(json::parse(graph_str));
		int sum = g.Minimum_Spanning_Tree();
		json tmp(g.to_display_json());
		tmp["sum"] = sum;
		json res{
			{ "okay", true },
			{ "result", std::move(tmp) }
		};
		return heap_str(res.dump());
	}
}