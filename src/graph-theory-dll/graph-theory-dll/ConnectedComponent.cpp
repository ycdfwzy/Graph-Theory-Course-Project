#include "ConnectedComponent.h"
#include <iostream>


ConnectedComponent::ConnectedComponent()
{
}


ConnectedComponent::~ConnectedComponent()
{
}

json ConnectedComponent::solve(int threshold) {
	this->threshold = threshold;
	onPath.clear();
	onPath.resize(movie_cnt);
	for (int i = 0; i < movie_cnt; i++) {
		onPath[i].resize(movie_cnt);
	}
	group_clock = 0;
	vis.clear();
	vis.resize(movie_cnt);

	group_cnt.clear();
	for (int i = 0; i < movie_cnt; i++) {
		if (!vis[i]) {
			group_cnt.push_back(0);
			group_clock++;
			dfs(i);
		}
	}
	for (int u = 0; u < movie_cnt; u++) {
		for (int v = 0; v < movie_cnt; v++) {
			if (!onPath[u][v]) {
				G[u][v] = 0;///<@note 清除不在dfs树上的边，在gui中不显示
			}
		}
	}
	json res = to_display_json();
	res["count"] = group_clock;///<@note 用于显示的额外域，连通分量的总数
	int cnt = 0;
	for (auto& tmp : group_cnt) {
		if (tmp > 1) {
			cnt++;
		}
	}
	res["more_than_one"] = cnt;///<@note 大于1的连通分量的个数
	return res;
}

void ConnectedComponent::dfs(int u) {
	group_cnt.back()++;
	vis[u] = true;
	node_attr[u]["group"] = group_clock;
	for (int v = 0; v < movie_cnt; v++) {
		if (G[u][v] > threshold && !vis[v]) {
			onPath[u][v] = onPath[v][u] = true;
			dfs(v);
		}
	}
}
