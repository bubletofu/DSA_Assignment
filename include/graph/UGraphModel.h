/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (fromNode == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));
        if (toNode == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        if (fromNode != toNode)
        {
            fromNode->connect(toNode, weight);
            toNode->connect(fromNode, weight);
        }
        else
        {
            fromNode->connect(toNode, weight);
        }
    }

    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (fromNode == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));
        if (toNode == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        // typename AbstractGraph<T>::Edge *edge = fromNode->getEdge(toNode);
        // if (edge == nullptr)
        // {
        //     throw EdgeNotFoundException(this->vertex2str(from));
        // }
        
        if (!this->connected(from, to))
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");

        if (fromNode != toNode)
        {
            fromNode->removeTo(toNode);
            toNode->removeTo(fromNode);
        }
        else
        {
            fromNode->removeTo(toNode);
        }
    }

    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *node = this->getVertexNode(vertex);
        if (node == nullptr)
            throw VertexNotFoundException(this->vertex2str(vertex));

        typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = this->nodeList.begin();
        while (it != this->nodeList.end())
        {
            typename AbstractGraph<T>::VertexNode *otherNode = *it;

            otherNode->removeTo(node);
            node->removeTo(otherNode);

            it++;
        }

        this->nodeList.removeItem(node, nullptr);
        delete node;
    }
    static UGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges,
                                  bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
    {
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }

        for (int i = 0; i < nedges; i++)
        {
            typename AbstractGraph<T>::VertexNode *fromNode = nullptr;
            typename AbstractGraph<T>::VertexNode *toNode = nullptr;

            typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = graph->nodeList.begin();
            while (it != graph->nodeList.end())
            {
                typename AbstractGraph<T>::VertexNode *node = *it;
                if (vertexEQ(node->vertex, edges[i].from))
                {
                    fromNode = node;
                    break;
                }
                it++;
            }

            it = graph->nodeList.begin();
            while (it != graph->nodeList.end())
            {
                typename AbstractGraph<T>::VertexNode *node = *it;
                if (vertexEQ(node->vertex, edges[i].to))
                {
                    toNode = node;
                    break;
                }
                it++;
            }

            if (fromNode == nullptr || toNode == nullptr)
            {
                throw VertexNotFoundException(graph->vertex2str(edges[i].from));
            }

            graph->connect(fromNode->vertex, toNode->vertex, edges[i].weight);
        }

        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
