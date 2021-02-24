#include <iostream>
#include <vector>
#include "DrawATree.h"
using namespace std;

struct treeNode {
	int data;
	enum c { RED, BLACK };
	c color;
	treeNode* left, * right, * parent;
};

//根
treeNode* pRoot;
//叶子节点，全局共用

treeNode leaf;
treeNode* leafNode = &leaf;

//LL型，右旋单旋
//和AVL差不多，主要是多了parent之间的更新
//画图更容易判断
void LL(treeNode* curr) {
	treeNode* leftNode = curr->left;

	curr->left = leftNode->right;
	if(leftNode->right != leafNode) leftNode->right->parent = curr;

	leftNode->parent = curr->parent;
	//注意如果对根节点进行旋转的话要更新根节点
	if (curr == pRoot) {
		pRoot = leftNode;
		pRoot->parent = nullptr;
	}
	else if (curr->parent->right == curr) curr->parent->right = leftNode;
	else curr->parent->left = leftNode;

	curr->parent = leftNode;
	leftNode->right = curr;
}

//RR型，左旋单旋
void RR(treeNode* curr) {
	treeNode* rightNode = curr->right;

	curr->right = rightNode->left;
	if (rightNode->left != leafNode) rightNode->left->parent = curr;

	rightNode->parent = curr->parent;
	if (curr == pRoot) {
		pRoot = rightNode;
		pRoot->parent = nullptr;
	}
	else if (curr->parent->left == curr) curr->parent->left = rightNode;
	else curr->parent->right = rightNode;

	curr->parent = rightNode;
	rightNode->left = curr;
}

//插入后重平衡
//判断是否需要重平衡有三种条件
//重平衡分三种
//1. 红黑树为空树
//2. 插入节点的 key 已经存在
//3. 插入节点的父节点是黑色节点
//4. 插入节点的父节点是红色节点
//4.1 存在叔叔节点且叔叔节点是红色节点
//4.2 叔叔节点不存在或叔叔节点是黑色节点，且插入节点的父节点是祖父节点的左子节点
//4.2.1 插入节点是父节点的左子节点
//4.2.2 插入节点是父节点的右子节点
//4.3 叔叔节点不存在或叔叔节点是黑色节点，且插入节点的父节点是祖父节点的右子节点
//4.3.1 插入节点是父节点的右子节点
//4.3.2 插入节点是父节点的左子节点
//要保持子孙路径的黑色节点个数一样和红色节点的子节点不能为红色节点
//画图更容易判断
void insertRebalance(treeNode* curr) {
	//迭代，①curr为根节点或者②curr的父节点为黑色无需重平衡
	//需要重平衡只有③curr的父节点为红色的情况
	while (curr != pRoot && curr->parent->color == treeNode::RED) {
		//找uncle结点，需要分两种
		if (curr->parent->parent->left == curr->parent) {
			treeNode* uncle = curr->parent->parent->right;
			//uncle为黑色，curr为左节点
			if (uncle->color == treeNode::BLACK
				&& curr->parent->left == curr) {
				curr->parent->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				LL(curr->parent->parent);
			}
			//uncle为黑色，curr为右节点
			//完成后变为curr为左节点的情况，迭代进行
			else if (uncle->color == treeNode::BLACK
				&& curr->parent->right == curr) {
				curr = curr->parent;
				RR(curr);
			}
			//uncle为红色
			else if (uncle->color == treeNode::RED) {
				curr->parent->color = treeNode::BLACK;
				uncle->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				curr = curr->parent->parent;
			}
		}
		//和上面以此类推
		else if (curr->parent->parent->right == curr->parent) {
			treeNode* uncle = curr->parent->parent->left;
			if (uncle->color == treeNode::BLACK
				&& curr->parent->left == curr) {
				curr = curr->parent;
				LL(curr);
			}
			else if (uncle->color == treeNode::BLACK 
				&& curr->parent->right == curr) {
				curr->parent->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				RR(curr->parent->parent);
			}
			else if (uncle->color == treeNode::RED) {
				curr->parent->color = treeNode::BLACK;
				uncle->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				curr = curr->parent->parent;
			}
		}	
	}
	//将根节点标为黑色，有两种条件会迭代回到根节点而结束
	pRoot->color = treeNode::BLACK;
}

