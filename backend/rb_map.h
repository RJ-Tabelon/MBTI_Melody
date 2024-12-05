#include <iostream>
#include "song.h"
using namespace std;

#ifndef PROJECT_3_MBTI_MELODY_RB_MAP_H
#define PROJECT_3_MBTI_MELODY_RB_MAP_H

template <typename KeyType>
class rb_map {
    struct rbNode {
        KeyType key;
        Song* val;
        // true if Red, false if Black
        bool isRed;
        rbNode *parent, *left, *right;
        rbNode(KeyType& key, Song *val) {
            this->key = key;
            this->val = val;
            isRed = true;
            parent = nullptr;
            left = nullptr;
            right = nullptr;
        }
    };

    rbNode *root;
    int elementSize;

    void leftRotate(rbNode* &node) {
        rbNode* child = node->right;
        // Exchange Subtrees
        node->right = child->left;

        if (child->left != nullptr) {
            node->right->parent = node;
        }

        child->parent = node->parent;
        if (child->parent == nullptr) {
            root = child;
        } else if (node == node->parent->left){
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }

        child->left = node;
        node->parent = child;
    }

    void rightRotate(rbNode* &node) {
        // Is Symmetric to leftRotate
        rbNode* child = node->left;
        node->left = child->right;

        if (child->right != nullptr) {
            node->left->parent = node;
        }

        child->parent = node->parent;
        if (child->parent == nullptr) {
            root = child;
        } else if (node == node->parent->right) {
            node->parent->right = child;
        } else {
            node->parent->left = child;
        }

        child->right = node;
        node->parent = child;
    }

    void replace(rbNode* &oldNode, rbNode* &newNode) {
        // Update the Information of the Parents
        if (oldNode->parent == nullptr) {
            root = newNode;
        } else if (oldNode == oldNode->parent->left) {
            // Change the Child
            oldNode->parent->left = newNode;
        } else {
            oldNode->parent->right = newNode;
        }

        // Update the New Node
        if (newNode != nullptr) {
            newNode->parent = oldNode->parent;
        }
    }

    void cut(rbNode* node) {
        // Use Postorder Traversal to Delete Children Before Parents
        while (node != nullptr) {
            cut(node->left);
            cut(node->right);
            delete node;
            node = nullptr;
        }
    }
public:
    rb_map() {
        root = nullptr;
        elementSize = 0;
    }

    ~rb_map() {
        cut(root);
    }

