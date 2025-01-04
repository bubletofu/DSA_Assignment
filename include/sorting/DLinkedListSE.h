/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T>
{
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {

                                        };

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0)
    {
        // TODO: implement this function
        //      - You should implement the merge sort algorithm
        if (this->count > 0)
        {
            typename DLinkedList<T>::Node *first = this->head->next;
            this->tail->prev->next = nullptr;
            this->head->next->prev = nullptr;
            mergeSort(first, comparator);

            typename DLinkedList<T>::Node *last = first;
            while (last->next != nullptr)
                last = last->next;

            last->next = this->tail;
            this->tail->prev = last;
            this->head->next = first;
            first->prev = this->head;
        }

    };

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }
    void mergeSort(typename DLinkedList<T>::Node *&head, int (*comparator)(T &, T &) = 0)
    {
        if ((head != nullptr) && (head->next != nullptr))
        {
            typename DLinkedList<T>::Node *second;
            devide(head, second);
            mergeSort(head, comparator);
            mergeSort(second, comparator);
            merge(head, second, comparator);
        }
    };
    void devide(typename DLinkedList<T>::Node *&first, typename DLinkedList<T>::Node *&second)
    {
        typename DLinkedList<T>::Node *mid = first;
        typename DLinkedList<T>::Node *last = first->next;
        while (last != nullptr)
        {
            last = last->next;
            if (last != nullptr)
            {
                last = last->next;
                mid = mid->next;
            }
        }
        second = mid->next;
        second->prev = 0;
        mid->next = 0;
    }
    void merge(typename DLinkedList<T>::Node *&first, typename DLinkedList<T>::Node *&second, int (*comparator)(T &, T &) = 0)
    {
        typename DLinkedList<T>::Node combined;
        typename DLinkedList<T>::Node *lastSorted = &combined;
        while ((first != nullptr) && (second != nullptr))
        {
            if (compare(first->data, second->data, comparator) <= 0)
            {
                lastSorted->next = first;
                first->prev = lastSorted;
                lastSorted = first;
                first = first->next;
            }
            else
            {
                lastSorted->next = second;
                second->prev = lastSorted;
                lastSorted = second;
                second = second->next;
            }
        }
        if (first == nullptr)
        {
            lastSorted->next = second;
            if (second != nullptr)
                second->prev = lastSorted;
            second = 0;
        }
        else
            lastSorted->next = first;
        if (first != nullptr)
            first->prev = lastSorted;

        first = combined.next;
    }
};

#endif /* DLINKEDLISTSE_H */
