#include "../include/Tree.h"



///////////////////////////////////Tree////////////////////////////
//rule of 5
Tree::Tree(int rootLabel) : node(rootLabel), children() {
}
Tree::~Tree(){
    clear();
    //delete &node;
}
Tree::Tree(const Tree& other) : node(other.node), children() {
    for (Tree* x : other.children){
        children.push_back(x->clone());
    }
}
Tree& Tree::operator=(const Tree& other){
    if (this != &other){
        clear();
        node = other.node;
        for (Tree* x: other.children){
            children.push_back(x);
        }
    }
    return *this;
}
Tree::Tree(Tree&& other) : node(other.node) , children(){
    int size = other.children.size();
    for (int i = 0 ; i < size ; i++){
        children.push_back(other.children[i]);
    }
    other.children.clear();
}
Tree& Tree::operator=(Tree&& other){
    if (this != &other){
        clear();
        node = other.node;
        int size = other.children.size();
        for (int i = 0 ; i < size ; i++){
            children.push_back(other.children[i]);
        }
        other.children.clear();
    }
    return *this;
}

//other functions
void Tree::clear() {
    //delete &node;
      for (Tree* x: children){
        delete x;

      }
      children.clear();
}
void Tree::setNode(int node) {this->node = node;}
int Tree::getNode()const  {return this->node;}
void Tree::setChildren(std::vector<Tree*> children) {
    for (Tree* x : children){
        this->children.push_back(x);
    }
}
std::vector<Tree*> Tree::getChildren() const {
    return this->children;
}
void Tree::addChild(const Tree& child){
    children.push_back(child.clone());
}
Tree* Tree::createTree(const Session& session, int rootLabel){
    std::vector<std::vector<int>>* vec = bfs(session,rootLabel); //each int vec is a node in the bfs
    Tree* tree = createTreeRec(rootLabel ,session.getCurrCycle(), *vec , session.getTreeType());
    delete vec;
    return tree;
}
std::vector<std::vector<int>>* Tree::bfs(const Session& session, int rootLabel){

    std::vector<std::vector<int>>* vec = new std::vector<std::vector<int>>();
    for (int i = 0 ; i < session.getGraph().getNumberOfVertices() ; i++){
        vector <int> curr{0,-1,-1}; //color , distance , father
        vec->push_back(curr);
    }
    vector<int> curr{1,0,-1};
    (*vec)[rootLabel] = curr;
    std::vector<int>* queue = new vector<int>();
    (*queue).push_back(rootLabel);

    while(!(*queue).empty()){
        int u = (*queue)[0];
        queue->erase(queue->begin());
        vector <int>* neighbors = session.getGraph().neighborsOf(u);
        for (int neighbor: *neighbors){
            if ((*vec)[neighbor][0] == 0){
                (*vec)[neighbor][0] = 1;                                //change color too gray
                (*vec)[neighbor][1] = (*vec)[u][1] + 1;                 //change distance
                (*vec)[neighbor][2] = u;                                //change "father"
                (*queue).push_back(neighbor);                           //insert neighbor too queue
            }
        }
        delete neighbors;
        (*vec)[u][0] = 2;
    }
    delete queue;
    return vec;
}
Tree* Tree::createTreeRec(int root ,int cycle,  std::vector<std::vector<int>>& bfsVec ,  TreeType treeType ){
    //this function get the vector from the BFS function and create a tree by using the vec
    Tree* tree;
    if (treeType == MaxRank){tree = new MaxRankTree(root,bfsVec);}
    else if (treeType == Cycle){tree = new CycleTree(root, cycle);}
    else {tree = new RootTree(root);}
    int len = bfsVec.size();
    for (int i = 0 ; i < len ; i++){
        if (bfsVec[i][2] == root){
            Tree* curr = &(*createTreeRec(i ,cycle, bfsVec , treeType));
            tree->addChild(*curr);
            delete curr;
        }
    }
    return tree;
}
int Tree::getNumberOfChildren() const{return children.size();}

