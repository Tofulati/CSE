#ifndef BST_H
#define BST_H
#include <iostream>
#include <stdexcept>

struct Node{
    int data;
    Node* left;
    Node* right;

    Node(int x){
        data = x;
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree{
    Node* root;

    Node* insert_helper(Node* root, int value){
        if (root == nullptr){
            root = new Node(value);
        }
        else{
            if (value < root->data){
                root->left = insert_helper(root->left, value);
            }
            else{
                root->right = insert_helper(root->right, value);
            }
        }

        return root;
    }
    // A function to display the tree graphically
    

    Node* search_helper(Node* root, int value){
        if (root == nullptr){
            return nullptr;
        }
        else{
            if (root->data == value){
                return root;
            }
            else if (value < root->data){
                return search_helper(root->left, value);
            }
            else{
                return search_helper(root->right, value);
            }
        }
    }

    Node* min_helper(Node* r){
        if(r == nullptr){
            return nullptr;
        }
        else if(r->left == nullptr){
            return r;
        }
        else{
            return min_helper(r->left);
        }
    }

    Node* max_helper(Node* r){
        if(r == nullptr){
            return nullptr;
        }
        else if(r->right == nullptr){
            return r;
        }
        else{
            return max_helper(r->right);
        }
    }

    Node* copy(Node* originTree){
        if(originTree == nullptr){
            return nullptr;
        }
        
        Node* newNode = new Node(originTree -> data);
        newNode->left = copy(originTree->left);
        newNode->right = copy(originTree->right);
        

        return newNode;
    }

public:
    BinarySearchTree(){
        root = nullptr;
    }

    BinarySearchTree(const BinarySearchTree& other){
        root = copy(other.root);
    }

    void insert(int x){
        root = insert_helper(root, x);
    }

    void traverse(Node* start){
        if(start != nullptr){
            traverse(start->left);
            std::cout << start->data << " ";
            traverse(start->right);
        }

    }

    Node* search(int x){
        return search_helper(root, x);
    }

    Node* min(){
        return min_helper(root);
    }

    Node* max(){
        return max_helper(root);
    }

    Node* successor(Node* start){
        if (start == nullptr){
            return nullptr;
        }
        if (start->right != nullptr){
            return min_helper(start->right);
        }
        else {
            Node* parent = findParent(start);
            if (parent == nullptr){
                return nullptr;
            }
            else{
                if (start == parent->left){
                    return parent;
                }
                else {
                    return nullptr;
                }
            }
        }

    }

    Node* findParent(Node* start){
        if (start == root){
            return nullptr;
        }
        Node* temp = root;
        Node* parent = root;

        while(temp != start){
            parent = temp;
            if (start->data < temp->data){
                temp = temp->left;
            }
            else{
                temp = temp->right;
            }
        }

        return parent;
    }

    void remove(Node* p){
        if(!p) return;
        Node* parent = findParent(p);

        if (p->left != nullptr && p->right != nullptr){
            Node* succ = min_helper(p->right);
            p->data = succ->data;
            remove(succ);
            return;
        }

        Node* child = (p->left != nullptr) ? p->left : p->right;
        
        if(parent == nullptr){
            root = child;
        }
        else{
            if(parent -> left == p){
                parent ->left = child;
            }
            else{
                parent -> right = child;
            }
        }
        delete p;
    }

    // A function to display the tree graphically
    void displayTree(Node* r, int level, int direction){
        // Don't worry about this function, just use it
        if (r != NULL){
            displayTree(r->right, level+1, 1);
            
            for (int i = 0; i < level-1; i++) {
                std::cout << "   ";
            }
            if (level > 0 ){
                if (direction == 1){
                    std::cout << " /--";
                }
                else{
                    std::cout << " \\--";
                }
            }
            std::cout << r->data;
            std::cout << std::endl;
            level++;
            
            displayTree(r->left, level, -1);
        }
    }

    // Call the function to display the tree and leave some space afterwards
    void draw(){
        displayTree(root, 0, 0);
        std::cout << std::endl << std::endl;
    }

};

#endif