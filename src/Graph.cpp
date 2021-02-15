#include <iostream>
#include "../include/Graph.h"
#include "algorithm"


//rule of 5
Graph::Graph(std::vector<std::vector<int>> matrix) : edges(), infectedNodes()  ,sickNodes() , numberOfVertices(matrix.size()){
    int len1 = matrix.size();
    for (int i = 0; i< len1; i++){
        int len2 = matrix[i].size();
        for (int j = i + 1; j < len2 ; j++){
            if (matrix[i][j] == 1){
                std::vector<int> curr{i, j};
                this->edges.push_back(curr);
            }
        }
    }

}
Graph::Graph() : edges(), infectedNodes() , sickNodes() , numberOfVertices(){
}
Graph::~Graph(){clear();
}
Graph::Graph(const Graph& other) : edges(), infectedNodes(other.infectedNodes) , sickNodes(other.sickNodes)  , numberOfVertices(other.numberOfVertices){
    for (std::vector<int> x : other.edges){
        edges.push_back(x);
    }


}
Graph& Graph::operator=(const Graph& other){
    if (this != &other){
        clear();
        //delete this;
        for (std::vector<int> x : other.edges){
            edges.push_back(x);
        }
        infectedNodes = other.infectedNodes;
        sickNodes = other.sickNodes;
        numberOfVertices = other.numberOfVertices;
    }
    return *this;

}
Graph::Graph(Graph&& other) : edges() , infectedNodes() , sickNodes() , numberOfVertices(other.numberOfVertices){
    //numberOfVertices = (other.numberOfVertices);
    int size = other.edges.size();
    for (int i = 0 ; i < size ; i ++){
        edges.push_back(other.edges[i]);
    }
    size = other.infectedNodes.size();
    for (int i = 0 ; i < size ; i ++){
        infectedNodes.push_back(other.infectedNodes[i]);
    }
    size = other.sickNodes.size();
    for (int i = 0 ; i < size ; i ++){
        sickNodes.push_back(other.sickNodes[i]);
    }
    other.clear();
}
Graph& Graph::operator=(Graph&& other){
    if (this != &other){
        this->numberOfVertices = other.numberOfVertices;
        int size = other.edges.size();
        for (int i = 0 ; i < size ; i ++){
            edges.push_back(other.edges[i]);
        }
        size = other.infectedNodes.size();
        for (int i = 0 ; i < size ; i ++){
            infectedNodes.push_back(other.infectedNodes[i]);
        }
        size = other.sickNodes.size();
        for (int i = 0 ; i < size ; i ++){
            sickNodes.push_back(other.sickNodes[i]);
        }

    }
    other.clear();
    return *this;
}

//other function
void Graph::clear() {
    edges.clear();
    sickNodes.clear();
    infectedNodes.clear();
}
void Graph::infectNode(int nodeInd){
    if(!isInfected(nodeInd)) {          //check if the node has been already infected
        this->infectedNodes.push_back(nodeInd);
        this->sickNodes.push_back(nodeInd);
    }
}
int Graph::dequeueFirst(){
    if (infectedNodes.empty()){return -1;}
    int curr = infectedNodes[0];
    infectedNodes.erase(infectedNodes.begin());
    return curr;
}
bool Graph::isInfected(int nodeInd){
    int size = sickNodes.size();
    for (int i = 0 ; i < size ; i++){
        if (sickNodes[i] == nodeInd){return true;}
    }
    return false;
}
std::vector<int> * Graph::neighborsOf(int vertex)  {        //create a int vector of neighbors
    std::vector<int>* curr = new  std::vector<int>();
    int size = this->edges.size();
    for (int i = 0 ; i < size ; i ++){
        if (edges[i][0] == vertex) {(*curr).push_back(edges[i][1]);}
        else if (edges[i][1] == vertex) {(*curr).push_back(edges[i][0]);}
    }
    sort(curr->begin(), curr->begin() + curr->size());
    return curr;
}
void Graph::removeNodeFromGraph(int node){
    for (int i = 0 ; i < (int)this->edges.size(); i++){
        if (this->edges[i][0] == node || this->edges[i][1] == node){
            this->edges.erase(this->edges.begin() + i);
            i--;
        }
    }
}
int Graph::getNumberOfVertices() const {return this->numberOfVertices;}
std::vector<std::vector<int>> Graph::getEdges() const{
    return this->edges;
}
std::vector<int> Graph::getSickNodeVec() const {
    std::vector<int> a(sickNodes); // = new std::vector <int>(sickNodes);
    sort(a.begin(), a.begin() + a.size());      //TODO
    //return sickNodes;
    return a;
}
bool Graph::isSick(int node){
    int size = sickNodes.size();
    for (int i = 0 ; i < size ; i++){
        if (sickNodes[i] == node){return true;}
    }
    return false;
}

