#define _CRT_SECURE_NO_WARNINGS
#include "export.h"
#include <fstream>
#include "../graph-theory-dll/json.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
	const char* pointer = load_graph();
	string graph_str = get_str(pointer);
	free_str(pointer);
	ofstream fout("simple.json");
	json obj(json::parse(graph_str));
	pointer = Minimum_Spanning_Tree(obj["result"].dump().c_str());
	json res(json::parse(pointer));
	free_str(pointer);
	fout << res;
	return 0;
}