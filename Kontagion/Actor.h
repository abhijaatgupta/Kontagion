#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
    Actor(int imID, int lives, Direction dir, double startX, double startY, int depth, double size, int health, StudentWorld* sWorld);
    virtual ~Actor();
    virtual void doSomething();
    StudentWorld* getAPointerToMyStudentWorld();
    int getHealth();
    void setHealth(int health);
    int getLives();
    void setLives(int lives);
    virtual bool isDamageable(){return false;}
    virtual bool isFood(){return false;}
    virtual bool isPit(){return false;}
    virtual bool isSalmonella(){return false;}
    virtual bool isBacteria(){return false;}
    virtual bool isDirt(){return false;}
private:
    int m_lives;
    int m_health;
    StudentWorld* m_sworld;
};

class Socrates: public Actor{
public:
    Socrates(StudentWorld* sWorld);
    virtual ~Socrates();
    virtual void doSomething();
    int getSprays();
    void useSpray();
    int getFlames();
    void useFlame();
    void gainFlame();
    virtual bool isDamageable();
private:
    int m_sprays;
    int m_flames;
};

class Dirt: public Actor{
public:
    Dirt(StudentWorld* sWorld, double x, double y);
    virtual ~Dirt();
    virtual bool isDirt(){return true;}
    virtual bool isDamageable();
};

class Projectile: public Actor{
public:
    Projectile(StudentWorld* sWorld, double x, double y, Direction dir, int dam, int pix, int imid);
    virtual ~Projectile();
    virtual void doSomething();
private:
    int m_pixels;
    int m_damage;
};

class Spray: public Projectile{
public:
    Spray(StudentWorld* sWorld, int x, int y, Direction dir);
    virtual ~Spray();
};

class Flame: public Projectile{
public:
    Flame(StudentWorld* sWorld, int x, int y, Direction dir);
    virtual ~Flame();
};

class Food : public Actor{
public:
    Food(StudentWorld* sWorld, int x, int y);
    virtual ~Food(){}
    virtual bool isFood(){return true;}
};

class droppedItem : public Actor {
public:
    droppedItem(StudentWorld* sWorld, double x, double y, int Imid, double life);
    virtual ~droppedItem(){}
    virtual bool isDamageable(){return true;}
    virtual void doSomething(){}
    double getlifetime(){return m_lifetime;}
    void lifeLoss(){m_lifetime--;}
private:
    double m_lifetime;
};

class Fungus : public droppedItem {
public:
    Fungus(StudentWorld* sWorld, double x, double y, double life);
    virtual ~Fungus(){}
    virtual void doSomething();
};

class ExtraLife : public droppedItem {
public:
    ExtraLife(StudentWorld* sWorld, double x, double y, double life);
    virtual ~ExtraLife(){}
    virtual void doSomething();
};

class FlameCharge : public droppedItem {
public:
    FlameCharge(StudentWorld* sWorld, double x, double y, double life);
    virtual ~FlameCharge(){}
    virtual void doSomething();
};

class RestoreHealth : public droppedItem {
public:
    RestoreHealth(StudentWorld* sWorld, double x, double y, double life);
    virtual ~RestoreHealth(){}
    virtual void doSomething();
};

class Bacteria : public Actor {
public:
    Bacteria(StudentWorld* sWorld, double x, double y, int health, int IID);
    virtual ~Bacteria(){}
    virtual void doSomething(){}
    virtual bool isDamageable(){return true;}
    virtual bool isBacteria(){return true;}
    void resetMovement();
    void reproduce(int Case);
    bool canMoveForward(double direction, double& x, double& y, double units);
    void salMoveAlgorithm();
    double getmovePlan(){return m_movePlanDist;}
    void setmovePlan(double newMovePlan){m_movePlanDist = newMovePlan;}
    int getfoodEaten(){return m_foodEaten;}
    void setFoodEaten(int food){m_foodEaten = food;}
private:
    double m_movePlanDist;
    int m_foodEaten;
};

class regSalmonella : public Bacteria {
public:
    regSalmonella(StudentWorld* sWorld, double x, double y);
    virtual ~regSalmonella(){}
    virtual void doSomething();
    virtual bool isSalmonella(){return true;}
};

class aggroSalmonella : public Bacteria {
public:
    aggroSalmonella(StudentWorld* sWorld, double x, double y);
    virtual ~aggroSalmonella(){}
    virtual void doSomething();
    virtual bool isSalmonella(){return true;}
};

class Ecoli : public Bacteria {
public:
    Ecoli(StudentWorld* sWorld, double x, double y);
    virtual ~Ecoli(){}
    virtual void doSomething();
};

class pit : public Actor {
public:
    pit(StudentWorld* sWorld, double x, double y);
    virtual ~pit(){}
    virtual bool isPit(){return true;}
    virtual void doSomething();
private:
    int numAggroSal;
    int numRegSal;
    int numEcoli;
};
#endif // ACTOR_H_

