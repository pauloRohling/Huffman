#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

#include <string>

using namespace std;

class HuffmanNode {
public:
    HuffmanNode(int frequency, char data, string bitData = "", HuffmanNode* left = nullptr, HuffmanNode* right = nullptr) {
        this->frequency = frequency;
        this->data = data;
        this->bitData = bitData;
        this->left = left;
        this->right = right;
    }

    bool IsLeaf() {
        return (this->left == nullptr && this->right == nullptr);
    }

public:
    int frequency;
    char data;
    string bitData;
    HuffmanNode* left;
    HuffmanNode* right;
};

#endif // HUFFMANNODE_H
