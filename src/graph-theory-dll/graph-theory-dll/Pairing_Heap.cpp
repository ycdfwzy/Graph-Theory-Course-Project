#include "Pairing_Heap.h"



Pairing_Heap::Pairing_Heap(){
	root = nullptr;
}

Pairing_Heap::Pairing_Heap(int val, int id) {
	root = new Pairing_Heap_Node(val, id);
}

Pairing_Heap::~Pairing_Heap(){
	while (!empty()) Pop();
}

//合并以节点a和节点b为根的两棵子树
inline Pairing_Heap_Node* Pairing_Heap_Node_Merge(Pairing_Heap_Node* a, Pairing_Heap_Node* b) {
	if (a->get_value() < b->get_value()) {
		a->link_child(b);
		return a;
	}
	else
	{
		b->link_child(a);
		return b;
	}
}
/*
Pairing_Heap_Node* Pairing_Heap_Node_List_Merge(Pairing_Heap_Node* c, Pairing_Heap_Node* End) {
	if (c->get_right() == End)
		return c;
	Pairing_Heap_Node* t = c->get_right();
	Pairing_Heap_Node* u = t->get_right();
	Pairing_Heap_Node *v = Pairing_Heap_Node_Merge(c, t);
	if (u == End) return v;
	return Pairing_Heap_Node_Merge(v, Pairing_Heap_Node_List_Merge(u, End));
}
*/
Pairing_Heap_Node** w = new Pairing_Heap_Node*[20000010];

std::pair<int,int> Pairing_Heap::Pop(){
	if (root == nullptr) {
		std::cout << "Heap is empty but pop!" << std::endl;
		return std::make_pair(0, 0);
	}
	std::pair<int, int> ret = std::make_pair(root->get_value(), root->get_id());

	Pairing_Heap_Node *I = root->get_child();
	Pairing_Heap_Node *J = I;
	//删除根
	delete root;
	//堆已空
	if (I == nullptr) {
		root = nullptr;
		return ret;
	}
	//堆仅剩一个节点
	if (I->get_left() == I) {
		root = I;
		root->set_father(nullptr);
		root->set_left(root);
		root->set_right(root);
		return ret;
	}

	//将根的儿子节点合并，并将最小节点作为新的根
	w[0] = I;
	I = I->get_right();
	int p = 0;
	while (I != J) {
		w[++p] = I;
		I = I->get_right();
	}
	for (int i = 0; i < p; i+=2) {
		w[++p] = Pairing_Heap_Node_Merge(w[i], w[i+1]);
	}
	root = w[p];
	root->set_father(nullptr);
	root->set_left(root);
	root->set_right(root);

	return ret;
}

void Pairing_Heap::Decrease_value(Pairing_Heap_Node *t, int dlt) {
	//减小元素权值
	int tmp = t->get_value() - dlt;
	t->set_value(tmp);
	if (t == root) return;
	//若违反堆的性质，则将节点连接到到根上
	if (tmp < t->get_father()->get_value()) {
		t->get_father()->cut_child(t);
		root = Pairing_Heap_Node_Merge(root, t);
		root->set_father(nullptr);
		root->set_left(root);
		root->set_right(root);
	}
}

Pairing_Heap_Node* Pairing_Heap::Insert(int val, int id) {
	Pairing_Heap *tmp = new Pairing_Heap(val, id);
	Pairing_Heap_Node* ret = tmp->root;
	Merge(tmp);	//插入元素操作本质上是堆的合并操作相同
	delete tmp;
	return ret;
}

void Pairing_Heap::Merge(Pairing_Heap* c) {
	//特判有一个堆为空的情况
	if (c->root == nullptr) return;
	if (root == nullptr) {
		root = c->root;
		c->root = nullptr;
		return;
	}
	//将当前堆调整为根节点元素更小的堆
	if (c->root->get_value() < root->get_value()) {
		std::swap(c->root, root);
	}
	//合并
	root->link_child(c->root);
	root->set_father(nullptr);
	root->set_left(root);
	root->set_right(root);
	c->root = nullptr;
}

void Pairing_Heap::clear() {
	while (!empty()) Pop();
}
