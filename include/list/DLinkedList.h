/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;
template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node* head; // this node does not contain user's data
    Node* tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T& lhs, T& rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T>*); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T>*) = 0,
        bool (*itemEqual)(T&, T&) = 0);
    DLinkedList(const DLinkedList<T>& list);
    DLinkedList<T>& operator=(const DLinkedList<T>& list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T& get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string(*item2str)(T&) = 0);
    // Inherit from IList: END

    void println(string(*item2str)(T&) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T>*) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T>* list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete* it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    Iterator begin() const {
        return Iterator(const_cast<DLinkedList<T>*>(this), true);
    }
    Iterator end() const {
        return Iterator(const_cast<DLinkedList<T>*>(this), false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

    Iterator bbegin() const {
        return BWDIterator(const_cast<DLinkedList<T>*>(this), true);
    }
    Iterator bend() const {
        return BWDIterator(const_cast<DLinkedList<T>*>(this), false);
    }    

protected:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T&))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T>& list);
    void removeInternalData();
    void checkIndex(int index);
    Node* getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node* next;
        Node* prev;
        friend class DLinkedList<T>;

    public:
        Node(Node* next = 0, Node* prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next = 0, Node* prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T>* pList;
        Node* pNode;

    public:
        Iterator(DLinkedList<T>* pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator& operator=(const Iterator& iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node* pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T& operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator& iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator& operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator
    {
    private:
        DLinkedList<T>* pList;
        Node* pNode;
    public:
        BWDIterator(DLinkedList<T>* pList = 0, bool bbegin = true)
        {
            if (bbegin)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->head;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        BWDIterator& operator=(const BWDIterator& BWDiterator)
        {
            this->pNode = BWDiterator.pNode;
            this->pList = BWDiterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node* pNext = pNode->next;
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
        T& operator*()
        {
            return pNode->data;
        }
        bool operator!=(const BWDIterator& BWDiterator)
        {
            return pNode != BWDiterator.pNode;
        }
        // Prefix -- overload
        BWDIterator& operator--()
        {
            pNode = pNode->prev;
            return *this;
        }
        // Postfix ++ overload
        BWDIterator operator--(int)
        {
            BWDIterator BWDiterator = *this;
            --(*this);
            return BWDiterator;
        }
        //Node* getNode() const { return pNode; }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T>*),
    bool (*itemEqual)(T&, T&))
{
    // TODO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;

    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;

    this->count = 0;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T>& list)
{
    // TODO
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;

    this->count = 0;

    int cnt = 0;
    Node *listCurrent = list.head->next;
    while(cnt < list.count){
        this->add(cnt, listCurrent->data);
        listCurrent = listCurrent->next;
        cnt++;
    }

}

template <class T>
DLinkedList<T>& DLinkedList<T>::operator=(const DLinkedList<T>& list)
{
    // TODO
    if(this == &list){
        return *this;
    }
    clear();
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    int cnt = 0;
    Node *listCurrent = list.head->next;
    while(cnt < list.count){
        this->add(cnt, listCurrent->data);
        listCurrent = listCurrent->next;
        cnt++;
    }
    return *this;

}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    this->clear();
    delete head; 
    delete tail; 
    head = nullptr; 
    tail = nullptr;
}

template <class T>
void DLinkedList<T>::checkIndex(int index)
{
    /**
     * Validates whether the given index is within the valid range of the list.
     * Throws an std::out_of_range exception if the index is negative or exceeds the number of elements.
     * Ensures safe access to the list's elements by preventing invalid index operations.
     */
    // TODO:
    if (!(index >= 0 && index <= this->count)) {
        throw std::out_of_range("Index is out of range!");
    }
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    Node *newE = new Node(e);
    newE->prev = tail->prev;
    newE->next = tail;
    tail->prev->next = newE;
    tail->prev = newE;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
   checkIndex(index);
    Node *current = this->head->next;
    if(index == 0){
        Node* newE = new Node(e, this->head->next, this->head);
        this->head->next->prev = newE;
        this->head->next = newE;
    }
    else if(index == this->count){
        Node* newE = new Node(e, this->tail, this->tail->prev);
        this->tail->prev->next = newE;
        this->tail->prev = newE;
    }
    else{
        Node *newE = new Node(e);
        int cnt = 0;
        while(cnt != index){
            current = current->next;
            cnt++;
        }
        newE->prev = current->prev;
        newE->next = current;
        current->prev->next = newE;
        current->prev = newE;
    }
    this->count++;

    current = nullptr;
}

template <class T>
typename DLinkedList<T>::Node* DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
     // TODO    
    Node *current = this->head;
    for(int i = 0; i < index; i++){
        current = current->next;
    }
    return current;

}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if (index < 0 || index > this->count - 1) {
        throw std::out_of_range("Index is out of range!");
    }
  Node *nodeToRemove;
    T removedValue;

    
    if (index == 0) {
        nodeToRemove = head->next;
        head->next = nodeToRemove->next;
        if (nodeToRemove->next) {
            nodeToRemove->next->prev = head;
        } else {
            tail = head; 
        }
    } 
    else if (index == count - 1) {
        nodeToRemove = tail->prev;
        tail->prev = nodeToRemove->prev;
        nodeToRemove->prev->next = tail;
    } 
    else {
        Node *prevNode = getPreviousNodeOf(index);
        nodeToRemove = prevNode->next;
        prevNode->next = nodeToRemove->next;
        nodeToRemove->next->prev = prevNode;
    }
    removedValue = nodeToRemove->data;
    delete nodeToRemove;
    this->count--;

    return removedValue;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return this->count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return this->count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    if (deleteUserData) {
        deleteUserData(this);
    }

        Node* current = this->head->next;
        while (current != tail) {
            Node* next = current->next;
            delete current;
            current = next;
        }

    this->head->next = tail;
    this->tail->prev = head;
    this->count = 0;
}

template <class T>
T& DLinkedList<T>::get(int index)
{
    // TODO
    if (index < 0 || index > this->count - 1) {
        throw std::out_of_range("Index is out of range!");
    }
     Node *current = this->head;
    for (int i = 0; i <= index; i++) {
        current = current->next;
    }
    
    return current->data;


}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    Node *current = this->head->next; //index = 0
    int index = 0;
    
    while (current != tail) {
        if (this->equals(current->data, item, this->itemEqual)){
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; 

    delete current;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    Node* current = this->head->next;
        while (current->next != nullptr){
            if (this->equals(current->data, item, this->itemEqual)){
                
                current->prev->next = current->next;
                current->next->prev = current->prev;
                this->count--;
                if(removeItemData != nullptr){
                    removeItemData(current->data);
                }
                delete current;
                return true;
            }
            else{ 
                current = current->next;
            }
        }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string(*item2str)(T&))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
     // TODO
    stringstream os;
    os << "[";
    bool firstLetter = true;
    for (Iterator it = begin(); it != end(); ++it) {
        if (!firstLetter) {
            os << ", ";
        }
        if (item2str) {
            os << item2str(*it);
        } else {
            os << *it;
        }
        firstLetter = false;
    }
    os << "]";
    return os.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T>& list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
     // TODO
    this->clear();
    int index = 0;
    for (Iterator it = list.begin(); it != list.end(); ++it) {
        this->add(index, *it);
        index++;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
     // TODO
    /* Node* current = this->head->next;
    while (current != tail) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;

    if (deleteUserData != nullptr) {
        deleteUserData(this);
    } */
}

#endif /* DLINKEDLIST_H */
