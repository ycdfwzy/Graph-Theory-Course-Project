#pragma once
#pragma comment(lib,"gerw")
#define DllImport __declspec(dllimport)

extern "C" {
	DllImport void free_str(const char* str);
	DllImport const char* get_str(const char* str);
	DllImport const char* load_graph();
	DllImport const char* get_display(const char* graph_str); 
	DllImport const char* connected_component(const char* graph_str, int threshold);
	DllImport const char* Centrality(const char* graph_str);
	DllImport const char* Single_Source_Shortest_Path(const char* graph_str, const char* S, const char* T);
	DllImport const char* Minimum_Spanning_Tree(const char* graph_str);
}