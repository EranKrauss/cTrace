#ifndef SESSION_H_
#define SESSION_H_
#include <vector>
//#include <iostream>
#include "iostream"
#include <fstream>
#include "json.hpp"
#include <sstream>
#include <string>
#include "Graph.h"


using json = nlohmann::json;
using namespace std;


class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    //rule of 5
    Session(const std::string& path);
    ~Session();
    Session(const Session&  other);
    Session& operator=(const Session& other);
    Session(Session&& other);
    Session& operator=(Session&& other);

    //getters and setters
    TreeType getTreeType() const;
    Graph getGraph() const;
    int getCurrCycle() const;

    //functions
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    bool terminationCondition();
    void enqueueInfected(int);
    int dequeueInfected();
    bool hasAgent(int node);
    void clear();



private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    int currCycle;

    //privatefunction
    bool fullyHealthy(vector<int>& comp);
    bool fullyInfected(vector<int>& comp);
    vector<vector<int>>* createComponents();
    void DFSUtil(int v, vector<bool>* visited, vector<int>* answer);
    json readJsonFile(string);
};

#endif
