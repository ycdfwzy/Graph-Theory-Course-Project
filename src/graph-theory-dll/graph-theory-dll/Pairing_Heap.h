#pragma once
#include <algorithm>
#include "Pairing_Heap_Node.h"
#include <list>
#include <cstring>
#include <iostream>

/*  配对堆类
    成员变量：堆顶节点指针root
	成员函数：获取堆顶元素Top()
	         弹出堆顶元素Pop()
			 插入新元素Insert()
			 合并两个堆Merge()
			 减小元素权值Decrease_value()
			 清空堆clear()
			 判断堆是否为空empty()
*/

class Pairing_Heap {
public:
	Pairing_Heap();
	Pairing_Heap(int, int);
	~Pairing_Heap();
	std::pair<int, int> Top();
	std::pair<int, int> Pop();
	bool empty() const;
	void clear();
	void Decrease_value(Pairing_Heap_Node*, int);
	void Merge(Pairing_Heap*);
	Pairing_Heap_Node* Insert(int, int);
private:
	Pairing_Heap_Node* root;
	//int size;
};

inline std::pair<int, int> Pairing_Heap::Top() { return std::make_pair(root->get_value(), root->get_id()); }
inline bool Pairing_Heap::empty() const { return root == nullptr; }

