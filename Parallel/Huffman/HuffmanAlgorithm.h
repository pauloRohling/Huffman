#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <HuffmanTree.h>
#include "LinkedList.h"
#include "FileManager.h"

using namespace std;

const int MAXSYMBOLS = 256;

class HuffmanAlgorithm {
public:
    HuffmanAlgorithm() {
        this->tree = new HuffmanTree();
        this->list = new LinkedList<HuffmanNode*>();
        this->finished = false;
        this->bitTextOverflow = 0;
    }

    void Reset() {
        this->tree->Destroy();
        this->list->Destroy();
        this->finished = false;
        this->bitTextOverflow = 0;
    }

    void Encode(string textPath) {
        try {
            this->targetText = FileManager::GetTextFromFile(textPath);
        } catch (const char* e) {
            throw e;
        }

        int symbolsPerThread[MAXSYMBOLS * NTHREADS];
        for(int i = 0; i < MAXSYMBOLS * NTHREADS; i++)
            symbolsPerThread[i] = 0;

        int size = int(this->targetText.size());
        int threadSize = size / NTHREADS;
        int symbolThreadSize = MAXSYMBOLS / NTHREADS;
        int frequencies[MAXSYMBOLS];


        double timeStart = omp_get_wtime();
        #pragma omp parallel num_threads(NTHREADS)
        {
            int id = omp_get_thread_num();
            int start = id * threadSize;
//            printf("ID: %d\n", id);
            int end = (start + threadSize < size) ? start + threadSize : size;
            for(int i = start; i < end; i++) {
                symbolsPerThread[MAXSYMBOLS * id + int(this->targetText.at(unsigned(i)))]++;
            }

            #pragma omp barrier

            start = symbolThreadSize * id;
            end = (start + symbolThreadSize < MAXSYMBOLS) ? start + symbolThreadSize : MAXSYMBOLS;

            for(int i = start; i < end; i++) {
                frequencies[i] = 0;
                for(int j = 0; j < NTHREADS; j++) {
                    frequencies[i] += symbolsPerThread[MAXSYMBOLS * j + i];
                }
            }
        }

//        printf("ID: %d\n", 12312321);

        for(int i = 0; i < MAXSYMBOLS; i++) {
            if(frequencies[i] != 0 && frequencies[i] != 13) {
                HuffmanNode* node = new HuffmanNode(frequencies[i], char(i));
                this->list->AddEnd(node);
            }
        }



//        int size = int(this->targetText.size()), counter;
//        #pragma omp parallel for num_threads(NTHREADS) default(none) shared(size)
//        for(int i = 0; i < size; i++) {
//            char targetChar = this->targetText.at(unsigned(i));

//            if(int(targetChar) != 13 && !this->Contains(targetChar)) {
//                HuffmanNode* node = new HuffmanNode(0, targetChar);
//                counter = 0;
//                for(int j = 0; j < size; j++)
//                    if(targetText.at(unsigned(j)) == targetChar)
//                        counter++;
//                node->frequency = counter;
//                this->list->AddEnd(node);
//            }
//        }
        cout << "TIME: " << omp_get_wtime() - timeStart << " seconds." << endl;

        this->SortByFrequency();

        LinkedList<HuffmanNode*>* newList = this->CopyLinkedList();

        while(newList->GetSize() > 1) {
            ListNode<HuffmanNode*>* a = newList->GetElementAt(0);
            ListNode<HuffmanNode*>* b = newList->GetElementAt(1);
            HuffmanNode* node = new HuffmanNode(a->value->frequency + b->value->frequency, '\0', "", a->value, b->value);
            ListNode<HuffmanNode*>* c = new ListNode<HuffmanNode*>(node);

            int position = this->GetPositionFor(newList, c->value);
            if(position != -1)
                newList->AddAt(c->value, position);
            ListNode<HuffmanNode*>* newBegin = newList->GetElementAt(2);
            newList->RemoveBegin();
            newList->RemoveBegin();
            newList->begin = newBegin;
        }
        this->tree->root = newList->begin->value;
        newList->Destroy();
        this->SetBitData();
        this->GenerateBitText();
        this->GenerateHuffmanText();
        this->finished = true;
    }

    void Encode(string textPath, string encodedTextPath, string keyPath) {
        this->Encode(textPath);
        this->ExportTo(encodedTextPath, keyPath, HUFFMAN_NORMAL_TEXT);
    }

