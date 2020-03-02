#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
class Actor;
class Socrates;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void initObj(int numPiles, bool noOverlap, int Case);
    void addToAllActors(Actor* a);
    bool damageableObjectAt(double x, double y, Actor* &damAct);
    bool overlap(double x, double y, Actor* act);
    bool overlapWithSocrates(double x, double y);
    bool overlapWithFood(double x, double y);
    bool findNearestFood(double& x, double& y, double dist, double xObj, double yObj);
    bool overlapWithDirt(double x, double y);
    double distActors(double x1, double y1, double x2, double y2);
    void gameStats();
    void addnewActors();
    Socrates* getSocrates(){return socrates;}
private:
    std::vector<Actor*> allActors;
    Socrates* socrates;
};

#endif // STUDENTWORLD_H_
