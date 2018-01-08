#pragma once
#include "GerwGraph.h"
/*!
 * @brief 根据用户指定的边阈值计算连通分量
 * @param threshold 边阈值
 */
class ConnectedComponent :public GerwGraph
{
	int threshold;
	vector<bool> vis;
	vector<vector<bool>>onPath;
	vector<int>group_cnt;
	int group_clock;
	/*!
	 * @brief 算法使用的深度优先搜索
	 * @param u 当前标号的结点
	 */
	void dfs(int u);
public:
	ConnectedComponent();
	~ConnectedComponent();
	/*!
	 * @brief 计算连通分量
	 * @param threshold 边阈值
	 * @return 用于GUI显示的json，其中有每个点所属连通分量的信息
	 */
	json solve(int threshold);
};