//插入结点
void insert(int num) {
	treeNode* prev = pRoot, *curr = pRoot;
	//如果找到了叶子结点的位置，分配空间
	while (curr != leafNode) {
		prev = curr;
		//如果数字比当前节点的值小，即进入当前节点的左子树继续判断
		if (num < curr->data) curr = curr->left;
		//如果数字比当前节点的值大，即进入当前节点的右子树继续判断
		else if (num > curr->data) curr = curr->right;
		else if (curr->data == num) return;
	}
	//建一个新结点，红色
	treeNode* newNode = new treeNode();
	newNode->color = treeNode::RED;
	newNode->parent = prev;
	newNode->left = newNode->right = leafNode;
	newNode->data = num;

	//如果树还没建立，则新结点为根节点
	//insertRebalance会把根节点标为黑色，parent要指回自己
	if (curr == pRoot) {
		pRoot = newNode;
		pRoot->parent = nullptr;
		insertRebalance(pRoot);
	}
	//因为不是传引用，所以要借用prev才能改变树
	else if (num < prev->data) {
		prev->left = newNode;
		insertRebalance(prev->left);
	}
	else if (num > prev->data) {
		prev->right = newNode;
		insertRebalance(prev->right);
	}
}

//1. 节点是红色
//2. 节点是黑色
//3. 节点为根节点
//删除后重平衡
void deleRebalance(treeNode* curr) {
	//1、3跳出循环
	//2节点是黑色进入循环
	while (curr->color == treeNode::BLACK && curr != pRoot) {
		//2.1 节点是父节点的左子节点
		if (curr->parent->left == curr) {
			treeNode* brother = curr->parent->right;
			//2.1.2 节点的兄弟节点是黑色
			//2.1.2.1 节点的兄弟节点有右子节点
			if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->right->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
			//2.1.2.2 节点的兄弟节点有左子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right == leafNode) {
				brother->left->color = treeNode::BLACK;
				brother->color = treeNode::RED;
				LL(brother);
				//循环回到2.1.2.1
			}
			//2.1.2.3 节点的兄弟节点有左右子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->right->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
			//2.1.2.4 节点的兄弟节点为叶子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right == leafNode) {
				brother->color = treeNode::RED;
				curr = curr->parent;
			}
			//2.1.3 节点的兄弟节点是红色
			else if (brother->color == treeNode::RED) {
				brother->left->color = treeNode::RED;
				brother->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
		}
		//2.2 节点是父节点的右子节点
		else if (curr->parent->right == curr) {
			treeNode* brother = curr->parent->left;
			//2.1.2.1 节点的兄弟节点有左子节点
			if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right == leafNode) {
				brother->color = curr->parent->color;
				brother->left->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
			//2.1.2.1 节点的兄弟节点有右子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right != leafNode) {
				brother->right->color = treeNode::BLACK;
				brother->color = treeNode::RED;
				RR(brother);
				//循环回到2.2.2.1
			}
			//2.2.2.3 节点的兄弟节点有左右子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->left->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
			//2.2.2.4 节点的兄弟节点为叶子节点
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right == leafNode) {
				brother->color = treeNode::RED;
				curr = curr->parent;
			}
			//2.2.3 节点的兄弟节点是红色
			else if (brother->color == treeNode::RED) {
				brother->right->color = treeNode::RED;
				brother->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
		}
	}
	//循环里2.1.2.4和2.2.2.4需要找到红节点标为黑色
	curr->color = treeNode::BLACK;
}

