#pragma once
#include "CSVHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "json.hpp"
using json = nlohmann::json;

/* 用户评分向量类
   用于计算用户间联系的小数部分
   x[t]表示评分在t-0.5~t+0.5的电影个数
   normalization()用于将向量归一化
   dot()用于计算两个向量的点积
*/
struct tensor{
	tensor() {
		for (int i = 0; i < 11; ++i)
			x[i] = 0;
	}
	double x[11];
	void normalization() {
		int cnt = 0;
		for (int i = 0; i < 11; ++i)
			cnt += (int)(x[i] + 1e-8);
		for (int i = 0; i < 11; ++i)
			x[i] /= cnt;
	}
	double dot(tensor& a) {
		double ret = 0;
		for (int i = 0; i < 11; ++i)
			ret += a.x[i] * x[i];
		return ret;
	}
	void clear() {
		for (int i = 0; i < 11; ++i)
			x[i] = 0;
	}
};

/*  图类
    成员变量：点数N，边数M
	         所在连通分量Group
			 邻接表edges
			 用户、电影名字到编号的映射userid,movieid
			 用户、电影编号到名字的映射iduser,idmovie
			 电影评分moviescores
			 用户评分userscores
			 某个电影的观影用户列表movieuser
			 某个用户观看过的电影列表usermovie
	成员函数：载入数据集并生成图load_csv()
	         计算连通分量connected_component()
			 输出为文本格式print_text()
			 输出为json格式print_json()
*/

class Graph {
public:
	Graph();
	~Graph();

	void load_csv(int);
	void connected_component();
	void print_text();
	void print_json();

	int N, M;
	int *Group;
	vector<int> *movieuser, *usermovie;
	int *moviescores;
	tensor *userscores;
	map<string, int> userid, movieid;
	string *iduser, *idmovie;
	vector<pair<int, double>> *edges;
};

