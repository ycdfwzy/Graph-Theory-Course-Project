#include <iostream>
#include "Graph.h"
#include <fstream>
#include <ctime>


using namespace std;
Graph G;


int main() {
	//int cnt = 0;
	int threshold;
	do {
		cout << "Please choose the threshold value(more than 1):" << endl;
		cin >> threshold;
	} while (threshold <= 1);
	cout << "Building graph..." << endl;
	//clock_t start = clock();
	G.load_csv(threshold);
	//clock_t ending = clock();
	//cout << (double)(ending - start) / CLOCKS_PER_SEC << endl;
	G.connected_component();
	cout << "Writing to text files..." << endl;
	G.print_text();
	cout << "Writing to json files..." << endl;
	G.print_json();
	return 0;
}