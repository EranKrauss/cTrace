#include "../include/Session.h"
#include "../include/Agent.h"
#include "../include/json.hpp"
#include <fstream>
#include <sstream>


//rule of 5
Session::Session(const std::string& path) : g(), treeType(), agents(), currCycle(0){
    std::ifstream stream(path);
    nlohmann::json reader;
    stream >> reader;

    //initialize g
    vector <vector <int>> graph;

    for (auto& elem : reader["graph"]){
        graph.push_back(elem);
    }
    g = Graph(graph);

    //initialized agents
    for (auto& elem: reader["agents"]){
        Agent* curr;
        if (elem[0] == "V"){
            curr = new Virus(elem[1]);
            //g.infectNode(elem[1]);
            //g.infectNode(elem[1]);              //add to sick
        }
        else {
            curr = new ContactTracer();
        }
        this->agents.push_back(curr);
    }


    //initialize treeType
    string c = reader["tree"];
    if(c == "M") {this->treeType = MaxRank;}
    else if (c == "C") {this->treeType = Cycle;}
    else {this->treeType = Root;}



}
Session::~Session(){
    clear();

}
Session::Session(const Session&  other) : g(other.g) , treeType(other.treeType) , agents(other.agents) , currCycle(other.currCycle) {
    int a = other.agents.size();
    for (int i = 0 ; i < a ; i++){
        this->agents.push_back((*other.agents[i]).clone());
    }

}
Session& Session::operator=(const Session& other){
    if (this != &other){
        clear();
        g = other.g;
        treeType = other.treeType;
        currCycle = other.currCycle;
        int size = other.agents.size();
        for (int i = 0 ; i < size ; i++){
            agents.push_back(other.agents[i]);
        }

    }

    return *this;
}
Session::Session(Session&& other) :   g(other.g)  , treeType(other.treeType), agents(other.agents) , currCycle(other.currCycle)  {
    other.g.clear();
    int size = other.agents.size();
    for (int i = 0 ; i < size ; i++){
        agents.push_back(other.agents[i]);
    }
    other.agents.clear();
}
Session& Session::operator=(Session&& other){
    if (this != &other){
        clear();
        g = other.g;
        other.g.clear();
        int size = other.agents.size();
        for (int i = 0 ; i < size ; i++){
            agents.push_back(other.agents[i]);
        }
        other.agents.clear();
        currCycle = (other.currCycle);
        treeType = (other.treeType);
    }

    return *this;
}

//other function
Graph Session::getGraph() const {     //TODO
    return (this->g);
}
void Session::simulate(){
    while (!terminationCondition()){
        int counter = agents.size();
        for (int i = 0; i < counter ; i++){         //every turn all the agents are acting.
            Agent* a = this->agents[i];
            a->act(*this);

        }

        currCycle++;        //calculate currCycle -> for currCycletree
    }

    //create vector Graph
    std::vector<std::vector<int>>* graph = new vector<vector<int>>();
    for (int i = 0; i < this->getGraph().getNumberOfVertices() ; i++){
        vector<int>* curr = new vector<int>();
        for (int j = 0 ; j < this->g.getNumberOfVertices() ; j++){
            curr->push_back(0);
        }
        graph->push_back(*curr);
        delete curr;
    }

    for (std::vector<int> x:this->getGraph().getEdges()){
        (*graph)[x[0]][x[1]] = 1;
        (*graph)[x[1]][x[0]] = 1;
    }


    //create the output file and insert the relevant text
    nlohmann::json j;
    j["graph"] = *graph;
    j["infected"] = this->g.getSickNodeVec();
    std::ofstream i;
    i.open("output.json");
    j >> i;
    i.close();

    graph->clear();
    delete graph;
}
void Session::addAgent(const Agent& agent){
    agents.push_back(agent.clone());
}
void Session::setGraph(const Graph& graph){

    g = graph;
}
void Session::enqueueInfected(int node){

    this->g.infectNode(node);

}
int Session::dequeueInfected(){
    return this->g.dequeueFirst();

}
TreeType Session::getTreeType() const{return this->treeType;}
int Session::getCurrCycle() const{return currCycle;}
bool Session::terminationCondition(){
    vector<vector<int>>* connectedComponents = createComponents();      //create a vector of vectors which each vector is a connected componenet
    bool stop = true;
    int size = connectedComponents->size();
    //for each connected componnet check if it fuuly healy or fully infected
    for (int i = 0 ; i < size && stop ;i++){
        bool firstCon = fullyHealthy((*connectedComponents)[i]);
        bool secondCon = fullyInfected((*connectedComponents)[i]);
        if (!firstCon & !secondCon){
            stop = false;
        }
    }
    delete connectedComponents;
    return stop;
}
bool Session::fullyHealthy(vector<int>& comp){  //get a connected component and check if it fully healty
    bool fully = true;
    for (int x : comp){
        if (hasAgent(x)){fully = false;}
    }
    return fully;
}
bool Session::hasAgent(int node){
    for (Agent* agent : agents){
        if(agent->getNodeInd() == node){return true;}
    }
    return false;
}
bool Session::fullyInfected(vector<int>& comp) {    //get a connected component and check if it fully infected
    bool fully = true;
    for (int x : comp){
        if(!g.isSick(x)) {fully = false;}
    }
    return fully;
}
vector<vector<int>>* Session::createComponents(){    //create a vector of vectors which each vector is a connected componenet
    vector<bool>* visited = new vector<bool>();
    vector <vector<int>> *allGraph = new vector<vector<int>>();
    for (int i = 0 ; i < g.getNumberOfVertices() ; i++){
        visited->push_back(false);
    }

    for (int i = 0 ; i < g.getNumberOfVertices(); i++){
        if (!(*visited)[i]){
            vector<int>* curr = new vector<int>();
            DFSUtil(i , visited, curr);
            allGraph->push_back((*curr));
            delete curr;

        }
    }

    (*visited).clear();
    delete visited;

    return (allGraph);
}
void Session::DFSUtil(int v , vector<bool>* visited, vector<int>* answer){
    (*visited)[v] = true;
    answer->push_back(v);

    vector <int>* neighbors = (g.neighborsOf(v));
    int a = (*neighbors).size();
    for (int i = 0 ; i < a ; i++){
        if (!(*visited)[(*neighbors)[i]]){
            DFSUtil((*neighbors)[i] , visited , answer);
        }
    }
    delete neighbors;


}   //using by the createComponents function
void Session::clear() {
    g.clear();
    for (Agent* agent : agents){
        delete agent;
    }
    //delete &agents;
}

