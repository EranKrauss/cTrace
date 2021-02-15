#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "Session.h"

class Session;

class Tree{
public:
    //rule of 5
    Tree(int rootLabel);
    virtual ~Tree();
    Tree(const Tree& other);
    Tree& operator=(const Tree& other);
    void clear();
    virtual Tree* clone() const = 0;
    Tree(Tree&& other);
    Tree& operator=(Tree&& other);

    //getters and setters
    void setNode(int node);
    int getNode() const;
    void setChildren(std::vector<Tree*>);
    std::vector<Tree*> getChildren() const;
    int getNumberOfChildren() const;


    //functions

    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;


private:
    int node;
    std::vector<Tree*> children;
    static Tree* createTreeRec(int ,int , std::vector<std::vector<int>>&, TreeType);
    static std::vector<std::vector<int>>* bfs(const Session& session, int rootLabel);

};

class CycleTree: public Tree{
public:
    //rule of 5
    CycleTree(int rootLabel, int currCycle);
    ~CycleTree();
    CycleTree(const CycleTree& other);
    CycleTree& operator=(const CycleTree& other);
    CycleTree(CycleTree&& other);
    CycleTree& operator=(CycleTree&& other);

    //functions
    virtual Tree* clone() const;
    virtual int traceTree();

private:
    int currCycle;
    int getCycleRec(int numOfRounds, Tree *tree);
};

class MaxRankTree: public Tree{
public:
    //rule of 5
    MaxRankTree(int rootLabel);
    ~MaxRankTree();
    MaxRankTree(const MaxRankTree& other);
    MaxRankTree& operator=(const MaxRankTree& other);
    MaxRankTree(int rootLabel , std::vector<std::vector<int>>& bfsVec);
    MaxRankTree(MaxRankTree&& other);
    MaxRankTree& operator=(MaxRankTree&& other);


    //functions
    virtual int traceTree();
    virtual Tree* clone() const;


private:
    int depth;

    //private functions
    vector<std::vector<int>* > *getMaxRankAndDepthVec();
    void getMaxRankAndDepthVecRec(int root, int depth, vector<vector<int> *> *vec);
};

class RootTree: public Tree{
public:
    //rule of 5
    RootTree(int rootLabel);
    ~RootTree();
    RootTree(const RootTree& other);
    RootTree& operator=(const RootTree& other);

    //functions
    virtual Tree* clone() const;
    virtual int traceTree();
};

#endif
