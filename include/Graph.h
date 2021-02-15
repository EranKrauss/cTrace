#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <algorithm>


class Graph{
public:
    //rule of 5
    Graph(std::vector<std::vector<int>> matrix);
    Graph();
    ~Graph();
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    Graph(Graph&& other);
    Graph& operator=(Graph&& other);

    //getters and setters
    std::vector<std::vector<int>> getEdges() const;           //does it send a copy ??
    int getNumberOfVertices () const;
    std::vector<int> getSickNodeVec() const ;

    //functions
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    std::vector<int> * neighborsOf(int vertex) ;
    void removeNodeFromGraph(int node);
    int dequeueFirst();
    bool isSick(int node);
    void clear();



private:
    std::vector<std::vector<int>> edges;
    std::vector<int> infectedNodes;                      //sick now
    std::vector<int> sickNodes;                          //were sick
    int numberOfVertices;

};

#endif