    // Inspired by Insertion Slides in "4 - Balanced Trees"
    bool insert(KeyType& key, Song *val) {
        // Default Node is Red Leaf
        rbNode *node = new rbNode(key, val), *parent = nullptr, *curr = root;

        // Find Leaf Entry
        while (curr != nullptr) {
            parent = curr;
            // Detect Duplicate Keys
            if (key == curr->key) {
                delete node;
                return false;
            }
            if (key < curr->key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        node->parent = parent;
        if (parent == nullptr) {
            // Color the Root Black Always
            node->isRed = false;
            root = node;
        } else if (node->key < parent->key) {
            parent->left = node;
        } else {
            parent->right = node;
        }
        
        elementSize++;

        // Recolor and Rotate to Maintain Rules of Tree
        rbNode* grandparent;
        while (node != root && node->isRed && node->parent->isRed) {
            parent = node->parent;
            grandparent = parent->parent;
            rbNode* pibling;
            if (parent == grandparent->left) {
                // Check the Pibling, or the Other Child of the Grandparent
                pibling = grandparent->right;
                // Rotate
                if (pibling == nullptr || !pibling->isRed) {
                    if (node == parent->right) {
                        leftRotate(parent);
                        // Move Up
                        node = parent;
                        parent = node->parent;
                    }
                    rightRotate(grandparent);
                    // Swap Colors
                    bool temp = parent->isRed;
                    parent->isRed = grandparent->isRed;
                    grandparent->isRed = temp;
                    node = parent;
                }
            } else {
                // Is Symmetric w/ Other Condition
                pibling = grandparent->left;
                if (pibling == nullptr || !pibling->isRed) {
                    if (node == parent->left) {
                        rightRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    leftRotate(grandparent);
                    bool temp = parent->isRed;
                    parent->isRed = grandparent->isRed;
                    grandparent->isRed = temp;
                    node = parent;
                }
            }
            // Flip
            if (pibling != nullptr && pibling->isRed) {
                // Avoid 2 Consecutive Reds w/ Recoloring
                grandparent->isRed = true;
                pibling->isRed = parent->isRed = false;
                node = grandparent;
            }
        }
        // Color the Root Black
        root->isRed = false;
        return true;
    }

    rbNode* search(KeyType& key) {
        rbNode* curr = root;
        while (curr != nullptr) {
            if (key == curr->key) {
                return curr;
            }
            if (key < curr->key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return nullptr;
    }

    void remove(KeyType& key) {
        rbNode* node = search(key);
        if (node == nullptr)  {
            cout << "'" << key << "' was not found." << endl;
            return;
        }

        rbNode *tracker = node, *replacement;
        bool originalRed = tracker->isRed;
        // Check the Cases of One Child and Multiple Children
        if (node->left == nullptr) {
            replacement = node->right;
            replace(node, node->right);
        } else if (node->right == nullptr) {
            replacement = node->left;
            replace(node, node->left);
        } else {
            // Find the Inorder Predecessor (Min of Right Subtree)
            tracker = node->right;
            while (tracker->left != nullptr) {
                tracker = tracker->left;
            }
            originalRed = tracker->isRed;
            replacement = tracker->right;
            if (tracker->parent == node && replacement != nullptr) {
                replacement->parent = tracker;
            } else {
                replace(tracker, tracker->right);
                tracker->right = node->right;
                tracker->right->parent = tracker;
            }
            replace(node, tracker);
            tracker->left = node->left;
            tracker->left->parent = tracker;
            tracker->isRed = node->isRed;
        }

        delete node;
        elementSize--;

        if (!originalRed) {
            while (replacement != root && !replacement->isRed) {
                if (replacement == replacement->parent->left) {
                    rbNode* sibling = replacement->parent->right;
                    // Check When Sibling is Red w/ Black Children
                    if (sibling->isRed) {
                        sibling->isRed = false;
                        replacement->parent->isRed = true;
                        leftRotate(replacement->parent);
                        sibling = replacement->parent->right;
                    }
                    // Check When Sibling is Black w/ Black Children if Any
                    if ((sibling->left == nullptr || !sibling->left->isRed) &&
                        (sibling->right == nullptr || !sibling->right->isRed)) {
                        sibling->isRed = true;
                        replacement = replacement->parent;
                    } else {
                        // Check When Sibling is Black w/ One Child Red and One Black if Any
                        if (sibling->right == nullptr || !sibling->right->isRed) {
                            sibling->isRed = true;
                            if (sibling->left == nullptr) {
                                sibling->left->isRed = false;
                            }
                            rightRotate(sibling);
                            sibling = replacement->parent->right;
                        }
                        sibling->isRed = replacement->parent->isRed;
                        replacement->parent->isRed = false;
                        if (sibling->right == nullptr) {
                            sibling->right->isRed = false;
                        }
                        leftRotate(replacement->parent);
                        // End Loop
                        replacement = root;
                    }
                } else {
                    // Is Symmetric w/ Other Condition
                    rbNode* sibling = replacement->parent->left;
                    // Check When Sibling is Red w/ Black Children
                    if (sibling->isRed) {
                        sibling->isRed = false;
                        replacement->parent->isRed = true;
                        rightRotate(replacement->parent);
                        sibling = replacement->parent->left;
                    }
                    // Check When Sibling is Black w/ Black Children if Any
                    if ((sibling->right == nullptr || !sibling->right->isRed) &&
                        (sibling->left == nullptr || !sibling->left->isRed)) {
                        sibling->isRed = true;
                        replacement = replacement->parent;
                    } else
                        // Check When Sibling is Black w/ One Child Red and One Black if Any
                    if (sibling->left == nullptr || !sibling->left->isRed) {
                        sibling->isRed = true;
                        if (sibling->right == nullptr) {
                            sibling->right->isRed = false;
                        }
                        leftRotate(sibling);
                        sibling = replacement->parent->left;
                    }
                    sibling->isRed = replacement->parent->isRed;
                    replacement->parent->isRed = false;
                    if (sibling->left == nullptr) {
                        sibling->left->isRed = false;
                    }
                    rightRotate(replacement->parent);
                    // End Loop
                    replacement = root;
                }
            }
            replacement->isRed = false;
        }
    }
    
    int count(KeyType& key) {
        if (this->search(key) == nullptr) {
            return 0;
        }
        return 1;
    }

    int size() const {
        return elementSize;
    }

    bool empty() const {
        if (elementSize) {
            return false;
        }
        return true;
    }

    void printInorder(int n) {
        // Validate Input
        if (root == nullptr) {
            cout << "There is no data to traverse." << endl;
            return;
        } else if (n <= 0) {
            cout << "The amount to traverse must be positive." << endl;
            return;
        }
        int count = 0;
        inorderHelper(root, n, count);

        if (count < n) {
            cout << "There is not enough data to traverse." << endl;
        }
    }

    void inorderHelper(rbNode*& node, int n, int& count) {
        if (node == nullptr) {
            return;
        }

        // Visit Left, Node, Right
        inorderHelper(node->left, n, count);
        if (count < n) {
            count++;
            cout << count << ". ";
            node->val->displayShort();
            cout << endl;
        }
        inorderHelper(node->right, n, count);
    }

    Song* operator[](KeyType& key) {
        rbNode* node = search(key);
        return node->val;
    }

};

#endif //PROJECT_3_MBTI_MELODY_RB_MAP_H