    void Decode(string encodedTextPath, string keyPath) {
        LinkedList<string>* tempList;
        try {
            tempList = FileManager::GetCharsFromFile(keyPath);
            this->huffmanText = FileManager:: GetTextFromFile(encodedTextPath);
        } catch (const char* e) {
            throw e;
        }
        this->list = this->CreateListToDecode(tempList);
        tempList->Destroy();
        delete tempList;


        this->tree->root = new HuffmanNode(NULL, '\0');
        HuffmanNode* nav = this->tree->root;

        int size = this->list->GetSize();
        for(int i = 0; i < size; i++) {
            HuffmanNode* node = this->list->GetValueAt(i);
            unsigned int sizeBitData = node->bitData.size() - 1;
            for(unsigned int j = 0; j < sizeBitData; j++) {
                if(node->bitData.at(j) == '1') {
                    if(nav->right == nullptr) {
                        nav->right = (j == sizeBitData-1)
                                ? new HuffmanNode(NULL, node->data, node->bitData)
                                : new HuffmanNode(NULL, '\0');
                    }
                    nav = nav->right;
                } else {
                    if(nav->left == nullptr) {
                        nav->left = (j == sizeBitData-1)
                                ? new HuffmanNode(NULL, node->data, node->bitData)
                                : new HuffmanNode(NULL, '\0');
                    }
                    nav = nav->left;
                }
            }
            nav = this->tree->root;
        }
        this->GenerateBitTextFromHuffmanText();
        this->GenerateNormalText();
        this->finished = true;
    }

    void Decode(string textPath, string encodedTextPath, string keyPath) {
        this->Decode(encodedTextPath, keyPath);
        this->ExportTo(textPath, keyPath, HUFFMAN_ENCODED_TEXT);
    }

    void ExportTo(string textPath, string keyPath, int textMode) {
        if(!finished)
            return;

        if(textMode == HUFFMAN_NORMAL_TEXT) {
            FileManager::CreateOutputFile(&this->huffmanText, 1, textPath);

            ListNode<HuffmanNode*>* nav = this->list->begin;
            int size = this->list->GetSize()+1;
            string* text = new string[unsigned(size)];

            for(int i = 0; i < size-1; i++) {
                text[i] = (nav->value->data);
                text[i].append("|" + (nav->value->bitData));
                nav = nav->next;
            }
            text[size-1] = to_string(this->bitTextOverflow);
            FileManager::CreateOutputFile(text, size, keyPath);
        } else {
            FileManager::CreateOutputFile(&this->targetText, 1, textPath);
        }
    }

private:
    bool Contains(char value) {
        ListNode<HuffmanNode*>* temp = this->list->begin;
        if(temp == nullptr)
            return false;

        do {
            if(temp->value->data == value)
                return true;
            temp = temp->next;
        } while(temp != nullptr);

        return false;
    }

    void SortByFrequency() {
        int size = this->list->GetSize();

        for(int j = 0; j < size; j++) {
            for(int i = 0; i < size - 1; i++) {
                ListNode<HuffmanNode*>* a = list->GetElementAt(i);
                ListNode<HuffmanNode*>* b = list->GetElementAt(i+1);
                if(a->value->frequency > b->value->frequency) {
                    if(a == list->begin) {
                        list->begin = b;
                    } else {
                        ListNode<HuffmanNode*>* previousA = list->GetElementAt(i-1);
                        previousA->next = b;
                    }
                    if(b == list->end)
                        list->end = a;
                    a->next = b->next;
                    b->next = a;
                }
            }
        }
    }

    LinkedList<HuffmanNode*>* CopyLinkedList() {
        int size = this->list->GetSize();
        LinkedList<HuffmanNode*>* newList = new LinkedList<HuffmanNode*>();
        for(int i = 0; i < size; i++) {
            HuffmanNode* node = this->list->GetValueAt(i);
            newList->AddEnd(new HuffmanNode(node->frequency, node->data));
        }
        return newList;
    }

    void SetBitData() {
        int size = this->list->GetSize();
//        #pragma omp parallel for num_threads(NTHREADS)
        for(int i = 0; i < size; i++) {
            HuffmanNode* node = this->list->GetValueAt(i);
            node->bitData = this->GetTrack(this->tree->root, node->data, "");
        }
    }

    string GetTrack(HuffmanNode* node, char value, string final) {
        if(node == nullptr)
            return "#";
        if(node->data == value)
            return final;

        string temp = GetTrack(node->left, value, final.append("0"));
        if(temp.at(temp.size()-1) == '#')
            return GetTrack(node->right, value, final.erase(final.size()-1).append("1"));
        else
            return temp;
    }

    int GetPositionFor(LinkedList<HuffmanNode*>* newList, HuffmanNode* value) {
        ListNode<HuffmanNode*>* temp = newList->begin;

        if(temp == nullptr)
            return -1;

        int i = 0;
        do {
            if(value->frequency > temp->value->frequency) {
                temp = temp->next;
                i++;
            } else {
                return i;
            }
        } while(temp != nullptr);
        return i;
    }

