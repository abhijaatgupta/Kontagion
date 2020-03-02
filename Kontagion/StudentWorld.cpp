#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;
#include "Actor.h"
#define PI 3.14159265
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
StudentWorld::~StudentWorld(){
    cleanUp();
}
int StudentWorld::init()
{
    socrates = new Socrates(this);
    int numPiles =  max(180 - 20*getLevel(), 20);
    initObj(numPiles, false, 1);
    numPiles = min(5 * getLevel(), 25); //food
    initObj(numPiles, true, 2);
    initObj(getLevel(), true, 3);
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
    bool levelComplete = true;
    socrates->doSomething();
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            allActors[i]->doSomething();
        }
    }
    gameStats();
    if (socrates->getHealth() <= 0){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            if (allActors[i]->isPit() && allActors[i]->getLives() > 0){
                levelComplete = false;
            }
            if (allActors[i]->getLives() <= 0){
                delete allActors[i];
                allActors[i] = nullptr;
            }
        }
    }
    if (levelComplete){
        for (int i = 0; i < allActors.size(); i++){
            if (allActors[i] != nullptr){
                if (allActors[i]->isBacteria()){
                    levelComplete = false;
                }
            }
        }
    }
    if (levelComplete){
        return GWSTATUS_FINISHED_LEVEL;
    }
    addnewActors();
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::gameStats(){
    ostringstream gameStats;
    gameStats << "Score: ";
    gameStats.fill('0');
    gameStats << setw(6) << getScore();
    gameStats << "  Level:";
    gameStats.fill(' ');
    gameStats << setw(3) << getLevel();
    gameStats << "  Lives:" << setw(2) << getLives();
    gameStats << "  Health:" << setw(4) << socrates->getHealth();
    gameStats << "  Sprays:" << setw(3) << socrates->getSprays();
    gameStats << "  Flames:" << setw(3) << socrates->getFlames();
    setGameStatText(gameStats.str());
}
void StudentWorld::initObj(int numPiles, bool noOverlap, int Case){
    Actor* obj;
    for(int i = 0; i < numPiles; i++){
        double j = 0;
        double k = 0;
        if (allActors.size() == 0){
            j = 120.0;
            k = 130.0;
        }
        for (int n = 0; n < allActors.size(); n++){
            if (noOverlap){
                if (allActors[n] == nullptr)
                    break;
                while(overlap(j, k, allActors[n]) || sqrt((VIEW_WIDTH/2 - j)*(VIEW_WIDTH/2 - j) + (VIEW_HEIGHT/2 - k)*(VIEW_HEIGHT/2 - k)) >= VIEW_RADIUS - 8){
                    j = randInt(0, VIEW_RADIUS*2);
                    k = randInt(0, VIEW_RADIUS*2);
                    n = 0;
                }
            }
            else {
                while(sqrt((VIEW_RADIUS - j)*(VIEW_RADIUS - j) + (VIEW_RADIUS - k)*(VIEW_RADIUS - k)) >= VIEW_RADIUS - 8){
                    j = randInt(0, VIEW_RADIUS*2);
                    k = randInt(0, VIEW_RADIUS*2);
                }
            }
        }
        if (Case == 1){
            obj = new Dirt(this, j, k);
            allActors.push_back(obj);
        }
        if (Case == 2){
            obj = new Food(this, j, k);
            allActors.push_back(obj);
        }
        if (Case == 3){
            obj = new pit(this, j, k);
            allActors.push_back(obj);
        }
    }
}

void StudentWorld::cleanUp()
{
    if (socrates != nullptr){
        delete socrates;
        socrates = nullptr;
    }
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            delete allActors[i];
        }
    }
    for (int i = 0; i < allActors.size();){
        allActors.erase(allActors.begin());
    }
}

void StudentWorld::addToAllActors(Actor* a){
    allActors.push_back(a);
}

bool StudentWorld::damageableObjectAt(double x, double y, Actor* &damAct){
    for (int i = 1; i < allActors.size(); i++){
        if (allActors[i] != nullptr && allActors[i]->isDamageable()){
            if (overlap(x, y, allActors[i])){
                damAct = allActors[i];
                return true;
            }
        }
    }
    return false;
}
bool StudentWorld::overlap(double x, double y, Actor* act){
    if (sqrt((act->getX() - x)*(act->getX() - x) + (act->getY() - y)*(act->getY() - y)) <= SPRITE_RADIUS*2){
        return true;
    }
    return false;
}
bool StudentWorld::overlapWithSocrates(double x, double y){
    return overlap(x, y, socrates);
}
bool StudentWorld::overlapWithFood(double x, double y){
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            if (allActors[i]->isFood()){
                if(overlap(x, y, allActors[i])){
                    allActors[i]->setLives(0);
                    return true;
                }
            }
        }
    }
    return false;
}
bool StudentWorld::findNearestFood(double& x, double& y, double dist, double xObj, double yObj){
    x = 10000;
    y = 10000;
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            if (allActors[i]->isFood()){
                if (distActors(xObj, yObj, allActors[i]->getX(), allActors[i]->getY()) < distActors(xObj, yObj, x, y)){
                    x = allActors[i]->getX();
                    y = allActors[i]->getY();
                }
            }
        }
    }
    if (distActors(x, y, xObj, yObj) <= 128){
        return true;
    }
    return false;
}
double StudentWorld::distActors(double x1, double y1, double x2, double y2){
    return (sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}
bool StudentWorld::overlapWithDirt(double x, double y){
    for (int i = 0; i < allActors.size(); i++){
        if (allActors[i] != nullptr){
            if (allActors[i]->isDirt()){
                if (sqrt((allActors[i]->getX() - x)*(allActors[i]->getX() - x) + (allActors[i]->getY() - y)*(allActors[i]->getY() - y)) <= SPRITE_WIDTH/2){
                    return true;
                }
            }
        }
    }
    return false;
}
void StudentWorld::addnewActors(){
    int ChanceFungus = max(510 - getLevel() * 10, 200);
    int ChanceGoodie = max(510 - getLevel() * 10, 250);
    int drop = randInt(0, ChanceFungus);
    if (drop == 0){
        int loc = randInt(0, 360/5);
        loc = loc*5;
        Fungus* fung = new Fungus(this, VIEW_RADIUS * (1-cos(loc*PI/180)), VIEW_RADIUS * (1-sin(loc*PI/180)), max(rand() % (300 - 10 * getLevel()), 50));
        allActors.push_back(fung);
        
    }
    drop = randInt(0, ChanceGoodie);
    if (drop == 0){
        int loc = randInt(0, 360/5);
        loc = loc*5;
        int n = randInt(0, 9);
        droppedItem* goodie;
        if (n == 0){
            goodie = new ExtraLife(this, VIEW_RADIUS * (1-cos(loc*PI/180)), VIEW_RADIUS * (1-sin(loc*PI/180)), max(rand() % (300 - 10 * getLevel()), 50));
            allActors.push_back(goodie);
        }
        else if (n <= 3){
            goodie = new FlameCharge(this, VIEW_RADIUS * (1-cos(loc*PI/180)), VIEW_RADIUS * (1-sin(loc*PI/180)), max(rand() % (300 - 10 * getLevel()), 50));
            allActors.push_back(goodie);
        }
        else{
            goodie = new RestoreHealth(this, VIEW_RADIUS * (1-cos(loc*PI/180)), VIEW_RADIUS * (1-sin(loc*PI/180)), max(rand() % (300 - 10 * getLevel()), 50));
            allActors.push_back(goodie);
        }
    }
}