////////////////////////////////////////////////////////////////////Cycle Tree////////////////////////
//rule of 5
CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel) , currCycle(currCycle){
}
CycleTree::~CycleTree(){
    clear();
}
CycleTree::CycleTree(const CycleTree& other) : Tree(other) , currCycle(other.currCycle){}
CycleTree& CycleTree::operator=(const CycleTree& other){
    if (this != &other){
        clear();
        delete &currCycle;
        setNode(other.getNode());
        setChildren(other.getChildren());
        this->currCycle = other.currCycle;
    }

    return *this;
}
//other functions
Tree* CycleTree::clone() const{
    return new CycleTree(*this);
}
CycleTree::CycleTree(CycleTree&& other) : Tree(other) , currCycle(other.currCycle) {
}
CycleTree& CycleTree::operator=(CycleTree&& other){
    if (this != &other){
        clear();
        this->setNode(other.getNode());
        currCycle = other.currCycle;
        int size = other.getChildren().size();
        for (int i = 0 ; i < size ; i++){
            this->addChild(*(other.getChildren()[i]));
        }
        other.clear();
    }
    return *this;
}
int CycleTree::traceTree(){
    return getCycleRec(this->currCycle , this);

}
int CycleTree::getCycleRec(int numOfRounds , Tree* tree){
    if (numOfRounds == 0){return tree->getNode();}
    else if(tree->getNumberOfChildren() ==0){return tree->getNode();}
    else{
        vector<Tree*> children = tree->getChildren();
        Tree* tree = children[0];
        return getCycleRec(numOfRounds - 1 , tree);
    }

}

///////////////////////////////////////////////////////////////////MaxRankTree//////////////////////
//rule of 5
MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) , depth(-1){

}
MaxRankTree::~MaxRankTree(){
}
MaxRankTree::MaxRankTree(const MaxRankTree& other) : Tree(other) , depth(other.depth) {


}
MaxRankTree& MaxRankTree::operator=(const MaxRankTree& other){
    if(this != &other){
        clear();
        setNode(other.getNode());
        setChildren(other.getChildren());
        depth = other.depth;
    }

    return *this;
}
MaxRankTree::MaxRankTree(int rootLabel , std::vector<std::vector<int>>& bfsVec) : Tree(rootLabel) , depth(bfsVec[rootLabel][1]) {}

//other functions
Tree* MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}
int MaxRankTree::traceTree() {
    std::vector<std::vector<int>*>* vec = getMaxRankAndDepthVec();
    if (vec->size() == 1){
        int a = (*(*vec)[0])[0];
        for (vector<int>* x : *vec){
            delete x;
        }
        delete vec;
        return a;
    }


    else{

        int depth(-1) ; //(0);
        int name(-1);   //(0);

        for (vector<int>* x : *vec) {
            if (depth == -1){
                depth = (*x)[1];
                name = (*x)[0];

            }

            if ((*x)[1] <= (depth) ){
                if ((*x)[1] < depth){
                    depth = (*x)[1];
                    name = (*x)[0];
                }
                else{
                    if ((*x)[0] < name){
                        name = (*x)[0];
                    }
                }
            }

        }
        for (vector<int>* x : *vec){
            delete x;
        }
        delete vec;
        return name;
    }

}
std::vector<std::vector<int>*>* MaxRankTree::getMaxRankAndDepthVec(){

    std::vector<std::vector<int>*>* vec = new vector<vector<int>*>();
    int a(this->getNode());
    getMaxRankAndDepthVecRec(a, 0, vec);
    return vec;
}
void MaxRankTree::getMaxRankAndDepthVecRec(int root, int depth , vector<vector<int>*>* vec) {


    vector<int>* curr = new vector<int>();
    curr->push_back(root);
    curr->push_back(depth);

    curr->push_back(this->getNumberOfChildren());

    if (vec->empty() || (*(*vec)[0])[2] == ((*curr)[2])){

        vec->push_back(curr);
    }

    else if ((*(*vec)[0])[2] < ((*curr)[2])){


        for (vector<int>* x : *vec){
            delete x;
            //x->clear();
        }
        vec->clear();
        vec->push_back(curr);
    }

    else {
        delete curr;}

    for (Tree* tree : this->getChildren()){
        int a(tree->getNode());
        ((MaxRankTree*)tree)->getMaxRankAndDepthVecRec(a, depth + 1, vec);
    }

}
MaxRankTree::MaxRankTree(MaxRankTree&& other) : Tree(other) , depth(other.depth) {
}
MaxRankTree& MaxRankTree::operator=(MaxRankTree&& other){
    if (this != &other){
        this->setNode(other.getNode());
        this->setChildren(other.getChildren());
        this->depth = other.depth;
    }
    other.clear();
    return *this;
}




/////////////////////////////////Root Tree/////////////////////////
//rule of 5
RootTree::RootTree(int rootLabel) : Tree(rootLabel){
}
RootTree::~RootTree(){
    clear();
}
RootTree::RootTree(const RootTree& other) : Tree(other){ }
RootTree& RootTree::operator=(const RootTree& other){
    if (this != &other){
        clear();
        setNode(other.getNode());
        setChildren(other.getChildren());
    }
    return *this;
}

//other functions
Tree* RootTree::clone()const  {
    return new RootTree(*this);
}
int RootTree::traceTree() {
    return this->getNode();
}
