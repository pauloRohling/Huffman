#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <HuffmanNode.h>
#include <omp.h>
#include <iostream>

const int NTHREADS = 8;

using namespace std;

class HuffmanTree {
public:
    HuffmanTree() {
        this->root = nullptr;
    }

    void Destroy(HuffmanNode* node) {
        if(node == nullptr)
            return;
        this->Destroy(node->left);
        this->Destroy(node->right);
        delete node;
    }

    void Destroy() {
        this->Destroy(this->root);
        this->root = nullptr;
    }

    void Print(int margin = 1) {
        this->DrawTree(this->root, margin);
        cout << endl << endl;
    }

private:
    void DrawTree(HuffmanNode* node, int margin) {
        if(node == nullptr) {
            this->DrawNode('#', margin);
            return;
        }
        this->DrawTree(node->right, margin+1);
        (node->data == '\0') ? this->DrawNode('@', margin) : this->DrawNode(node->data, margin);
        this->DrawTree(node->left, margin+1);
    }

    void DrawNode(char value, int margin) {
        for(int i = 0; i < margin; i++)
            printf("\t");
        cout << value << endl;
    }

public:
    HuffmanNode* root;
};

enum {
    HUFFMAN_NORMAL_TEXT,
    HUFFMAN_ENCODED_TEXT
};

#endif // HUFFMANTREE_H
