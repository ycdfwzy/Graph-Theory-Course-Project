#include "Graph.h"

int Graph::INF = (1 << 30) - 1;

Graph::Graph(int N, int M) {
	this->N = N;
	this->M = M;
	tot = 0;
	a = new int[N + 10];
	//to = new int[M + M + 10];
	nxt = new int[M + M + 10];
	node = new int[M + M + 10];
	weight = new int[M + M + 10];
	for (int i = 0; i < N; ++i)
		a[i] = 0;
}

Graph::~Graph() {
	delete[] a;
	//delete[] to;
	delete[] nxt;
	delete[] node;
	delete[] weight;
}

void Graph::addedge(int x, int y, int w) {
	node[++tot] = y;
	nxt[tot] = a[x];
	a[x] = tot;
	weight[tot] = w;
}

int Graph::ShortestPath(int S, int T, int *d, int *pre, long long *path) {
	for (int i = 0; i < N; ++i)
		d[i] = INF;
	if (path != nullptr) {
		for (int i = 0; i < N; ++i)
			path[i] = 0;
		path[S] = 1;
	}

	d[S] = 0;
	Pairing_Heap heap;
	Pairing_Heap_Node **t = new Pairing_Heap_Node*[N+1];
	t[S] = heap.Insert(d[S], S);
	//Dijkstra
	while (!heap.empty()){
		std::pair<int, int> tmp = heap.Pop(); //堆优化
		int minx = tmp.first;
		int x = tmp.second;
		//std::cout << x << std::endl;
		for (int i = a[x]; i; i = nxt[i]) {
			if (path != nullptr && d[node[i]] == minx + weight[i]) {
				//计算路径数
				path[node[i]] += path[x];
			} else
			if (d[node[i]] > minx + weight[i]) {
				if (d[node[i]] == INF) {
					t[node[i]] = heap.Insert(minx + weight[i], node[i]); //堆优化
				}
				else
					heap.Decrease_value(t[node[i]], d[node[i]] - minx - weight[i]); //堆优化
				d[node[i]] = minx + weight[i];
				pre[node[i]] = x;
				//计算路径数
				if (path != nullptr)
					path[node[i]] = path[x];
			}
		}
	}

	delete[] t;
	return d[T];
}

