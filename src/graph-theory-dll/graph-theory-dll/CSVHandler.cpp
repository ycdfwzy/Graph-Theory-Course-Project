//
// Created by Gerwa on 2017/11/6.
//

#include "CSVHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>

string stripQuote(const string &str, int start, int end) {
	while (start < end && iswspace(str[start]) || str[start] == '"') {
		start++;
	}
	while (start < end && iswspace(str[end - 1]) || str[end - 1] == '"') {
		end--;
	}
	return str.substr(start, end - start);
}

void CSVHandler::load(const string &fileName) {
	csv.clear();
	ifstream fin(fileName);
	if (!fin) {
		cerr << "cannot open input file " << fileName << endl;
		return;
	}
	stringstream ss;
	ss << fin.rdbuf();
	string input(ss.str());
	int index = 0;
	int row = 0;
	while (index < input.size()) {
		int end_index = index;
		while (end_index < input.size() && input[end_index] != ',' && input[end_index] != '\n') {
			end_index++;
		}
		string element = stripQuote(input, index, end_index);

		if (row == csv.size()) {
			csv.resize(csv.size() + 1);
		}
		csv[row].push_back(std::move(element));

		if (end_index < input.size()) {

			if (input[end_index] == ',') {//comma
				//pass
			}
			else if (input[end_index] == '\n') {//newline
				row++;
			}
			else {
				cerr << "unexecpted seperate value at " << fileName << " at line " << row + 1 << endl;
			}
		}

		index = end_index + 1;
	}
}

void CSVHandler::save(const string& fileName) {
	string output;
	for (int row = 0; row < csv.size(); row++) {
		for (int column = 0; column < csv[row].size(); column++) {
			output.push_back('"');
			output += csv[row][column];
			output.push_back('"');
			if (column + 1 < csv[row].size()) {
				output.push_back(',');
			}
		}
		output.push_back('\n');
	}
	ofstream fout(fileName);
	if (!fout) {
		cerr << "cannot open output file " << fileName << endl;
	}
	fout << output;
}

vector<vector<string>> &CSVHandler::getCSV() {
	return csv;
}
