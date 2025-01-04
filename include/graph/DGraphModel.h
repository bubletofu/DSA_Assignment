/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include <iomanip>

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

   
    void connect(T from, T to, float weight = 0)
    {
        typename AbstractGraph<T>::VertexNode *ver_from = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *ver_to = this->getVertexNode(to);

        if (ver_from == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        else if (ver_to == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        ver_from->connect(ver_to, weight);
    }

    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (fromNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (toNode == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }

        if (!this->connected(from, to))
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");

        fromNode->removeTo(toNode);
    }
    // void remove(T vertex)
    // {
    //     // TODO
    //     typename AbstractGraph<T>::VertexNode *node = this->getVertexNode(vertex);

    //     if (node == nullptr)
    //         throw VertexNotFoundException(this->vertex2str(vertex));

    //     for (auto vertex : this->nodeList)
    //     {
    //         typename AbstractGraph<T>::Edge *edge1 = vertex->getEdge(node);
    //         typename AbstractGraph<T>::Edge *edge2 = node->getEdge(vertex);
    //         if (edge1)
    //             vertex->removeTo(node);
    //         if (edge2)
    //             node->removeTo(vertex);
    //     }

    //     this->nodeList.removeItem(node, nullptr);
    //     delete node;
    // }

    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *rnode = this->getVertexNode(vertex);
        if (rnode == nullptr)
            throw VertexNotFoundException(this->vertex2str(vertex));
        else
        {
            for (auto vertex : this->nodeList)
            {
                typename AbstractGraph<T>::Edge *edge1 = vertex->getEdge(rnode);
                typename AbstractGraph<T>::Edge *edge2 = rnode->getEdge(vertex);
                if (edge1)
                    vertex->removeTo(rnode);
                if (edge2)
                    rnode->removeTo(vertex);
            }
            this->nodeList.removeItem(rnode);
            delete rnode;
        }
        // typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = this->nodeList.begin();
        // while (it!= this->nodeList.end())
        // {
        //     (*it)->removeTo(rnode);
        //     rnode->removeTo((*it));
        //     ++it;

        // }
        // this->nodeList.removeItem(rnode);
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        DGraphModel<T> *graph = new DGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; i++)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
