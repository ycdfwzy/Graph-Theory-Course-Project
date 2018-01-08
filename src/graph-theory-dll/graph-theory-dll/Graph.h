#pragma once
#include "Pairing_Heap.h"
#include "ThreadPool.h"
#include <Thread>
#include <queue>
#include <cstring>
#include <algorithm>
#include <iostream>

/*  图 类
    成员变量：点数N，边数M，邻接表节点数统计变量tot
	         邻接表数组：头指针a,节点node,下一个节点nxt,边权值weight
			 无穷大INF
	成员函数：加边addedge()
	         计算单源最短路ShortestPath()
			 计算最大生成树MST()
			 计算介数中心度Betweenness_Centrality()
			 计算紧密中心度Closeness_Centrality()
			 多源最短路Floyd()
*/

class Graph {
public:
	Graph(int, int);
	~Graph();
	void addedge(int, int, int);
	int ShortestPath(int, int, int*, int*, long long *path = nullptr);
	int MST(int*, int*);
	//int MST_test(int*, int*);
	void Betweenness_Centrality(double*);
	//void Betweenness_Centrality_test(double*);
	void Closeness_Centrality(double*);
	void Floyd(int**);
	static int INF;
private:
	int N, M, tot;
	int *a, *node, *nxt/*, *to*/;
	int *weight;
};

