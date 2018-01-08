#include "Pairing_Heap_Node.h"



Pairing_Heap_Node::Pairing_Heap_Node(int value_, int id_):
	value(value_), id(id_), Left(this), Right(this), Father(nullptr), Child(nullptr) {}


Pairing_Heap_Node::~Pairing_Heap_Node(){

}

void Pairing_Heap_Node::cut_child(Pairing_Heap_Node *c) {
	if (c->Father != this) return;
	//若孩子节点不是c,直接删除
	if (Child != c) {
		c->Left->Right = c->Right;
		c->Right->Left = c->Left;
		return;
	}
	//若孩子节点是c，且仅有一个孩子节点，将孩子节点置为空
	if (c->Left == c) {
		Child = nullptr;
		return;
	}
	//若孩子节点是c，且仅布置一个孩子节点，将孩子节点置为c的下一个兄弟
	Child = c->Right;
	c->Left->Right = c->Right;
	c->Right->Left = c->Left;
}

void Pairing_Heap_Node::link_child(Pairing_Heap_Node *c) {
	//若孩子节点为空，更新孩子节点
	if (Child == nullptr) {
		Child = c;
		c->Left = c->Right = c;
		c->Father = this;
		return;
	}
	//否则直接插入到孩子节点之前
	c->Left = Child->Left;
	c->Right = Child;
	Child->Left->Right = c;
	Child->Left = c;

	c->Father = this;
}
