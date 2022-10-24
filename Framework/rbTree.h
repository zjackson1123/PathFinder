#include "framework.h"

struct Node { 
    int val;
    Node *parent;
    Node *left;
    Node *right;
    int color;
};

typedef Node *NodePtr;

class RBTree {
    private:
        NodePtr root;
        NodePtr nullNode;

        int search(NodePtr node, int key, int delta_v) {
            if(node == nullNode){
                 return 0;
            }
            if(node->val < key){
                if(node->right == nullNode) {
                    return node->val + delta_v;
                }
                return search(node->right, key, delta_v);
            }
            if(node->val > key) {
                if(node->left == nullNode) {
                    return node->val;
                }
                return search(node->left, key, delta_v);
            }   
            return 0;
        }

        void FixTree(NodePtr current) {
            NodePtr p;
            while(current->parent->color == 1) {
                if(current->parent == current->parent->parent->right) {
                    p = current->parent->parent->left;
                    if(p->color == 1) {
                        p->color = 0;
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        current = current->parent->parent;
                    } else {
                        if(current == current->parent->left) {
                            current = current->parent;
                            rightRotate(current);        
                        }
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        leftRotate(current->parent->parent);

                    }
                } else {
                    p = current->parent->parent->right;
                    if(p->color == 1) {
                        p->color = 0;
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        current = current->parent->parent;
                    } else {
                        if(current = current->parent->right) {
                            current = current->parent;
                            leftRotate(current);
                        }
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        rightRotate(current->parent->parent);
                    }
                }
                if(current == root) break;
            }
            root->color = 0;
        }

    public:
        RBTree() {
            nullNode = new Node;
            nullNode->color = 0;
            nullNode->left = nullptr;
            nullNode->right = nullptr;
            root = nullNode;
        }

        int searchTree(int key, int delta_v) {
            return search(this->root, key, delta_v);
        }

        void leftRotate(NodePtr p) {
            NodePtr a = p->right;
            p->right = a->left;
            if(a->left != nullNode) {
                a->left->parent = p;
            }
            a->parent = p->parent;
            if(p->parent == nullptr) {
                this->root = a;
            } else if (p == p->parent->left) {
                p->parent->left = a;
            } else {
                p->parent->right = a;
            }
            a->left = p;
            p->parent = a;
        }

        void rightRotate(NodePtr p) {
            NodePtr a = p->left;
            p->left = a->right;
            if(a->right != nullNode) {
                a->right->parent = p;
            }
            a->parent = p->parent;
            if(p->parent == nullptr) {
                this->root = a;
            } else if (p == p->parent->right) {
                p->parent->right = a;
            } else {
                p->parent->left = a;
            }
            a->right = p;
            p->parent = a;
        }

        void rbInsert(int key) {
            NodePtr node = new Node;
            node->parent = nullptr;
            node->val = key;
            node->left = nullNode;
            node->right = nullNode;
            node->color = 1;

            NodePtr a = nullptr;
            NodePtr p =this->root;

            while (p != nullNode) {
                a = p;
                if(node->val < p->val) {
                    p = p->left;
                } else {
                    p = p->right;
                }
            }

            node->parent = a;
            if(a == nullptr) {
                root = node;
            } else if (node->val < a->val) {
                a->left = node;
            } else {
                a->right = node;
            }

            if(node->parent == nullptr) {
                node->color = 0;
                return;
            }

            if(node->parent->parent == nullptr) {
                return;
            }

            FixTree(node);
            
        }

};