//删除节点
void dele(int num) {
	treeNode* curr = pRoot;
	while (curr != leafNode) {
		//如果数字比当前节点的值小，即进入当前节点的左子树继续判断
		if (num < curr->data) curr = curr->left;
		//如果数字比当前节点的值大，即进入当前节点的右子树继续判断
		else if (num > curr->data) curr = curr->right;
		else if (curr->data == num) {
			//当前结点为根节点
			if (curr == pRoot) {
				//根节点为叶子节点
				if (curr->left == leafNode && curr->right == leafNode) {
					pRoot = leafNode;
					delete curr;
					curr = nullptr;
				}
				//根节点有右结点
				else if (curr->left == leafNode && curr->right != leafNode) {
					pRoot = curr->right;
					curr->right->parent = nullptr;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//根节点有左节点
				else if (curr->left != leafNode && curr->right == leafNode) {
					pRoot = curr->left;
					curr->left->parent = nullptr;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//根节点有左右节点
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//找到后继结点
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//记录前驱结点的值，再往下递归找前驱结点（一定是一个叶子节点）
					//必须这样做，不可以删除直接删除前驱结点，因为回溯时要进行重平衡
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			//当前节点为左节点
			else if (curr->parent->left == curr) {
				//当前节点为叶子节点且为红色
				if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::RED) {
					curr->parent->left = leafNode;
					delete curr;
					curr = nullptr;
				}
				//当前节点为叶子节点且为黑色
				else if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::BLACK) {
					deleRebalance(curr);
					curr->parent->left = leafNode;
					delete curr;
					curr = nullptr;
				}
				//当前节点有右结点
				else if (curr->left == leafNode && curr->right != leafNode) {
					curr->parent->left = curr->right;
					curr->right->parent = curr->parent;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//当前节点有左节点
				else if (curr->left != leafNode && curr->right == leafNode) {
					curr->parent->left = curr->left;
					curr->left->parent = curr->parent;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//当前节点有左右节点
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//找到后继结点
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//记录前驱结点的值，再往下递归找前驱结点（一定是一个叶子节点）
					//必须这样做，不可以删除直接删除前驱结点，因为回溯时要进行重平衡
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			//当前节点为右节点
			else if (curr->parent->right == curr) {
				//当前节点为叶子节点且为红色
				if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::RED) {
					curr->parent->right = leafNode;
					delete curr;
					curr = nullptr;
				}
				//当前节点为叶子节点且为黑色
				else if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::BLACK) {
					deleRebalance(curr);
					curr->parent->right = leafNode;
					delete curr;
					curr = nullptr;
				}
				//当前节点有右结点
				else if (curr->left == leafNode && curr->right != leafNode) {
					curr->parent->right = curr->right;
					curr->right->parent = curr->parent;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//当前节点有左节点
				else if (curr->left != leafNode && curr->right == leafNode) {
					curr->parent->right = curr->left;
					curr->left->parent = curr->parent;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//当前节点有左右节点
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//找到后继结点
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//记录前驱结点的值，再往下递归找前驱结点（一定是一个叶子节点）
					//必须这样做，不可以删除直接删除前驱结点，因为回溯时要进行重平衡
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			break;
		}
	}
}

//建树
void createTree(vector<int> v) {
	for (int i = 0; i < static_cast<int>(v.size()); i++) {
		insert(v[i]);
		DrawTree::Draw(pRoot);
	}
}

int main() {
	vector<int> v = { 49,38,65,97,76,13,27,100 };

	//构造叶子节点，所有叶子节点都用这个
	leafNode->left = leafNode->right = leafNode->parent = nullptr;
	leafNode->color = treeNode::BLACK;
	leafNode->data = -1;

	//初始化根节点为叶子节点
	pRoot = leafNode;
	createTree(v);

	DrawTree::Draw(pRoot);

	int num;
	string action;
	while (true) {
		cout << "删除/添加/退出（d/a/z）: ";
		cin >> action;
		if (action == "d") {
			cin >> num;
			dele(num);
		}
		else if (action == "a") {
			cin >> num;
			insert(num);
		}
		else if (action == "z") break;
		DrawTree::Draw(pRoot);
	}
	return 0;
}