#include <iostream>
#include "song.h"
using namespace std;

#ifndef PROJECT_3_MBTI_MELODY_RB_MAP_H
#define PROJECT_3_MBTI_MELODY_RB_MAP_H

class rb_map {
    struct rbNode {
        string key;
        Song* val;
        // true if Red, false if Black
        bool isRed;
        rbNode *parent, *left, *right;
        rbNode(string& key, Song *val) {
            this->key = key;
            this->val = val;
            isRed = true;
            parent = nullptr;
            left = nullptr;
            right = nullptr;
        }
    };
    rbNode *root;
    int count;
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
public:
    rb_map() {
        // FIXME: In Progress
    }
    // std::map Functions
    // Inspired by Insertion Slides in "4 - Balanced Trees"
    void insert(string& key, Song *val) {
        // Default Node is Red Leaf
        rbNode *node = new rbNode(key, val), *parent = nullptr, *curr = root;

        // Find Leaf Entry
        while (curr != nullptr) {
            parent = curr;
            if (node->key < curr->key) {
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

        // Recolor and Rotate to Maintain Rules of Tree
        rbNode* grandparent = nullptr;
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
    }

    rbNode* search(string& key) {
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

    void remove(string& key) {
        rbNode* node = search(key);
        if (node == nullptr)  {
            cout << "'" << key << "' was not found." << endl;
            return;
        }

        rbNode *y = node, *x;
        bool original = y->isRed;
        if (node->left == nullptr) {
            // FIXME: In Progress
        }
    }

    // operator[]()
    // count()
    // size()
    // empty()
};

#endif //PROJECT_3_MBTI_MELODY_RB_MAP_H