    int BinaryToDecimal(string bin) {
        int decimal = 0;
        int base = 1;
        for(int i = 1; i <= 8; i++) {
            if(bin.at(8-i) == '1')
                decimal += base;
            base *= 2;
        }
        return decimal;
    }

    string DecimalToBinary(int n) {
        string number = "";
        int i = 0;
        while (n > 0) {
            number.insert(0, to_string(n % 2));
            n = n / 2;
            i++;
        }
        while(number.size() < 8)
            number.insert(0, "0");
        return number;
    }

    void GenerateBitText() {
        this->bitText = "";
        ListNode<HuffmanNode*>* temp = this->list->begin;

        for(unsigned int i = 0; i < this->targetText.size(); i++) {
            char value = this->targetText.at(i);
            if(int(value) != 13) {
                while(temp->value->data != value)
                    temp = temp->next;
                this->bitText += temp->value->bitData;
                temp = this->list->begin;
            }
        }
    }

    void GenerateHuffmanText() {
        this->huffmanText = "";
        string bitText = this->bitText;


        while(bitText.size() % 8 != 0) {
            bitText.append("0");
            this->bitTextOverflow++;
        }

        int intSize = bitText.size() / 8;
        for(int i = 0; i < intSize; i++) {
            string temp = "";
            for(int j = 8*i; j < (8 + 8*i); j++)
                temp += bitText.at(j);
            this->huffmanText += char(BinaryToDecimal(temp));
        }
    }

    void GenerateNormalText() {
        this->targetText = "";
        HuffmanNode* temp = this->tree->root;
        for(unsigned int i = 0; i < this->bitText.size(); i++) {
            temp = (this->bitText.at(i) == '1') ? temp->right : temp->left;
            if(temp->data != '\0') {
                this->targetText += temp->data;
                temp = this->tree->root;
            }
        }
    }

    LinkedList<HuffmanNode*>* CreateListToDecode(LinkedList<string>* list) {
        LinkedList<HuffmanNode*>* newList = new LinkedList<HuffmanNode*>();
        int size = list->GetSize()-1;

        for(int i = 0; i < size; i++) {
            string value = list->GetValueAt(i);
            char data = value.at(0);

            if(int(value.at(0)) == 10 || int(value.at(0)) == 13) {
                i++;
                value = list->GetValueAt(i);
                while(value.at(0) != '1' && value.at(0) != '0')
                    value.erase(value.begin());
            } else {
                value.erase(value.begin());
                value.erase(value.begin());
            }

            HuffmanNode* newNode = new HuffmanNode(NULL, data, value);
            newList->AddEnd(newNode);
        }
        this->bitTextOverflow = stoi(list->GetValueAt(size));
        return newList;
    }

    void GenerateBitTextFromHuffmanText() {
        string tempString, texts[NTHREADS];
        char targetChar;
        int size = int(this->huffmanText.size()),
                    threadSize = size / NTHREADS,
                    sizeRest = size - (threadSize * NTHREADS),
                    temp, id;

//        #pragma omp parallel num_threads(NTHREADS) default(none) private(targetChar, temp, id) shared(size, threadSize)
//        {
//            #pragma omp single
//            {
//                for(int j = 0; j < NTHREADS; j++) {
//                    #pragma omp task
//                    {
//                        id = j;
//                        tempString = "";
//                        for(int i = threadSize * id; i < threadSize * (id+1); i++) {
//                            targetChar = this->huffmanText.at(unsigned(i));
//                            temp = (int(targetChar) < 0) ? int(targetChar) + 256 : int(targetChar);

//                            if(!(int(temp) == 13 && int(this->huffmanText.at(unsigned(i)+1)) == 10))
//                                tempString += DecimalToBinary(temp);
//                        }
//                        texts[id] = tempString;
//                    }
//                }
//            }
//        }


        this->bitText = "";
        for(int i = 0; i < size; i++) {
            targetChar = this->huffmanText.at(unsigned(i));
            temp = (int(targetChar) < 0) ? int(targetChar) + 256 : int(targetChar);

            if(!(int(temp) == 13 && int(this->huffmanText.at(unsigned(i)+1)) == 10))
                this->bitText += DecimalToBinary(temp);
        }
        for(int i = 0; i < NTHREADS; i++) {
            this->bitText += texts[i];
        }

        for(int i = 0; i < this->bitTextOverflow; i++)
            this->bitText.erase(this->bitText.end()-1);
    }

private:
    HuffmanTree* tree;
    LinkedList<HuffmanNode*>* list;
    string targetText, bitText, huffmanText;
    bool finished;
    int bitTextOverflow;
};

#endif // HUFFMAN_H
