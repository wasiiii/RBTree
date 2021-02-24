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

//��
treeNode* pRoot;
//Ҷ�ӽڵ㣬ȫ�ֹ���

treeNode leaf;
treeNode* leafNode = &leaf;

//LL�ͣ���������
//��AVL��࣬��Ҫ�Ƕ���parent֮��ĸ���
//��ͼ�������ж�
void LL(treeNode* curr) {
	treeNode* leftNode = curr->left;

	curr->left = leftNode->right;
	if(leftNode->right != leafNode) leftNode->right->parent = curr;

	leftNode->parent = curr->parent;
	//ע������Ը��ڵ������ת�Ļ�Ҫ���¸��ڵ�
	if (curr == pRoot) {
		pRoot = leftNode;
		pRoot->parent = nullptr;
	}
	else if (curr->parent->right == curr) curr->parent->right = leftNode;
	else curr->parent->left = leftNode;

	curr->parent = leftNode;
	leftNode->right = curr;
}

//RR�ͣ���������
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

//�������ƽ��
//�ж��Ƿ���Ҫ��ƽ������������
//��ƽ�������
//1. �����Ϊ����
//2. ����ڵ�� key �Ѿ�����
//3. ����ڵ�ĸ��ڵ��Ǻ�ɫ�ڵ�
//4. ����ڵ�ĸ��ڵ��Ǻ�ɫ�ڵ�
//4.1 ��������ڵ�������ڵ��Ǻ�ɫ�ڵ�
//4.2 ����ڵ㲻���ڻ�����ڵ��Ǻ�ɫ�ڵ㣬�Ҳ���ڵ�ĸ��ڵ����游�ڵ�����ӽڵ�
//4.2.1 ����ڵ��Ǹ��ڵ�����ӽڵ�
//4.2.2 ����ڵ��Ǹ��ڵ�����ӽڵ�
//4.3 ����ڵ㲻���ڻ�����ڵ��Ǻ�ɫ�ڵ㣬�Ҳ���ڵ�ĸ��ڵ����游�ڵ�����ӽڵ�
//4.3.1 ����ڵ��Ǹ��ڵ�����ӽڵ�
//4.3.2 ����ڵ��Ǹ��ڵ�����ӽڵ�
//Ҫ��������·���ĺ�ɫ�ڵ����һ���ͺ�ɫ�ڵ���ӽڵ㲻��Ϊ��ɫ�ڵ�
//��ͼ�������ж�
void insertRebalance(treeNode* curr) {
	//��������currΪ���ڵ���ߢ�curr�ĸ��ڵ�Ϊ��ɫ������ƽ��
	//��Ҫ��ƽ��ֻ�Т�curr�ĸ��ڵ�Ϊ��ɫ�����
	while (curr != pRoot && curr->parent->color == treeNode::RED) {
		//��uncle��㣬��Ҫ������
		if (curr->parent->parent->left == curr->parent) {
			treeNode* uncle = curr->parent->parent->right;
			//uncleΪ��ɫ��currΪ��ڵ�
			if (uncle->color == treeNode::BLACK
				&& curr->parent->left == curr) {
				curr->parent->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				LL(curr->parent->parent);
			}
			//uncleΪ��ɫ��currΪ�ҽڵ�
			//��ɺ��ΪcurrΪ��ڵ���������������
			else if (uncle->color == treeNode::BLACK
				&& curr->parent->right == curr) {
				curr = curr->parent;
				RR(curr);
			}
			//uncleΪ��ɫ
			else if (uncle->color == treeNode::RED) {
				curr->parent->color = treeNode::BLACK;
				uncle->color = treeNode::BLACK;
				curr->parent->parent->color = treeNode::RED;
				curr = curr->parent->parent;
			}
		}
		//�������Դ�����
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
	//�����ڵ��Ϊ��ɫ������������������ص����ڵ������
	pRoot->color = treeNode::BLACK;
}

//������
void insert(int num) {
	treeNode* prev = pRoot, *curr = pRoot;
	//����ҵ���Ҷ�ӽ���λ�ã�����ռ�
	while (curr != leafNode) {
		prev = curr;
		//������ֱȵ�ǰ�ڵ��ֵС�������뵱ǰ�ڵ�������������ж�
		if (num < curr->data) curr = curr->left;
		//������ֱȵ�ǰ�ڵ��ֵ�󣬼����뵱ǰ�ڵ�������������ж�
		else if (num > curr->data) curr = curr->right;
		else if (curr->data == num) return;
	}
	//��һ���½�㣬��ɫ
	treeNode* newNode = new treeNode();
	newNode->color = treeNode::RED;
	newNode->parent = prev;
	newNode->left = newNode->right = leafNode;
	newNode->data = num;

	//�������û���������½��Ϊ���ڵ�
	//insertRebalance��Ѹ��ڵ��Ϊ��ɫ��parentҪָ���Լ�
	if (curr == pRoot) {
		pRoot = newNode;
		pRoot->parent = nullptr;
		insertRebalance(pRoot);
	}
	//��Ϊ���Ǵ����ã�����Ҫ����prev���ܸı���
	else if (num < prev->data) {
		prev->left = newNode;
		insertRebalance(prev->left);
	}
	else if (num > prev->data) {
		prev->right = newNode;
		insertRebalance(prev->right);
	}
}

//1. �ڵ��Ǻ�ɫ
//2. �ڵ��Ǻ�ɫ
//3. �ڵ�Ϊ���ڵ�
//ɾ������ƽ��
void deleRebalance(treeNode* curr) {
	//1��3����ѭ��
	//2�ڵ��Ǻ�ɫ����ѭ��
	while (curr->color == treeNode::BLACK && curr != pRoot) {
		//2.1 �ڵ��Ǹ��ڵ�����ӽڵ�
		if (curr->parent->left == curr) {
			treeNode* brother = curr->parent->right;
			//2.1.2 �ڵ���ֵܽڵ��Ǻ�ɫ
			//2.1.2.1 �ڵ���ֵܽڵ������ӽڵ�
			if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->right->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
			//2.1.2.2 �ڵ���ֵܽڵ������ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right == leafNode) {
				brother->left->color = treeNode::BLACK;
				brother->color = treeNode::RED;
				LL(brother);
				//ѭ���ص�2.1.2.1
			}
			//2.1.2.3 �ڵ���ֵܽڵ��������ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->right->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
			//2.1.2.4 �ڵ���ֵܽڵ�ΪҶ�ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right == leafNode) {
				brother->color = treeNode::RED;
				curr = curr->parent;
			}
			//2.1.3 �ڵ���ֵܽڵ��Ǻ�ɫ
			else if (brother->color == treeNode::RED) {
				brother->left->color = treeNode::RED;
				brother->color = treeNode::BLACK;
				RR(curr->parent);
				break;
			}
		}
		//2.2 �ڵ��Ǹ��ڵ�����ӽڵ�
		else if (curr->parent->right == curr) {
			treeNode* brother = curr->parent->left;
			//2.1.2.1 �ڵ���ֵܽڵ������ӽڵ�
			if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right == leafNode) {
				brother->color = curr->parent->color;
				brother->left->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
			//2.1.2.1 �ڵ���ֵܽڵ������ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right != leafNode) {
				brother->right->color = treeNode::BLACK;
				brother->color = treeNode::RED;
				RR(brother);
				//ѭ���ص�2.2.2.1
			}
			//2.2.2.3 �ڵ���ֵܽڵ��������ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left != leafNode && brother->right != leafNode) {
				brother->color = curr->parent->color;
				brother->left->color = treeNode::BLACK;
				curr->parent->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
			//2.2.2.4 �ڵ���ֵܽڵ�ΪҶ�ӽڵ�
			else if (brother->color == treeNode::BLACK
				&& brother->left == leafNode && brother->right == leafNode) {
				brother->color = treeNode::RED;
				curr = curr->parent;
			}
			//2.2.3 �ڵ���ֵܽڵ��Ǻ�ɫ
			else if (brother->color == treeNode::RED) {
				brother->right->color = treeNode::RED;
				brother->color = treeNode::BLACK;
				LL(curr->parent);
				break;
			}
		}
	}
	//ѭ����2.1.2.4��2.2.2.4��Ҫ�ҵ���ڵ��Ϊ��ɫ
	curr->color = treeNode::BLACK;
}

