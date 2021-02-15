//
// Created by spl211 on 10/11/2020.
//

#ifndef AGENT_H_
#define AGENT_H_

#include "Session.h"

class Agent {
public:
    //rule of 5
    Agent();
    virtual ~Agent();
    Agent(const Agent& other);

    //functions
    virtual void act(Session& session)=0;
    virtual Agent* clone() const=0 ;
    virtual int getNodeInd() const=0;
};

class ContactTracer: public Agent{
public:
    //rule of 5
    ContactTracer();
    ~ContactTracer();
    ContactTracer(const ContactTracer& other);

    //getters and setters
    int getNodeInd() const;

    //functions
    virtual void act(Session& session);
    virtual Agent* clone() const;
};

class Virus: public Agent{
public:
    //rule of 5
    Virus(int nodeInd);
    ~Virus();
    Virus(const Virus& other);

    //getters and setters
    int getNodeInd() const;

    //functions
    virtual void act(Session& session);
    virtual Agent* clone() const;

private:
    const int nodeInd;
};


#endif //TASK1_AGENT_H
