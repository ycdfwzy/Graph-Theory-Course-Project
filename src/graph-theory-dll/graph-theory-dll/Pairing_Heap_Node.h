#pragma once

/*  配对堆节点类
    成员变量：元素权值value
	         元素编号id
			 左右兄弟节点Left，Right
			 父亲节点Father
			 孩子节点Child
	成员函数：获取、设置元素权值 get_value(), set_value()
	         获取、设置元素编号 get_id(), set_id()
			 获取、设置左右孩子节点 get_left(), get_right(), set_left(), set_right()
			 获取、设置父节点 get_father(), set_father()
			 获取、设置孩子节点 get_child(), set_child()
			 切断某个孩子节点cut_child()
			 链接某个孩子节点link_child()
*/

class Pairing_Heap_Node {
public:
	Pairing_Heap_Node(int, int);
	~Pairing_Heap_Node();
	void cut_child(Pairing_Heap_Node*);
	void link_child(Pairing_Heap_Node*);
	int get_value()const;
	int get_id()const;
	Pairing_Heap_Node* get_left()const;
	Pairing_Heap_Node* get_right()const;
	Pairing_Heap_Node* get_father()const;
	Pairing_Heap_Node* get_child()const;
	void set_value(int);
	void set_id(int);
	void set_left(Pairing_Heap_Node*);
	void set_right(Pairing_Heap_Node*);
	void set_father(Pairing_Heap_Node*);
	void set_child(Pairing_Heap_Node*);
private:
	int id;
	int value;
	Pairing_Heap_Node *Left, *Right, *Father, *Child;
};

inline int Pairing_Heap_Node::get_value() const { return value; }
inline int Pairing_Heap_Node::get_id() const { return id; }
inline Pairing_Heap_Node* Pairing_Heap_Node::get_left()const { return Left; }
inline Pairing_Heap_Node* Pairing_Heap_Node::get_right()const { return Right; }
inline Pairing_Heap_Node* Pairing_Heap_Node::get_father()const { return Father; }
inline Pairing_Heap_Node* Pairing_Heap_Node::get_child()const { return Child; }
inline void Pairing_Heap_Node::set_value(int value_) { value = value_; }
inline void Pairing_Heap_Node::set_id(int id_) { id = id_; }
inline void Pairing_Heap_Node::set_left(Pairing_Heap_Node* left_) { Left = left_; }
inline void Pairing_Heap_Node::set_right(Pairing_Heap_Node* right_) { Right = right_; }
inline void Pairing_Heap_Node::set_father(Pairing_Heap_Node* father_) { Father = father_; }
inline void Pairing_Heap_Node::set_child(Pairing_Heap_Node* child_) { Child = child_; }
