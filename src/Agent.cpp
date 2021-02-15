#include "../include/Agent.h"
#include "../include/Tree.h"


////////////////////////////////Agent//////////////////////////////////////
////rule Of 5
Agent::Agent()= default;
Agent::~Agent()= default;
Agent::Agent(const Agent& other)=default;


//////////////////////////////////Virus//////////////////////////////////////
//rule Of 5
Virus::Virus(int nodeInd) : nodeInd(nodeInd){}
Virus::~Virus(){}
Virus::Virus(const Virus& other) : nodeInd(other.nodeInd){}

//other function
int Virus::getNodeInd() const{
    return this->nodeInd;
}
Agent* Virus::clone() const{
    return new Virus(*this);
}
void Virus::act(Session &session) {
    session.enqueueInfected(this->nodeInd);     //infect myself
    bool found(false);
    std::vector<int>* a = session.getGraph().neighborsOf(this->nodeInd);
    int size = a->size();
    for (int i = 0 ; i < size && !found ; i++){ //finding a not occupy node - if it find its spread
        if (!(session.getGraph().isInfected((*a)[i]))  & !session.hasAgent((*a)[i])){
            found = true;
            session.addAgent(Virus((*a)[i]));      //spreading to another node
        }
    }

    delete a;
}


/////////////////////////////////ContactTracer///////////////////////////////
//rule Of 5
ContactTracer::ContactTracer() =default;
ContactTracer::~ContactTracer()=default;
ContactTracer::ContactTracer(const ContactTracer& other){}

//other function
Agent* ContactTracer::clone() const{
    return new ContactTracer(*this);
}
int ContactTracer::getNodeInd() const{return (-1);}
void ContactTracer::act(Session &session){
    int startingNode = session.dequeueInfected();       //get the first sick node from the list
    if (startingNode != -1) {
        Tree *bfsTree = Tree::createTree(session, startingNode);    //create tree by using bfs algorithm
        Graph *g;
        g = new Graph(session.getGraph());
        g->removeNodeFromGraph(bfsTree->traceTree());               //create a new relevant graph
        session.setGraph(*g);                                       //replace the old graph
        delete bfsTree;
        delete g;
    }


}
