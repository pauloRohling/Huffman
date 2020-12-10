#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <iostream>

using namespace std;

template <class Type>
class ListNode {
public:
    ListNode() {
        this->next = nullptr;
    }

    ListNode(Type value) {
        this->value = value;
        this->next = nullptr;
    }

    ListNode(Type value, ListNode<Type>* next) {
        this->value = value;
        this->next = next;
    }

    Type value;
    ListNode<Type>* next;
};

template <class Type>
class LinkedList {
public:
    LinkedList() {
        this->begin = nullptr;
        this->end = nullptr;
    }

    LinkedList(Type value) {
        ListNode<Type>* element = new ListNode<Type>(value);
        this->begin = element;
        this->end = element;
    }

    void Destroy() {
        ListNode<Type>* temp = this->begin;
        ListNode<Type>* next;

        while(temp != nullptr) {
            next = temp->next;
            delete temp;
            temp = next;
        }

        delete temp;
        delete next;

        this->begin = nullptr;
        this->end = nullptr;
    }

    void AddEnd(Type value) {
        ListNode<Type>* element = new ListNode<Type>(value);
        if(this->IsEmpty()) {
            this->begin = element;
            this->end = element;
        } else {
            this->end->next = element;
            this->end = element;
        }
    }

    void AddBegin(Type value) {
        ListNode<Type>* element = new ListNode<Type>(value, this->begin);
        if(this->IsEmpty()) {
            this->begin = element;
            this->end = element;
        } else {
            this->begin = element;
        }
    }

    void AddAt(Type value, int position) {
        if(this->PositionIsValid(position)) {
            if(position == 0) {
                this->AddBegin(value);
            } else {
                ListNode<Type>* previousElement = GetElementAt(position-1);
                ListNode<Type>* nextElement = GetElementAt(position);
                ListNode<Type>* element = new ListNode<Type>(value, nextElement);
                previousElement->next = element;
            }
        } else if(position >= GetSize()) {
            this->AddEnd(value);
        }
    }

    void RemoveEnd() {
        if(!this->IsEmpty()) {
            if(this->begin == this->end) {
                delete this->end;
                this->begin, this->end = nullptr;
            } else {
                ListNode<Type>* element = GetElementAt(GetSize()-2);
                element->next = nullptr;
                delete this->end;
                this->end = element;
            }
        }
    }

    void RemoveBegin() {
        if(!this->IsEmpty()) {
            if(this->begin == this->end) {
                delete this->begin;
                this->begin, this->end = nullptr;
            } else {
                ListNode<Type>* element = GetElementAt(1);
                delete this->begin;
                this->begin = element;
            }
        }
    }

    void RemoveAt(int position) {
        if(position == 0) {
            this->RemoveBegin();
        } else if(position == (GetSize()-1)) {
            this->RemoveEnd();
        } else if(this->PositionIsValid(position)) {
            ListNode<Type>* element = GetElementAt(position);
            ListNode<Type>* previousElement = GetElementAt(position-1);
            ListNode<Type>* nextElement = GetElementAt(position+1);
            previousElement->next = nextElement;
            delete element;
        }
    }

    Type GetValueAt(int position) {
        if(this->PositionIsValid(position) && !this->IsEmpty()) {
            ListNode<Type>* temp = this->begin;
            for(int i = 0; i < position; i++)
                temp = temp->next;
            return temp->value;
        }
        return NULL;
    }

    ListNode<Type>* GetElementAt(int position) {
        if(this->PositionIsValid(position) && !this->IsEmpty()) {
            ListNode<Type>* temp = this->begin;
            for(int i = 0; i < position; i++)
                temp = temp->next;
            return temp;
        }
        return nullptr;
    }

    int GetPositionOf(Type value) {
        ListNode<Type>* temp = this->begin;
        if(temp == nullptr)
            return -1;

        int i = 0;
        do {
            if(temp->value == value)
                return i;
            temp = temp->next;
            i++;
        } while(temp != nullptr);

        return -1;
    }

    bool Contains(Type value) {
        ListNode<Type>* temp = this->begin;
        if(temp == nullptr)
            return false;

        do {
            if(temp->value == value)
                return true;
            temp = temp->next;
        } while(temp != nullptr);

        return false;
    }

    void Print() {
        ListNode<Type>* temp = this->begin;
        int i = 0;
        printf("###### LINKED LIST ######\n");
        printf("POSITION\tVALUE\n");
        while(temp != nullptr) {
            cout << "[" << i << "]\t\t" << temp->value << endl;
            temp = temp->next;
            i++;
        }

        cout << "\nTamanho da Lista: " << this->GetSize() << endl << endl;
    }

    int GetSize() {
        ListNode<Type>* temp = this->begin;
        if(temp == nullptr)
            return 0;

        int i = 1;
        while(temp->next != nullptr) {
            temp = temp->next;
            i++;
        }
        return i;
    }

public:
    ListNode<Type>* begin;
    ListNode<Type>* end;

private:
    bool IsEmpty() {
        return (this->begin == nullptr);
    }

    bool PositionIsValid(int position) {
        return (position >= 0 && position < this->GetSize());
    }
};

#endif // LINKEDLIST_H