//ɾ���ڵ�
void dele(int num) {
	treeNode* curr = pRoot;
	while (curr != leafNode) {
		//������ֱȵ�ǰ�ڵ��ֵС�������뵱ǰ�ڵ�������������ж�
		if (num < curr->data) curr = curr->left;
		//������ֱȵ�ǰ�ڵ��ֵ�󣬼����뵱ǰ�ڵ�������������ж�
		else if (num > curr->data) curr = curr->right;
		else if (curr->data == num) {
			//��ǰ���Ϊ���ڵ�
			if (curr == pRoot) {
				//���ڵ�ΪҶ�ӽڵ�
				if (curr->left == leafNode && curr->right == leafNode) {
					pRoot = leafNode;
					delete curr;
					curr = nullptr;
				}
				//���ڵ����ҽ��
				else if (curr->left == leafNode && curr->right != leafNode) {
					pRoot = curr->right;
					curr->right->parent = nullptr;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//���ڵ�����ڵ�
				else if (curr->left != leafNode && curr->right == leafNode) {
					pRoot = curr->left;
					curr->left->parent = nullptr;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//���ڵ������ҽڵ�
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//�ҵ���̽��
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//��¼ǰ������ֵ�������µݹ���ǰ����㣨һ����һ��Ҷ�ӽڵ㣩
					//������������������ɾ��ֱ��ɾ��ǰ����㣬��Ϊ����ʱҪ������ƽ��
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			//��ǰ�ڵ�Ϊ��ڵ�
			else if (curr->parent->left == curr) {
				//��ǰ�ڵ�ΪҶ�ӽڵ���Ϊ��ɫ
				if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::RED) {
					curr->parent->left = leafNode;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ�ΪҶ�ӽڵ���Ϊ��ɫ
				else if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::BLACK) {
					deleRebalance(curr);
					curr->parent->left = leafNode;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ����ҽ��
				else if (curr->left == leafNode && curr->right != leafNode) {
					curr->parent->left = curr->right;
					curr->right->parent = curr->parent;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ�����ڵ�
				else if (curr->left != leafNode && curr->right == leafNode) {
					curr->parent->left = curr->left;
					curr->left->parent = curr->parent;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ������ҽڵ�
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//�ҵ���̽��
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//��¼ǰ������ֵ�������µݹ���ǰ����㣨һ����һ��Ҷ�ӽڵ㣩
					//������������������ɾ��ֱ��ɾ��ǰ����㣬��Ϊ����ʱҪ������ƽ��
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			//��ǰ�ڵ�Ϊ�ҽڵ�
			else if (curr->parent->right == curr) {
				//��ǰ�ڵ�ΪҶ�ӽڵ���Ϊ��ɫ
				if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::RED) {
					curr->parent->right = leafNode;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ�ΪҶ�ӽڵ���Ϊ��ɫ
				else if (curr->left == leafNode && curr->right == leafNode
					&& curr->color == treeNode::BLACK) {
					deleRebalance(curr);
					curr->parent->right = leafNode;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ����ҽ��
				else if (curr->left == leafNode && curr->right != leafNode) {
					curr->parent->right = curr->right;
					curr->right->parent = curr->parent;
					curr->right->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ�����ڵ�
				else if (curr->left != leafNode && curr->right == leafNode) {
					curr->parent->right = curr->left;
					curr->left->parent = curr->parent;
					curr->left->color = treeNode::BLACK;
					delete curr;
					curr = nullptr;
				}
				//��ǰ�ڵ������ҽڵ�
				else  if (curr->left != leafNode && curr->right != leafNode) {
					//�ҵ���̽��
					auto save = curr->right;
					while (save->left != leafNode) save = save->left;
					//��¼ǰ������ֵ�������µݹ���ǰ����㣨һ����һ��Ҷ�ӽڵ㣩
					//������������������ɾ��ֱ��ɾ��ǰ����㣬��Ϊ����ʱҪ������ƽ��
					int value = save->data;
					dele(value);
					curr->data = value;
				}
			}
			break;
		}
	}
}

//����
void createTree(vector<int> v) {
	for (int i = 0; i < static_cast<int>(v.size()); i++) {
		insert(v[i]);
		DrawTree::Draw(pRoot);
	}
}

int main() {
	vector<int> v = { 49,38,65,97,76,13,27,100 };

	//����Ҷ�ӽڵ㣬����Ҷ�ӽڵ㶼�����
	leafNode->left = leafNode->right = leafNode->parent = nullptr;
	leafNode->color = treeNode::BLACK;
	leafNode->data = -1;

	//��ʼ�����ڵ�ΪҶ�ӽڵ�
	pRoot = leafNode;
	createTree(v);

	DrawTree::Draw(pRoot);

	int num;
	string action;
	while (true) {
		cout << "ɾ��/���/�˳���d/a/z��: ";
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