int Graph::MST(int *d, int *e) {
	bool *v = new bool[N+1];
	int ret = 0;
	for (int i = 0; i < N; ++i)
		d[i] = INF, e[i] = -1, v[i] = true;
	Pairing_Heap heap;
	Pairing_Heap_Node **t = new Pairing_Heap_Node*[N + 1];
	//Prim
	for (int k = 0; k < N; ++k) {
		if (e[k] != -1) continue;
		v[k] = false;
		for (int i = a[k]; i; i = nxt[i])
		if (v[node[i]] && d[node[i]] > -weight[i]){
			int y = node[i];
			if (e[y] == -1)
				t[y] = heap.Insert(-weight[i], y); //堆优化
			else
				heap.Decrease_value(t[y], d[y]+weight[i]); //堆优化
			e[y] = k;
			d[y] = -weight[i];
		}
		while (!heap.empty()) {
			std::pair<int, int> tmp = heap.Pop(); //堆优化
			int minx = tmp.first;
			ret += minx;
			int x = tmp.second;
			v[x] = false;
			for (int i = a[x]; i; i = nxt[i])
			if (v[node[i]] && d[node[i]] > -weight[i]) {
				if (e[node[i]] == -1) {
					t[node[i]] = heap.Insert(-weight[i], node[i]); //堆优化
				}
				else
				{
					heap.Decrease_value(t[node[i]], d[node[i]]+weight[i]); //堆优化
				}
				d[node[i]] = -weight[i];
				e[node[i]] = x;
			}
		}

	}
	delete[] v;
	return -ret;
}
/*
int Graph::MST_test(int *d, int *e) {
	int ret = 0;
	bool *v = new bool[N+1];
	for (int i = 0; i < N; ++i)
		d[i] = INF, e[i] = -1, v[i] = true;

	for (int k = 0; k < N; ++k) {
		if (e[k] != -1) continue;
		v[k] = false;
		int cnt = 0;
		for (int i = a[k]; i; i = nxt[i])
		if (v[node[i]] && d[node[i]] > -weight[i]){
			int y = node[i];
			if (e[y] == -1) ++cnt;
			e[y] = k;
			d[y] = -weight[i];
		}
		while (cnt > 0) {
			--cnt;
			int minx = INF;
			int vj;
			for (int i = 0; i < N; ++i)
			if (v[i] && minx > d[i]) {
				minx = d[i];
				vj = i;
			}
			ret += minx;
			v[vj] = false;
			for (int i = a[vj]; i; i = nxt[i])
				if (v[node[i]] && d[node[i]] > -weight[i]) {
					if (e[node[i]] == -1) {
						++cnt;
					}
					d[node[i]] = -weight[i];
					e[node[i]] = vj;
				}
		}

	}

	delete[] v;
	return -ret;
}
*/
void Graph::Betweenness_Centrality(double *Cb) {
	for (int i = 0; i < N; ++i)
		Cb[i] = 0;

	int *d = new int[N + 1];
	int *pre = new int[N + 1];
	long long *path = new long long[N + 1];
	int *q = new int[N + 1];
	long double *delta = new long double[N + 1];
	int *de = new int[N + 1];

	for (int i = 0; i < N; ++i) {
		ShortestPath(i, 0, d, pre, path);
		for (int i = 0; i < N; ++i)
			de[i] = 0;
		for (int i = 0; i < N; ++i)
			for (int j = a[i]; j; j = nxt[j])
				if (d[node[j]] == d[i] + weight[j])
					de[node[j]]++;

		int l = 1, r = 1;
		q[1] = i;
		while (l <= r) {
			int x = q[l++];
			for (int j = a[x]; j; j = nxt[j])
				if (d[node[j]] == d[x] + weight[j]){
					if (--de[node[j]] == 0) q[++r] = node[j];
				}
		}

		while (r > 1) {
			int x = q[r--];
			delta[x] = 0;
			for (int j = a[x]; j; j = nxt[j])
				if (d[node[j]] == d[x] + weight[j])
					delta[x] += (long double)path[x] / path[node[j]] * (1. + delta[node[j]]);
			Cb[x] += delta[x];
		}
	}

	//delete
	delete[] d;
	delete[] pre;
	delete[] path;
	delete[] q;
	delete[] delta;
	delete[] de;
}
/*
void Graph::Betweenness_Centrality_test(double *Cb) {
	for (int i = 0; i < N; ++i)
		Cb[i] = 0;
	int **d = new int*[N + 1];
	int *pre = new int[N + 1];
	long long **path = new long long*[N+1];
	for (int i = 0; i < N; ++i) {
		d[i] = new int[N + 1];
		path[i] = new long long[N + 1];
	}
	for (int i = 0; i < N; ++i)
		ShortestPath(i, 0, d[i], pre, path[i]);
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j) if (i != j)
			for (int k = 0; k < N; ++k) if (i != k && j != k)
				if (d[j][i] + d[i][k] == d[j][k])
					Cb[i] += (long double)path[j][i] * path[i][k] / path[j][k];

	for (int i = 0; i < N; ++i) {
		delete[] d[i];
		delete[] path[i];
	}
	delete[] d;
	delete[] pre;
	delete[] path;
}
*/
void Graph::Closeness_Centrality(double *Cc) {
	int **d = new int*[N+1];
	for (int i = 0; i < N; ++i) {
		d[i] = new int[N + 1];
		for (int j = 0; j < N; ++j)
			d[i][j] = INF;
		d[i][i] = 0;
	}
	//邻接表转为邻接矩阵
	for (int i = 0; i < N; ++i) {
		for (int j = a[i]; j; j = nxt[j]) {
			d[i][node[j]] = std::min(d[i][node[j]], weight[j]);
			d[node[j]][i] = std::min(d[node[j]][i], weight[j]);
		}
	}
	//多元最短路
	Floyd(d);
	//计算紧密中心度
	for (int i = 0; i < N; ++i) {
		Cc[i] = 0;
		int n = 0;
		for (int j = 0; j < N; ++j)
		if (d[i][j] < INF){
			Cc[i] += d[i][j];
			++n;
		}
		if (n == 1)
			Cc[i] = 0;
		else
			Cc[i] = (n - 1) / Cc[i];
	}

	//delete
	for (int i = 0; i < N; ++i)
		delete[] d[i];
	delete[] d;
}

void Graph::Floyd(int **d) {
	unsigned t = std::thread::hardware_concurrency();
	int n = N;
	for (int k = 0; k < N; ++k) {
		//多线程
		ThreadPool threads;
		std::vector< std::future<bool> > infos;
		for (int ii = 0; ii < t; ++ii) {
			infos.push_back(threads.enqueue([d, ii, k, t, n]()->bool{
				int s1 = ii*(n / t);
				int s2 = (ii+1)*(n / t);
				if (ii == t-1) s2 = n;
				for (int i = s1; i < s2; ++i)
					for (int j = 0; j < n; ++j)
						if (d[i][j] > d[i][k] + d[k][j])
							d[i][j] = d[i][k] + d[k][j];
				return true;
			}));
		}
	}
}
