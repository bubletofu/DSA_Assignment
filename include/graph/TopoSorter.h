/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"

template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }
    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        // TODO
        if (mode == DFS)
            return dfsSort(sorted);
        else
            return bfsSort(sorted);
    }
    DLinkedList<T> bfsSort(bool sorted = true)
    {
        // TODO
        DLinkedList<T> topoOrder;
        xMap<T, int> indegreeMap = vertex2inDegree(this->hash_code);
        DLinkedListSE<T> list = listOfZeroInDegrees();
        if (sorted)
            list.sort();

        Queue<T> open;
        for (typename DLinkedListSE<T>::Iterator it = list.begin(); it != list.end(); it++)
            open.push(*it);
        while (!open.empty())
        {
            T vertex = open.pop();
            topoOrder.add(vertex);

            DLinkedListSE<T> children = this->graph->getOutwardEdges(vertex);
            // if (sorted)
            //     children.sort();
            for (typename DLinkedListSE<T>::Iterator it = children.begin(); it != children.end(); it++)
            {
                T child = *it;
                if (open.contains(child))
                    continue;
                if (topoOrder.contains(child))
                    continue;

                int new_indeg = indegreeMap.get(child) - 1;
                indegreeMap.put(child, new_indeg);
                if (new_indeg == 0)
                    open.push(child);
            }
        }
        return topoOrder;
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TODO
        xMap<T, bool> visited(this->hash_code);
        DLinkedList<T> topo;
        Stack<T> stack;
        DLinkedList<T> list = listOfZeroInDegrees();
        for (typename DLinkedList<T>::Iterator it = list.begin(); it != list.end(); it++)
        {
            if (!visited.containsKey(*it))
            {
                // cout << "visited: " << *it << endl;
                helper(*it, visited, stack);
            }
        }
        while (!stack.empty())
        {
            topo.add(stack.peek());
            stack.pop();
        }
        return topo;
    }

protected:
    void helper(T vertex, xMap<T, bool> &visited, Stack<T> &stack)
    {
        visited.put(vertex, true);
        DLinkedList<T> vertices = this->graph->getOutwardEdges(vertex);
        for (typename DLinkedList<T>::Iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            if (!visited.containsKey(*it))
            {
                helper(*it, visited, stack);
            }
        }
        stack.push(vertex);
    }
    // Helper functions

    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        xMap<T, int> map(hash);
        typename DGraphModel<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            map.put(vertex, inDegree);

            vertexIt++;
        }
        return map;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T &, int))
    {
        xMap<T, int> map(hash);
        typename DGraphModel<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int outDegree = this->graph->outDegree(vertex);
            map.put(vertex, outDegree);

            vertexIt++;
        }
        return map;
    }
    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> list;
        typename DGraphModel<T>::Iterator vertexIt = this->graph->begin();
        while (vertexIt != this->graph->end())
        {
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            if (inDegree == 0)
                list.add(vertex);

            vertexIt++;
        }
        return list;
    }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
